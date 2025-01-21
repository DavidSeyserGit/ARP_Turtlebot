#include "odom.h"
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

void SendCmdVel(int port) {
    const std::string kServerIp_ = "192.168.100.51";
    const std::string shm_name = "/odom_data";

    // Open shared memory
    int shm_fd = shm_open(shm_name.c_str(), O_RDONLY, 0666);
    if (shm_fd < 0) {
        perror("shm_open failed");
        throw std::runtime_error("Failed to open shared memory");
    }

    // Map shared memory
    void* shm_ptr = mmap(nullptr, sizeof(OdomData), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        throw std::runtime_error("Failed to map shared memory");
    }
    OdomData* odomData = static_cast<OdomData*>(shm_ptr);

    // Create socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket failed");
        munmap(odomData, sizeof(OdomData));
        close(shm_fd);
        throw std::runtime_error("Failed to create socket");
    }

    // Configure server address
    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, kServerIp_.c_str(), &server_address.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(socket_fd);
        munmap(odomData, sizeof(OdomData));
        close(shm_fd);
        throw std::runtime_error("Invalid server IP address");
    }

    // Connect to server
    if (connect(socket_fd, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        perror("connect failed");
        close(socket_fd);
        munmap(odomData, sizeof(OdomData));
        close(shm_fd);
        throw std::runtime_error("Connection failed");
    }

    // Command sending loop
    char buffer[128];
    int count_time=0;
    int sign = 1;
    while (true) {
        // Read data from shared memory
        float x = odomData->x;
        float y = odomData->y;

        if (count_time>10)
        {
            count_time=0;
            sign=sign*-1;
        }
        
        // Calculate velocities
        double linear_velocity = sign*0.1;
        double angular_velocity = 0.5;

        // Format message
        int message_length = std::snprintf(buffer, sizeof(buffer),
            "---START---{\"linear\": %.2f, \"angular\": %.2f}___END___",
            linear_velocity, angular_velocity);
        if (message_length < 0 || message_length >= static_cast<int>(sizeof(buffer))) {
            std::cerr << "Error: Failed to format the message" << std::endl;
            break;
        }

        // Send message
        ssize_t bytes_sent = send(socket_fd, buffer, message_length, 0);
        if (bytes_sent < 0) {
            perror("Send failed");
            break;
        }

        std::cout << "Sent command: " << std::string(buffer, message_length) << std::endl;

        // Sleep before sending the next command
        std::this_thread::sleep_for(std::chrono::seconds(1));
        count_time++;
    }

    // Cleanup
    close(socket_fd);
    if (munmap(odomData, sizeof(OdomData)) == -1) {
        perror("munmap failed");
    }
    if (close(shm_fd) == -1) {
        perror("close shm_fd failed");
    }
    
    
}

int main(){
    SendCmdVel(9999);
}