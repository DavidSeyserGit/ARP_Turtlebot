#ifndef SEND_CMD_VEL_H_
#define SEND_CMD_VEL_H_

#include <iostream>
#include <format>
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
    const std::string kServerIp_ = "192.168.100.51";

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
    std::memset(buffer, 0, strlen(buffer));

    // Example velocities
    double linear_velocity = 0.0;
    double angular_velocity = 0.0;

    // Send data in a loop
while (true) {
    // Format the message
    int message_length = std::snprintf(buffer, sizeof(buffer),
        "---START---{\"linear\": %.2f, \"angular\": %.2f}___END___",
        linear_velocity, angular_velocity);

    // Check if formatting was successful
    if (message_length < 0 || message_length >= static_cast<int>(sizeof(buffer))) {
        std::cerr << "Error: Failed to format the message" << std::endl;
        break;
    }

    // Send the formatted message
    ssize_t bytes_sent = send(socket_fd, buffer, message_length, 0);

    // Handle errors in send
    if (bytes_sent < 0) {
        perror("Send failed");
        break; // Exit loop on error
    }

    // Print the message and bytes sent
    std::cout << "Bytes sent: " << bytes_sent << std::endl;
    std::cout << "Sent: " << std::string(buffer, message_length) << std::endl;
    linear_velocity +=0.1;
    angular_velocity +=0.1; 

    if (angular_velocity > 0.3)
    {
        break;
    }
    
    // Pause for 1 second before sending the next message
    std::this_thread::sleep_for(std::chrono::seconds(2));
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