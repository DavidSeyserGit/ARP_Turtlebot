#define _POSIX_C_SOURCE 200809L
#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <regex>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "client/client.h"

struct OdomData {
    float x;
    float y;
    float theta;
};

constexpr char kSharedMemoryName[] = "/odom_data";
constexpr size_t kSharedMemorySize = sizeof(OdomData);

// Function to initialize and create shared memory
OdomData* InitializeSharedMemory(int& shm_fd) {
    shm_fd = shm_open(kSharedMemoryName, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open failed");
        throw std::runtime_error("Failed to create or open shared memory");
    }

    // Set the size of the shared memory
    if (ftruncate(shm_fd, kSharedMemorySize) == -1) {
        perror("ftruncate failed");
        close(shm_fd);
        throw std::runtime_error("Failed to set shared memory size");
    }

    // Map shared memory into the process's address space
    void* shared_memory = mmap(0, kSharedMemorySize, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        throw std::runtime_error("Failed to map shared memory");
    }

    return static_cast<OdomData*>(shared_memory);
}

// Function to process and store odometry data in existing shared memory
void ProcessAndStoreOdometryData(Client& client, OdomData* odom_data) {
    // Receive data from the client
    std::string received_data = client.ReceiveData();
    //std::cout << "Received data: " << received_data << std::endl;

    // Parse the data using regular expressions
    try {
        std::regex regex_x(R"("x":\s*(-?\d+\.\d+))");
        std::regex regex_y(R"("y":\s*(-?\d+\.\d+))");
        std::regex regex_theta(R"("z":\s*(-?\d+\.\d+))");

        std::smatch match;

        if (std::regex_search(received_data, match, regex_x) && match.size() > 1) {
            odom_data->x = std::stof(match.str(1));
        } else {
            throw std::runtime_error("Failed to parse 'x' from received data");
        }

        if (std::regex_search(received_data, match, regex_y) && match.size() > 1) {
            odom_data->y = std::stof(match.str(1));
        } else {
            throw std::runtime_error("Failed to parse 'y' from received data");
        }

        if (std::regex_search(received_data, match, regex_theta) && match.size() > 1) {
            odom_data->theta = std::stof(match.str(1));
        } else {
            throw std::runtime_error("Failed to parse 'theta' from received data");
        }

        /*std::cout << "Processed Odometry Data: X=" << odom_data->x
                  << " Y=" << odom_data->y
                  << " Theta=" << odom_data->theta << std::endl;*/
    } catch (const std::exception& e) {
        std::cerr << "Error processing data: " << e.what() << std::endl;
    }
}
