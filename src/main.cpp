#include "client/client.h"
#include "shared_memory/shared_memory.h"
#include <atomic>
#include <iostream>
#include <stdexcept>
#include <thread>
#include "laser_data/laser_data.h"
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

    const int px_height = 720;
    const int px_width = 1080;
    std::vector<std::vector<float>> pixel_array(px_width, std::vector<float>(px_height, 1.0));
    float i=0.0;

    try
    {
        // Client client(9997);
        Client client(8080);
        while (1)
        {
            try
            {
                std::string received_data = client.ReceiveData();

                // std::cout << received_data[0] << received_data[1] << received_data[2] << received_data[3] << received_data[4] << received_data[5] << received_data[6] << received_data[7] << received_data[8] << received_data[9] << received_data[10] << std::endl;

                // Include data evaluation for Laserscan

                // std::stringstream buffer;
                // Extract JSON content
                std::string json_content = extract_json(received_data);

                // Parse the JSON content
                ondemand::parser parser;
                ondemand::document data = parser.iterate(json_content);

                // Access ranges array
                ondemand::array ranges = data["ranges"];
                
                calc_pixel_coord(ranges, pixel_array, px_height, px_width, 144, 0.0, i, 0.0);
                // std::cout << "Next ITERATION" << std::endl;
                /*
                for(auto row : pixel_array)
                {
                    for(auto pixel : row)
                    {
                        std::cout << pixel << " ";
                    }
                    std::cout << std::endl;
                }

                
                std::cout << "Ranges: " << std::endl;

                size_t count = 0;
                for (auto dist : ranges)
                {
                    std::cout << double(dist) << std::endl;
                    count++;
                }

                std::cout << "Number of ranges: " << count << std::endl;

                sleep(1);
                */
            }

            catch (const std::runtime_error &e)
            {

                std::cerr << "Error: " << e.what() << std::endl;
                return 1;
            }

            i+=0.01;
            usleep(100000);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}