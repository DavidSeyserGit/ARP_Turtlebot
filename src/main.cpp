#include "../include/client/client.h"
#include <iostream>
#include <stdexcept>

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

int main() {
    Client client(9997);
    while(1){
        try {
            std::string received_data = client.ReceiveData();

            std::cout << "data rec: " << received_data << std::endl;

        } catch (const std::runtime_error& e) {
            
            std::cerr << "Error: " << e.what() << std::endl;
            return 1; 
        }
    }
    return 0;
}