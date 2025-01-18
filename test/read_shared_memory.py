import mmap
import struct
import os
import time  # Für regelmäßige Abfragen

# Definition der Struktur OdomData
ODOM_DATA_FORMAT = "fff"  # Drei Floats für x, y, theta
ODOM_DATA_SIZE = struct.calcsize(ODOM_DATA_FORMAT)  # Größe der Struktur

SHARED_MEMORY_NAME = "/odom_data"

def read_shared_memory():
    try:
        # Öffnen der Shared Memory
        shm_fd = os.open(f"/dev/shm{SHARED_MEMORY_NAME}", os.O_RDONLY)
        
        # Mappe die Shared Memory in den Adressraum
        with mmap.mmap(shm_fd, ODOM_DATA_SIZE, mmap.MAP_SHARED, mmap.PROT_READ) as shared_memory:
            while True:  # Kontinuierlich lesen
                # Zurück zum Anfang der Shared Memory setzen
                shared_memory.seek(0)

                # Lese die Daten aus der Shared Memory
                data = shared_memory.read(ODOM_DATA_SIZE)
                
                # Entpacke die Daten gemäß der Strukturdefinition
                x, y, theta = struct.unpack(ODOM_DATA_FORMAT, data)
                print(f"Received Odometry Data:\nX: {x}\nY: {y}\nTheta: {theta}")

                # Warte 1 Sekunde, bevor die nächsten Daten gelesen werden
                time.sleep(1)

        # Schließe den File Descriptor
        os.close(shm_fd)

    except FileNotFoundError:
        print(f"Shared memory {SHARED_MEMORY_NAME} not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    read_shared_memory()
