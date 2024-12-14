#define _POSIX_C_SOURCE 200809L
#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "client/client.h"


constexpr char kSharedMemoryName[] = "/odom_data";
constexpr size_t kSharedMemorySize = 1024; // Size not declared

struct OdometryData {
    double x;
    double y;
    double theta;
};


void ProcessAndStoreOdometryData(Client& client) {
    // Setup shared memory
    int shm_fd = shm_open(kSharedMemoryName, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        throw std::runtime_error("Failed to create shared memory");
    }

    if (ftruncate(shm_fd, kSharedMemorySize) == -1) {
        shm_unlink(kSharedMemoryName);
        throw std::runtime_error("Failed to set shared memory size");
    }

    void* shared_memory = mmap(0, kSharedMemorySize, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        shm_unlink(kSharedMemoryName);
        throw std::runtime_error("Failed to map shared memory");
    }

    auto* odom_data = static_cast<OdometryData*>(shared_memory);

    // Receive data from the client
    std::string received_data = client.ReceiveData();
    std::cout << received_data << std::endl;

    // Process the received data
    // Placeholder: Simulated parsing logic
    odom_data->x = 1.0; // Replace with parsed value from received_data
    odom_data->y = 2.0; // Replace with parsed value from received_data
    odom_data->theta = 3.1415; // Replace with parsed value from received_data

    /*std::cout << "Processed Odometry Data: X=" << odom_data->x
              << " Y=" << odom_data->y
              << " Theta=" << odom_data->theta << std::endl;*/

    // Cleanup shared memory
    munmap(shared_memory, kSharedMemorySize);
    shm_unlink(kSharedMemoryName);
}
