#ifndef SEND_CMD_VEL_H_
#define SEND_CMD_VEL_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

struct OdomData{
    float x;
    float y;
    float theta;
};


bool LinearController(double* linear_velocity, double* angular_velocity, OdomData* odom){

    return 0;
}

void SendCmdVel(int port) {
    const std::string kServerIp_ = "127.0.0.1";
    const std::string shm_name = "/odom_data";  // Shared memory name

    //open shared memory
    int shm_fd = shm_open(shm_name.c_str(), O_RDONLY, 0666);
    if (shm_fd < 0) {
        throw std::runtime_error("Failed to open shared memory");
    }      

    // Map shared memory
    void* shm_ptr = mmap(0, sizeof(OdomData), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        close(shm_fd);
        throw std::runtime_error("Failed to map shared memory");
    }

    OdomData* odomData = static_cast<OdomData*>(shm_ptr); //here i get the shared memory data

    // Create a socket
    int socket_fd = 
        socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Configure the server address
    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    
    if (inet_pton(AF_INET, kServerIp_.c_str(), &server_address.sin_addr) <= 0) {
        close(socket_fd);
        throw std::runtime_error("Invalid server IP address");
    }

    // Connect to the server
    if (connect(socket_fd, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        close(socket_fd);
        perror("Connection error");
        throw std::runtime_error("Connection failed");
    }

    // Message buffer
    char buffer[70];
    std::memset(buffer, 0, strlen(buffer));
    
    //init
    double linear_velocity = 0.0;
    double angular_velocity = 0.0;

    while (true) {
        LinearController(&linear_velocity, &angular_velocity, odomData); // linear controller should overrite the linear_velocity and angular velocity values. 
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
        //std::cout << "Bytes sent: " << bytes_sent << std::endl;
        //std::cout << "Sent: " << std::string(buffer, message_length) << std::endl;
        linear_velocity +=0.1;
        angular_velocity +=0.1; 

        if (angular_velocity > 0.3)
        {
            linear_velocity = 0;
            angular_velocity = 0; 

        }
        
        // Pause for 1 second before sending the next message
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    close(socket_fd);
}

#endif  // SEND_CMD_VEL_H_