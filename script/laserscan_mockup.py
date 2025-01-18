import numpy as np
import time
from multiprocessing import shared_memory

def create_shared_memory_array(name, shape, dtype):
    size = np.prod(shape) * np.dtype(dtype).itemsize
    try:
        shm = shared_memory.SharedMemory(name=name, create=True, size=size)
        print(f"Writer: Shared memory block '{name}' created with size {size} bytes.")
    except FileExistsError:
        shm = shared_memory.SharedMemory(name=name)
        print(f"Writer: Shared memory block '{name}' already exists. Attaching.")
    shared_array = np.ndarray(shape, dtype=dtype, buffer=shm.buf)
    return shm, shared_array

def simulate_laser_scan(num_points, max_range, noise_std):
    """
    Simulates a laser scan with random obstacles and noise.
    Returns `0` for ranges without obstacles or not between 1 and `max_range`.

    Args:
        num_points: Number of points in the scan.
        max_range: Maximum range of the laser scanner.
        noise_std: Standard deviation of the Gaussian noise added to the range values.

    Returns:
        A numpy array of size (num_points,) containing the simulated range values.
    """
    angles = np.linspace(0, 2 * np.pi, num_points, endpoint=False)  # Full circle scan

    # Generate base range values
    ranges = np.zeros(num_points)  # Initialize with 0 (no obstacle detected)

    # Randomly generate obstacles
    num_obstacles = np.random.randint(1, 5)  # Random number of obstacles (1 to 4)
    for _ in range(num_obstacles):
        obstacle_range = np.random.uniform(1, max_range)  # Random obstacle distance
        obstacle_angle_start = np.random.uniform(0, 2 * np.pi)  # Random start angle
        obstacle_angle_end = obstacle_angle_start + np.random.uniform(0.1, 0.5)  # Random angular size

        # Ensure angle range is within bounds
        obstacle_angle_end = min(obstacle_angle_end, 2 * np.pi)

        # Mask for obstacle angle range
        mask = (angles >= obstacle_angle_start) & (angles <= obstacle_angle_end)
        ranges[mask] = obstacle_range

    # Add Gaussian noise to the range values
    noisy_ranges = ranges + np.random.normal(scale=noise_std, size=num_points)

    # Filter: Set values outside 1 and max_range to 0
    noisy_ranges[(noisy_ranges < 1) | (noisy_ranges > max_range)] = 0

    return noisy_ranges

def continuously_write_laser_scan(shared_array, shm, num_points, max_range, noise_std):
    try:
        while True:
            # Simulate laser scan data
            laser_scan_data = simulate_laser_scan(num_points, max_range, noise_std)

            # Copy data to shared memory
            np.copyto(shared_array, laser_scan_data)
            print("Writer: Wrote data:", laser_scan_data[:5])

            time.sleep(0.1)  # Reduced sleep time
    except KeyboardInterrupt:
        print("Writer: Stopped writing.")
    finally:
        print("Writer: Closing shared memory.")
        shm.close()

def main():
    array_shape = (360,)
    array_dtype = np.float32
    shared_memory_name = "laserscan"

    num_points = 360  # Number of points in the scan
    max_range = 3.0   # Maximum range of the laser scanner (meters)
    noise_std = 0.2   # Standard deviation of the Gaussian noise

    shm, shared_array = create_shared_memory_array(shared_memory_name, array_shape, array_dtype)
    continuously_write_laser_scan(shared_array, shm, num_points, max_range, noise_std)

if __name__ == "__main__":
    main()