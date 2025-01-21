#include "../include/client/client.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h> // Make sure to include errno

const std::string Client::kServerIp_ = "192.168.100.51";
// const std::string Client::kServerIp_ = "192.168.0.202";

Client::Client(int port) : server_port_(port), socket_fd_(-1) { // Initialize socket_fd_
    ConnectToServer();
}

Client::~Client() {
    CloseConnection();
}

void Client::ConnectToServer() {
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ < 0) {
        throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
    }

    sockaddr_in server_adress{};
    std::memset(&server_adress, 0, sizeof(server_adress));
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(server_port_);

    if (inet_pton(AF_INET, kServerIp_.c_str(), &server_adress.sin_addr) <= 0) {
        throw std::runtime_error("Invalid server IP address");
    }

    if (connect(socket_fd_, reinterpret_cast<sockaddr*>(&server_adress), sizeof(server_adress)) < 0) {
        throw std::runtime_error("Can't connect to server: " + std::string(strerror(errno)));
    }

    std::cout << "Successfully connected to IP address: " << kServerIp_ << std::endl;
}

void Client::CloseConnection() {
    if (socket_fd_ >= 0) {
        close(socket_fd_);
        socket_fd_ = -1; // Reset socket_fd_
        std::cout << "Connection closed." << std::endl;
    }
}

std::string Client::ReceiveData() {
    constexpr int kBufferSize = 1024000;
    char buffer[kBufferSize];
    std::memset(buffer, 0, kBufferSize);

    ssize_t bytes_received = recv(socket_fd_, buffer, kBufferSize - 1, 0);

    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // No data available yet (non-blocking socket)
            return ""; // Return empty string
        } else if (errno == ECONNRESET || errno == ECONNREFUSED || errno == EPIPE) {
            // Connection closed by server or other network error
            CloseConnection(); // Important: Close the socket
            throw std::runtime_error("Connection error: " + std::string(strerror(errno)));
        } else {
            throw std::runtime_error("recv failed: " + std::string(strerror(errno)));
        }
    } else if (bytes_received == 0) {
        // Connection gracefully closed by the server
        CloseConnection();
        return ""; // Return empty string to indicate disconnection
    }

    return std::string(buffer, bytes_received);
}