#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "odom.h"
#include "../include/client/client.h"
#include <thread>
#include "../include/laser_data/laser_data.h"
#include <vector>
/**
 * @file main.cpp
 * @brief A simple test application to demonstrate client-server communication.
 *
 * This program is designed to test the connection between a client and a server.
 * It continuously receives data from the server and prints it to the console.
 *
 * @note This is a test program and is not intended for production use.
 * It runs an infinite loop and lacks advanced error handling or resource management.
 */

extern void SendCmdVel(int port);
extern bool CreateSharedMemory(const std::string &name, size_t size, void *&memory);
extern bool AttachSharedMemory(const std::string &name, size_t size, void *&memory);
extern void DetachSharedMemory(void *memory, size_t size);
extern void DestroySharedMemory(const std::string &name);

/*
we can use shared_memory  to give a region in memory
a name and save our data from the imu and laserscan to this region in memory

we can then read this data from python or rust with the same name

we need to use semaphores for this so that we dont clash trying to access the memory
at the same time
*/

int main()
{

    // try
    // {
    //     SendCmdVel(9999);
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }

    const int px_height = 1080;
    const int px_width = 1920;
    float i = 0.0;

    try
    {
        Client client(9997);
        // Client client(8080);
        while (1)
        {
            try
            {
                try
                {
                    const std::string shm_name = "/odom_data";

                    // Open shared memory
                    int shm_fd = shm_open(shm_name.c_str(), O_RDONLY, 0666);
                    if (shm_fd < 0)
                    {
                        perror("shm_open failed");
                        throw std::runtime_error("Failed to open shared memory");
                    }

                    // Map shared memory
                    void *shm_ptr = mmap(nullptr, sizeof(OdomData), PROT_READ, MAP_SHARED, shm_fd, 0);
                    if (shm_ptr == MAP_FAILED)
                    {
                        perror("mmap failed");
                        close(shm_fd);
                        throw std::runtime_error("Failed to map shared memory");
                    }
                    OdomData *odomData = static_cast<OdomData *>(shm_ptr);

                    std::string received_data = client.ReceiveData();

                    // Extract JSON content
                    std::string json_content = extract_json(received_data);

                    // Parse the JSON content
                    ondemand::parser parser;
                    ondemand::document data = parser.iterate(json_content);

                    // Access ranges array
                    ondemand::array ranges = data["ranges"];

                    create_map(ranges, px_height, px_width, 72, -odomData->y, -odomData->x, -odomData->theta);
                    // create_map(ranges, px_height, px_width, 144, 0.0, 0.0, 0.0);
                    sleep(1);
                }
                catch (...)
                {
                }
            }

            catch (const std::runtime_error &e)
            {

                std::cerr << "Error: " << e.what() << std::endl;
                return 1;
            }

            i += 0.01;
            usleep(10000);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}