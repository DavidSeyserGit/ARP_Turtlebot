import socket

def start_server(host='127.0.0.1', port=9997):
    """
    Start a TCP server that listens for connections and prints received messages.
    """
    # Create a socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    try:
        # Bind the socket to the specified host and port
        server_socket.bind((host, port))
        print(f"Server started. Listening on {host}:{port}...")
        
        # Listen for incoming connections
        server_socket.listen(1)
        
        while True:
            # Accept a connection
            client_socket, client_address = server_socket.accept()
            print(f"Connection established with {client_address}")
            
            # Receive data
            while True:
                data = client_socket.recv(1024)  # Buffer size of 1024 bytes
                if not data:
                    print("Client disconnected.")
                    break
                
                # Print received data
                print("Received:", data.decode('utf-8'))
            
            # Close the client socket
            client_socket.close()
            print("Client connection closed.")
    
    except KeyboardInterrupt:
        print("\nServer shutting down.")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        # Close the server socket
        server_socket.close()

if __name__ == "__main__":
    start_server()