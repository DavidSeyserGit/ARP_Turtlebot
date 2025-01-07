#include "client/client.h"
#include <atomic>
#include <iostream>
#include <stdexcept>
#include <thread>

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
struct OdomData {
    float x;
    float y;
    float theta;
}; //seyser der befehl extern geht nicht auf strukturen, und ich wollte jz nur für das struct keine explizite header datei erstellen deswegen habe ich es hier eins zu eins definiert

extern void SendCmdVel(int port);
extern OdomData* InitializeSharedMemory(int& shm_fd);
extern void ProcessAndStoreOdometryData(Client& client, OdomData* odom_data);

/*
we can use shared_memory  to give a region in memory 
a name and save our data from the imu and laserscan to this region in memory

we can then read this data from python or rust with the same name 

we need to use semaphores for this so that we dont clash trying to access the memory
at the same time
*/

int main() {
    /*
    try
    {
        SendCmdVel(9999);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    try
    {
        Client client(9998);
        while(1){
            try {
                std::string received_data = client.ReceiveData();

                std::cout << received_data << std::endl;

            } catch (const std::runtime_error& e) {
                
                std::cerr << "Error: " << e.what() << std::endl;
                return 1; 
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
    */
   	Client client(9998);
    int shm_fd;
    OdomData* odom_data = InitializeSharedMemory(shm_fd);
    while(true){
    ProcessAndStoreOdometryData(client, odom_data);
    }
    /*Cleanup shared memory
    munmap(odom_data, kSharedMemorySize);
    close(shm_fd);
    shm_unlink("/my_shared_memory");

    return 0;*/

}