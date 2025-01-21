#include "odom.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <thread>

// Shared memory constants
constexpr char kSharedMemoryName[] = "/odom_data";
constexpr size_t kSharedMemorySize = sizeof(OdomData);

OdomData* InitializeSharedMemory(int& shm_fd) {
    // Clean up any previous shared memory object
    shm_unlink(kSharedMemoryName);

    // Create or open the shared memory object
    shm_fd = shm_open(kSharedMemoryName, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open failed");
        throw std::runtime_error("Failed to create or open shared memory");
    }

    // Set the size of the shared memory object
    if (ftruncate(shm_fd, kSharedMemorySize) == -1) {
        perror("ftruncate failed");
        close(shm_fd);
        throw std::runtime_error("Failed to set shared memory size");
    }

    // Map the shared memory object into the address space
    void* shared_memory = mmap(nullptr, kSharedMemorySize, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        throw std::runtime_error("Failed to map shared memory");
    }

    // Cast and initialize the shared memory
    auto* odom_data = static_cast<OdomData*>(shared_memory);
    *odom_data = {};  // Zero-initialize the struct
    std::cout << "Shared memory initialized at address: " << static_cast<void*>(odom_data) << std::endl;
    return odom_data;
}

void ProcessAndStoreOdometryData(Client& client, OdomData* odom_data) {
    while (true) {
        std::string received_data = client.ReceiveData();

        if (received_data.empty()) {
            std::cerr << "No data received or connection closed." << std::endl;
            break;
        }

        size_t start_pos = received_data.find("---START---");
        size_t end_pos = received_data.find("___END___");

        if (start_pos != std::string::npos && end_pos != std::string::npos) {
            std::string message = received_data.substr(start_pos + 11, end_pos - start_pos - 11);
            try {
                std::regex regex_x(R"("x":\s*(-?\d+\.\d+))");
                std::regex regex_y(R"("y":\s*(-?\d+\.\d+))");
                std::regex regex_theta(R"("z":\s*(-?\d+\.\d+))");

                std::smatch match;

                if (std::regex_search(message, match, regex_x)) {
                    odom_data->x = std::stof(match.str(1));
                }
                if (std::regex_search(message, match, regex_y)) {
                    odom_data->y = std::stof(match.str(1));
                }
                if (std::regex_search(message, match, regex_theta)) {
                    odom_data->theta = std::stof(match.str(1));
                }

                std::cout << "Odometry updated: x=" << odom_data->x
                          << ", y=" << odom_data->y
                          << ", theta=" << odom_data->theta << std::endl;

            } catch (const std::exception& e) {
                std::cerr << "Error processing data: " << e.what() << std::endl;
                break;
            }
        } else {
            std::cerr << "Invalid message format received." << std::endl;
        }
    }
}

int main(){
        try {
        std::cout << "Starting main function..." << std::endl;

        // Shared memory setup
        int shm_fd;
        OdomData* odom_data = InitializeSharedMemory(shm_fd);
        if (!odom_data) {
            throw std::runtime_error("Failed to initialize shared memory");
        }
        // Client setup
        Client odomClient(9998);
        // Process and store odometry data
        ProcessAndStoreOdometryData(odomClient, odom_data);
        //std::cout << "Odometry Data: x=" << odom_data->x
                //<< ", y=" << odom_data->y
                //<< ", theta=" << odom_data->theta << std::endl;

        // Simulate delay
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Main function finished." << std::endl;

        // Cleanup shared resources
        munmap(odom_data, sizeof(OdomData));
        close(shm_fd);
        shm_unlink(kSharedMemoryName);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}