import socket
import sys
from multiprocessing import shared_memory

def destroy_existing_shared_memory(name):
    """
    Destroy an existing shared memory block if it exists.
    """
    try:
        shm = shared_memory.SharedMemory(name=name)
        shm.close()
        shm.unlink()
        print(f"Existing shared memory '{name}' destroyed.")
    except FileNotFoundError:
        print(f"No existing shared memory '{name}' to destroy.")
    except Exception as e:
        print(f"Error destroying shared memory '{name}': {e}")

def start_server(host='127.0.0.1', port=9997, shm_size=1024):
    """
    Start a TCP server that creates a shared memory block and does nothing with it.
    """
    # Destroy existing shared memory at the start
    destroy_existing_shared_memory("odom_data")

    # Create shared memory block
    try:
        shm = shared_memory.SharedMemory(name="odom_data", create=True, size=shm_size)
        print("Shared memory created with name 'odom_data' and size:", shm_size)
    except Exception as e:
        print(f"Error creating shared memory: {e}")
        sys.exit(1)
    
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
        # Clean up
        print("Cleaning up shared memory and closing server.")
        shm.close()
        shm.unlink()
        server_socket.close()

if __name__ == "__main__":
    start_server()