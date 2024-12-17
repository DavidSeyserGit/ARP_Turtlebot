#include "client/client.h"
#include "shared_memory/shared_memory.h"
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

extern void SendCmdVel(int port);

#include "client/client.h"
#include "shared_memory/shared_memory.h"
#include <atomic>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono> // For std::this_thread::sleep_for

/**
 * @file main.cpp
 * @brief Test application to demonstrate a separate thread.
 */

extern void SendCmdVel(int port);

int main() {
    try {
        // Launch SendCmdVel in a separate thread
        std::thread cmd_vel_thread(SendCmdVel, 9997);

        if (cmd_vel_thread.joinable()) {
            std::cout << "SendCmdVel started in a separate thread." << std::endl;
            cmd_vel_thread.detach(); // Detach the thread to run independently
        }

        // Main thread task: Print a counter
        int counter = 0;
        while (counter < 10) {
            std::cout << "Main thread counter: " << counter++ << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work in the main thread
        }

        std::cout << "Main thread finished its task." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}