#ifndef SEND_CMD_VEL_H_
#define SEND_CMD_VEL_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

void SendCmdVel(int port) {
    const std::string kServerIp_ = "127.0.0.1";

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Failed to initialize Winsock");
    }
    #endif

    // Create a socket
    int socket_fd = 
    #ifdef _WIN32
        socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    #else
        socket(AF_INET, SOCK_STREAM, 0);
    #endif

    if (socket_fd < 0) {
    #ifdef _WIN32
        throw std::runtime_error("Failed to create socket: " + std::to_string(WSAGetLastError()));
    #else
        throw std::runtime_error("Failed to create socket");
    #endif
    }

    // Configure the server address
    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    
    if (inet_pton(AF_INET, kServerIp_.c_str(), &server_address.sin_addr) <= 0) {
    #ifdef _WIN32
        closesocket(socket_fd);
        WSACleanup();
    #else
        close(socket_fd);
    #endif

        throw std::runtime_error("Invalid server IP address");
    }

    // Connect to the server
    if (connect(socket_fd, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) {
    #ifdef _WIN32
        closesocket(socket_fd);
        WSACleanup();
        throw std::runtime_error("Connection failed");
    #else
        close(socket_fd);
        perror("Connection error");
        throw std::runtime_error("Connection failed");
    #endif
    }

    // Message buffer
    char buffer[70];
    std::memset(buffer, 0, sizeof(buffer));

    // Example velocities
    double linear_velocity = 0.0;
    double angular_velocity = 0.0;

    // Send data in a loop
    while (true) {
        int message_length = snprintf(buffer, sizeof(buffer),
                                      "---START---{\"linear\": %.2f, \"angular\": %.2f}___END___",
                                      linear_velocity, angular_velocity);
        if (message_length < 0 || message_length >= static_cast<int>(sizeof(buffer))) {
            throw std::runtime_error("Failed to format message");
        }

        ssize_t bytes_sent = send(socket_fd, buffer, message_length, 0);
    
        //error handeling when no data is sent must be added later -> non fatal error and we should try again
        if (bytes_sent < 0) {
        #ifdef _WIN32
            std::cerr << "Failed to send data: " << WSAGetLastError() << std::endl;
        #else
            perror("Failed to send data");
        #endif
        } else {
            std::cout << "Sent: " << std::string(buffer, message_length) << std::endl;
        }

        // Pause for 1 second before sending the next message
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Close the socket
#ifdef _WIN32
    closesocket(socket_fd);
    WSACleanup();
#else
    close(socket_fd);
#endif
}

#endif  // SEND_CMD_VEL_H_