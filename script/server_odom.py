import socket
import time
import json

HOST = '127.0.0.1'  # Local IP address
PORT = 9998         # Port the server listens on

# Function to create data with dynamic x and y values
def generate_data(x, y):
    return {
        "header": {
            "seq": 76,
            "stamp": {"secs": int(time.time()), "nsecs": 0},
            "frame_id": "odom"
        },
        "child_frame_id": "base_footprint",
        "pose": {
            "pose": {
                "position": {"x": x, "y": y, "z": 0.0},
                "orientation": {"x": 0.0, "y": 0.0, "z": -0.0013921211939305067, "w": 0.9999990463256836}
            },
            "covariance": [0.0]
        },
        "twist": {
            "twist": {
                "linear": {"x": -0.0002410541637800634, "y": 0.0, "z": 0.0},
                "angular": {"x": 0.0, "y": 0.0, "z": -0.01978989876806736}
            },
            "covariance": [0.0]
        }
    }

# Server setup
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Server running on {HOST}:{PORT}")

    conn, addr = s.accept()
    print(f"Connected by {addr}")
    with conn:
        x = 0.0
        y = 0.0
        while True:
            # Generate data with increasing x and y values
            x += 0.1
            y += 0.1

            data = generate_data(x, y)
            message = f"---START---{json.dumps(data)}___END___"

            # Send data to the client
            conn.sendall(message.encode('utf-8'))
            print(f"Sent: x={x}, y={y}")

            time.sleep(1)  # Delay between messages