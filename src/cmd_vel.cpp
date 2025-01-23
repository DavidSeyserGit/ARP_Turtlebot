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


void LinearController(const OdomData* odomData, double& linear_velocity, double& angular_velocity) {
    // Target position and orientation
    constexpr float target_x = 4.0; // Replace with your desired target x
    constexpr float target_y = 0.0; // Replace with your desired target y
    constexpr float target_theta = 0.0; // Replace with your desired target orientation

    // Calculate errors
    float delta_x = target_x - odomData->x;
    float delta_y = target_y - odomData->y;
    float delta_theta = target_theta - odomData->theta;

    // Calculate control parameters
    float rho = std::sqrt(delta_x * delta_x + delta_y * delta_y);
    float alpha = -odomData->theta + std::atan2(delta_y, delta_x);
    float beta = delta_theta - alpha;

    // Normalize angles to [-PI, PI]
    while (alpha > M_PI) alpha -= 2 * M_PI;
    while (alpha < -M_PI) alpha += 2 * M_PI;

    while (beta > M_PI) beta -= 2 * M_PI;
    while (beta < -M_PI) beta += 2 * M_PI;

    // Control gains
    constexpr float k_rho = 0.7;  // Gain for linear velocity
    constexpr float k_alpha = 1; // Gain for alpha (heading correction)
    constexpr float k_beta = -0.5; // Gain for beta (orientation correction)

    // Compute velocities
    linear_velocity = k_rho * rho;
    angular_velocity = k_alpha * alpha + k_beta * beta;

    // Stop moving if close enough to the target
    if (rho < 0.25) {
        linear_velocity = 0.0;
        angular_velocity = 0.0;
    }
    std::cout << linear_velocity << " " << rho << " " << std::endl;
}

void SendCmdVel(int port) {
    const std::string kServerIp_ = "192.168.100.55";
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

    double linear_velocity = 0;
    double angular_velocity = 0;
    char buffer[128];
    while (true) {
        LinearController(odomData, linear_velocity, angular_velocity);
        
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