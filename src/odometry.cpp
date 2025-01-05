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
#include <json/json.h> // Assuming a JSON library is available

constexpr char kSharedMemoryName[] = "/odom_data";
constexpr size_t kSharedMemorySize = sizeof(OdomData);

struct OdomData {
    float x;
    float y;
    float theta;
};

void ProcessAndStoreOdometryData(Client& client) {
    // Open or create shared memory
    int shm_fd = shm_open(kSharedMemoryName, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        throw std::runtime_error("Failed to create shared memory");
    }

    if (ftruncate(shm_fd, kSharedMemorySize) == -1) {
        close(shm_fd);
        shm_unlink(kSharedMemoryName);
        throw std::runtime_error("Failed to set shared memory size");
    }

    void* shared_memory = mmap(0, kSharedMemorySize, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        close(shm_fd);
        shm_unlink(kSharedMemoryName);
        throw std::runtime_error("Failed to map shared memory");
    }

    auto* odom_data = static_cast<OdomData*>(shared_memory);

    // Receive data from the client
    std::string received_data = client.ReceiveData();
    std::cout << "Received data: " << received_data << std::endl;

    // Parse the JSON string
    try {
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errors;

        if (!Json::parseFromStream(reader, std::istringstream(received_data), &root, &errors)) {
            throw std::runtime_error("Failed to parse JSON: " + errors);
        }

        // Extract the necessary fields
        odom_data->x = root["pose"]["pose"]["position"]["x"].asFloat();
        odom_data->y = root["pose"]["pose"]["position"]["y"].asFloat();
        odom_data->theta = root["pose"]["pose"]["orientation"]["z"].asFloat();

        std::cout << "Processed Odometry Data: X=" << odom_data->x
                  << " Y=" << odom_data->y
                  << " Theta=" << odom_data->theta << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error processing data: " << e.what() << std::endl;
    }

    // Cleanup shared memory
    munmap(shared_memory, kSharedMemorySize);
    close(shm_fd);
}
