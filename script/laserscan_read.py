import numpy as np
import cv2
from multiprocessing import shared_memory
from time import sleep

# --- Odometry Simulator ---
class OdometrySimulator:
    def __init__(self, start_x=0, start_y=0, start_theta=0, linear_speed=0.1, angular_speed=0.01):
        self.x = start_x
        self.y = start_y
        self.theta = start_theta
        self.linear_speed = linear_speed
        self.angular_speed = angular_speed
        self.step_count = 0
        self.state_duration = np.random.randint(50, 150)
        self.current_state = "STRAIGHT"
        self.steps_in_state = 0

    def update_state(self):
        self.steps_in_state += 1
        if self.steps_in_state >= self.state_duration:
            self.steps_in_state = 0
            self.state_duration = np.random.randint(50, 150)
            self.current_state = np.random.choice(["STRAIGHT", "DIAGONAL", "CIRCULAR"])
            print(f"Switching to state: {self.current_state} for {self.state_duration} steps")

    def update(self):
        self.update_state()
        if self.current_state == "STRAIGHT":
            self.y += self.linear_speed
        elif self.current_state == "DIAGONAL":
            self.theta += self.angular_speed
            self.x += self.linear_speed * np.cos(self.theta)
            self.y += self.linear_speed * np.sin(self.theta)
        elif self.current_state == "CIRCULAR":
            self.theta += self.angular_speed
            self.x += self.linear_speed * np.cos(self.theta)
            self.y += self.linear_speed * np.sin(self.theta)
        return self.x, self.y, self.step_count, self.theta


# --- Laser Scan Reader ---
def get_real_laser_scan_data():
    try:
        shm = shared_memory.SharedMemory(name="laserscan")
        shared_array = np.ndarray((360,), dtype=np.float32, buffer=shm.buf)
        return shared_array.copy()
    except FileNotFoundError:
        print("Shared memory not found. Ensure the writer script is running.")
        return np.zeros((360,), dtype=np.float32)

# --- Visualization ---
def visualize(scan_data_callback, odometry_simulator, map_size=(1000, 1000), scale=100):
    map_img = np.zeros((map_size[0], map_size[1], 3), dtype=np.uint8)
    center_x, center_y = map_size[0] // 2, map_size[1] // 2
    prev_x_odom, prev_y_odom = 0, 0

    cv2.namedWindow("Laser Scan and Odometry Map", cv2.WINDOW_NORMAL)

    while True:
        x_odom, y_odom, step_count, theta = odometry_simulator.update()
        scan_data = scan_data_callback()

        if scan_data is None or len(scan_data) == 0:
            print("No scan data available")
            continue

        angles = np.linspace(0, 2 * np.pi, len(scan_data), endpoint=False)
        x = scan_data * np.cos(angles)
        y = scan_data * np.sin(angles)
        x_map = (x * scale + center_x).astype(int)
        y_map = (y * scale + center_y).astype(int)

        dx = int((x_odom - prev_x_odom) * scale)
        dy = int((y_odom - prev_y_odom) * scale)

        if abs(dx) < map_size[1] and abs(dy) < map_size[0]:
            if dy > 0:
                map_img[dy:] = map_img[:-dy]
                map_img[:dy] = 0
            elif dy < 0:
                map_img[:dy] = map_img[-dy:]
                map_img[dy:] = 0
            if dx > 0:
                map_img[:, dx:] = map_img[:, :-dx]
                map_img[:, :dx] = 0
            elif dx < 0:
                map_img[:, :dx] = map_img[:, -dx:]
                map_img[:, dx:] = 0

        prev_x_odom, prev_y_odom = x_odom, y_odom

        valid_points = (x_map >= 0) & (x_map < map_size[1]) & (y_map >= 0) & (y_map < map_size[0])
        distances = scan_data[valid_points]

        if distances.size > 0:
            min_distance = np.min(distances)
            max_distance = np.max(distances)
            normalized_distances = (distances - min_distance) / (max_distance - min_distance) if max_distance > min_distance else np.zeros_like(distances)
            cmap = cv2.applyColorMap((normalized_distances * 255).astype(np.uint8), cv2.COLORMAP_JET)
            for i, (xm, ym) in enumerate(zip(x_map[valid_points], y_map[valid_points])):
                if 0 <= xm < map_size[1] and 0 <= ym < map_size[0]:
                    cv2.circle(map_img, (xm, ym), 1, tuple(cmap[i, 0].tolist()), -1)

        overlay = map_img.copy()
        text_info = [
            f"Step: {step_count}",
            f"X: {x_odom:.2f} m",
            f"Y: {y_odom:.2f} m",
            f"Theta: {np.degrees(theta):.2f} deg"
        ]
        for i, text in enumerate(text_info):
            cv2.putText(overlay, text, (10, 30 + i * 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)
        alpha = 0.8
        blended = cv2.addWeighted(overlay, alpha, map_img, 1 - alpha, 0)
        cv2.imshow("Laser Scan and Odometry Map", blended)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        sleep(0.1)

    cv2.destroyAllWindows()


# --- Main Function ---
def main():
    odometry_simulator = OdometrySimulator()
    visualize(get_real_laser_scan_data, odometry_simulator)

if __name__ == "__main__":
    main()