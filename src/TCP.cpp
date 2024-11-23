#include "TCP.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread> // For background receiving loop

TCP::TCP() : sock(0), onDataReceived(nullptr), isReceiving(false) {
    memset(&addr, 0, sizeof(addr));
}

TCP::~TCP() {
    closeConnection();
}

bool TCP::sendData(const std::string& data) {
    if (send(sock, data.c_str(), data.size(), 0) < 0) {
        perror("Send failed");
        return false;
    }
    return true;
}

void TCP::closeConnection() {
    isReceiving = false; // Stop the receiving loop
    if (sock > 0) {
        close(sock);
        sock = 0;
    }
}

bool TCP::bindToPort(int port) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return false;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(sock);
        return false;
    }

    return true;
}

bool TCP::listenForClients(int backlog) {
    if (listen(sock, backlog) < 0) {
        perror("Listen failed");
        return false;
    }
    return true;
}

TCP* TCP::acceptClient() {
    socklen_t addrlen = sizeof(addr);
    int client_sock = accept(sock, (struct sockaddr *)&addr, &addrlen);
    if (client_sock < 0) {
        perror("Accept failed");
        return nullptr;
    }

    TCP* client = new TCP();
    client->sock = client_sock; // Set the accepted socket to the new TCP instance
    client->addr = addr;
    return client;
}

bool TCP::connectToServer(const std::string& ip, int port) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return false;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        return false;
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Connection failed");
        return false;
    }

    return true;
}

void TCP::callBack(const std::function<void(const std::string&)>& callback) {
    onDataReceived = callback;
}

void TCP::startReceiving() {
    isReceiving = true;

    std::thread([this]() {
        while (isReceiving) {
            char buffer[1024] = {0};
            int bytes_read = read(sock, buffer, sizeof(buffer));
            if (bytes_read > 0) {
                std::string data(buffer, bytes_read);

                // Automatically invoke the callback if it's set
                if (onDataReceived) {
                    onDataReceived(data);
                }
            } else if (bytes_read == 0) {
                // Connection closed by the client
                isReceiving = false;
            } else {
                // Error occurred
                perror("Receive error");
                isReceiving = false;
            }
        }
    }).detach(); // Run the receiving loop in a separate thread
}