#include <iostream>
#include <memory> // For std::shared_ptr
#include <thread>
#include <chrono>
#include "TCP.h"

#define PORT 9999

int main() {
    auto client = std::make_shared<TCP>(); // Create a shared TCP instance

    // Connect to server
    if (!client->connectToServer("192.168.100.51", PORT)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    std::string message = "---START---{\"linear\": 0.0, \"angular\": 0}___END___";
    if (!client->sendData(message)) {
        std::cerr << "Failed to send data. Exiting." << std::endl;
        return 1;
    }
    std::cout << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); //sleep

    client->closeConnection();
    return 0;
}
