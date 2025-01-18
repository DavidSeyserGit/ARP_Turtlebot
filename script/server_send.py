import struct
from multiprocessing import shared_memory

def set_shared_memory(x, y, theta):
    """
    Initialize shared memory and set specific values for x, y, and theta.
    """
    shm_name = "odom_data"
    shm_size = struct.calcsize('fff')  # Size for 3 floats: x, y, theta

    # Destroy existing shared memory if needed
    try:
        existing_shm = shared_memory.SharedMemory(name=shm_name)
        existing_shm.close()
        existing_shm.unlink()
        print(f"Existing shared memory '{shm_name}' destroyed.")
    except FileNotFoundError:
        print(f"No existing shared memory '{shm_name}' to destroy.")
    except Exception as e:
        print(f"Error destroying shared memory '{shm_name}': {e}")

    # Create new shared memory
    try:
        shm = shared_memory.SharedMemory(name=shm_name, create=True, size=shm_size)
        print(f"Shared memory '{shm_name}' created with size {shm_size}.")

        # Write values to shared memory
        data = struct.pack('fff', x, y, theta)
        shm.buf[:shm_size] = data
        print(f"Shared memory set with: x={x}, y={y}, theta={theta}")

        return shm
    except Exception as e:
        print(f"Error creating or initializing shared memory: {e}")
        return None

if __name__ == "__main__":
    # Set specific values
    shm = set_shared_memory(1.0, 2.0, 3.14)  # Example values
    if shm:
        input("Press Enter to clean up shared memory and exit...")
        shm.close()
        shm.unlink()