#include <iostream>
#include "TCP.h"

#define PORT 8080

void callBack(const std::string& data) {
    std::cout << data << std::endl;
}

int main() {
    TCP server;

    // Bind to port and start listening
    if (!server.bindToPort(PORT)) {
        std::cerr << "Failed to bind to port." << std::endl;
        return 1;
    }

    if (!server.listenForClients(3)) {
        std::cerr << "Failed to listen for clients." << std::endl;
        return 1;
    }

    while (true) {
        // Accept a new client connection
        TCP* client = server.acceptClient();
        if (!client) {
            std::cerr << "Failed to accept client." << std::endl;
            continue; // Keep listening for other clients
        }

        // Set a callback for handling received data
        client->callBack(callBack);
        client->startReceiving();
    }

    return 0;
}