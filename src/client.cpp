#include <iostream>
#include <memory> // For std::shared_ptr
#include <thread>
#include <chrono>
#include "TCP.h"

#define PORT 9997

int main() {
    auto client = std::make_shared<TCP>(); // Create a shared TCP instance

    // Connect to server
    if (!client->connectToServer("127.0.0.1", PORT)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

        std::string message = "test " + std::to_string(time);
        if (!client->sendData(message)) {
            std::cerr << "Failed to send data. Exiting." << std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2)); //sleep
    }

    client->closeConnection();
    return 0;
}
