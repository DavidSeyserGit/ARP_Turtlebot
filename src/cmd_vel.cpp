#ifndef SEND_CMD_VEL_H_
#define SEND_CMD_VEL_H_

#include <arpa/inet.h>
#include <atomic>
#include <chrono>  
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

// Sends a velocity command as a formatted string over TCP to the specified port.
void SendCmdVel(int port) {
    const std::string kServerIp_ = "127.0.0.1";
    /*
        start the connection to the robot on the IP with Socket 9999 
    */
    int socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd_ < 0){
        throw std::runtime_error("Failed to create socket");
    };

    sockaddr_in server_adress;
    std::memset(&server_adress, 0, sizeof(server_adress));
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(9999); //fixed port for sending data to the robot
    
    if (inet_pton(AF_INET, kServerIp_.c_str(), &server_adress.sin_addr)<0){
        throw std::runtime_error("Invalid server IP adress");
    }

    if (connect(socket_fd_, reinterpret_cast<sockaddr*>(&server_adress), sizeof(server_adress)) < 0){
        throw std::runtime_error("Connection failed");
    }
    /*
    send data

    message is of this type:
    "---START---{\"linear\": 0.0, \"angular\": 0}___END___";
    so a messagebuffer can be used to construct this message

    for this snprintf can be used
        -> only stack allocated
        -> avoiding heap allocation

    ---START---             -> 11 characters
    {\"linear\":            -> 12 characters                  
    <linear_velocity>       -> when used as a double: 12 characters
    ,\"angular\":           -> 13 characters
    <angular_velocity>      -> when used as a double: 12 characters
    }___END___              -> 10 characters
    
    buffer needs to be at least 70 characters long
    */
    char buffer[70];
    memset(buffer, 0, 70); //fill the buffer with 0

    double linear_velocity = 0;
    double angular_velocity = 0;
    while(1){
        //only for testing
        int message_length = snprintf(buffer, sizeof(buffer),
                "---START---{\"linear\": %.2f, \"angular\": %.2f}___END___",
                linear_velocity, angular_velocity);                             
        std::string message(buffer, message_length);
        size_t bytes_sent = send(socket_fd_, buffer, sizeof(buffer), 0);
        std::cout << message << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

#endif  // SEND_CMD_VEL_H_