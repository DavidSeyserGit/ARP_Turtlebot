import socket

def start_server(host='127.0.0.1', port=9999):
    """
    Start a simple TCP server that receives messages and prints them to the console.
    """
    # Create a TCP server socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    try:
        server_socket.bind((host, port))
        server_socket.listen(1)
        print(f"Server started. Listening on {host}:{port}...")

        while True:
            print("Waiting for a client connection...")
            client_socket, client_address = server_socket.accept()
            print(f"Connection established with {client_address}")

            try:
                while True:
                    # Receive data from the client
                    data = client_socket.recv(1024)
                    if not data:
                        print("Client disconnected.")
                        break

                    # Print the received message
                    print("Received message:", data.decode('utf-8'))
            except Exception as e:
                print(f"Error with client connection: {e}")
            finally:
                client_socket.close()
                print("Client connection closed.")
    except KeyboardInterrupt:
        print("\nServer shutting down.")
    except Exception as e:
        print(f"Server error: {e}")
    finally:
        server_socket.close()

if __name__ == "__main__":
    start_server()