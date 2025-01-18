#define _POSIX_C_SOURCE 200809L
#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct OdomData {
    float x;
    float y;
    float theta;
};

constexpr char kSharedMemoryName[] = "/odom_data";
constexpr size_t kSharedMemorySize = sizeof(OdomData);

int main() {
    // Open shared memory
    int shm_fd = shm_open(kSharedMemoryName, O_RDONLY, 0666);
    if (shm_fd < 0) {
        throw std::runtime_error("Failed to open shared memory");
    }

    // Map shared memory
    void* shared_memory = mmap(0, kSharedMemorySize, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        close(shm_fd);
        throw std::runtime_error("Failed to map shared memory");
    }

    // Access the data
    auto* odom_data = static_cast<OdomData*>(shared_memory);

    // Output the data
    std::cout << "Read from Shared Memory: " << std::endl;
    std::cout << "X: " << odom_data->x << std::endl;
    std::cout << "Y: " << odom_data->y << std::endl;
    std::cout << "Theta: " << odom_data->theta << std::endl;

    // Cleanup
    munmap(shared_memory, kSharedMemorySize);
    close(shm_fd);

    return 0;
}
