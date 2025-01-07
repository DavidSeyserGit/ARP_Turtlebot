import socket

HOST = '127.0.0.1'  # Lokale IP-Adresse
PORT = 9998         # Port, den der Client verwendet

# Daten, die an den Client gesendet werden
data = '''---START---{"header": {"seq": 76, "stamp": {"secs": 1706582145, "nsecs": 968175994}, "frame_id": "odom"}, "child_frame_id": "base_footprint", "pose": {"pose": {"position": {"x": -2.5310608180006966e-05, "y": 9.952720603223497e-08, "z": 0.0}, "orientation": {"x": 0.0, "y": 0.0, "z": -0.0013921211939305067, "w": 0.9999990463256836}}, "covariance": [0.0]}, "twist": {"twist": {"linear": {"x": -0.0002410541637800634, "y": 0.0, "z": 0.0}, "angular": {"x": 0.0, "y": 0.0, "z": -0.01978989876806736}}, "covariance": [0.0]}}___END___'''

# Server starten
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Server running on {HOST}:{PORT}")

    conn, addr = s.accept()
    print(f"Connected by {addr}")
    with conn:
        while True:
            conn.sendall(data.encode('utf-8'))
