#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "odom.h"
#include "client/client.h"
#include <thread>

constexpr char kSharedMemoryName[] = "/odom_data";
void SendCmdVel(int port);
int main() {
    /*
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
        std::cout << "Odometry Data: x=" << odom_data->x
                << ", y=" << odom_data->y
                << ", theta=" << odom_data->theta << std::endl;

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
    */

    SendCmdVel(9999);
    return 0;
}