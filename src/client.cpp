/**
 * @file client.cpp
 * @brief Implementation of the Client class for TCP communication.
 * 
 * This file contains the implementation of the Client class methods, which
 * allows connecting to a server with a fixed IP and receiving data as a string.
 */

#include "../include/client/client.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>

const std::string Client::kServerIp_ = "127.0.0.1";

/**
 * @brief Constructs a Client object and connects to the server.
 * 
 * @param port The port number to connect to.
 */

Client::Client(int port) : server_port_(port){
    ConnectToServer();
}

/**
 * @brief Destroys the Client object and closes the connection.
 */
Client::~Client(){
    CloseConnection();
}

/**
 * @brief Establishes a connection to the server.
 * 
 * This function initializes the socket, defines the server address, and
 * establishes a connection to the server using the fixed IP and provided port.
 * 
 * @throw std::runtime_error If the socket creation, address assignment, or
 * connection fails.
 */
void Client::ConnectToServer(){
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ < 0){
        throw std::runtime_error("Failed to create socket"); //critical error, throw an execption
    }

    sockaddr_in server_adress;
    std::memset(&server_adress, 0, sizeof(server_adress)); //fill the server_adress with 0's 
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(server_port_);

    if(inet_pton(AF_INET, kServerIp_.c_str(), &server_adress.sin_addr)<=0){
        throw std::runtime_error("Invalid server IP adress");
    }

    if (connect(socket_fd_, reinterpret_cast<sockaddr*>(&server_adress), sizeof(server_adress)) < 0){
        throw std::runtime_error("Connection failed");
    }

    std::cout << "Succesfully connected to IP adress: " << kServerIp_ << std::endl;
}

/**
 * @brief Receives data from the server as a string.
 * 
 * @return A string containing the data received from the server.
 * 
 * @throw std::runtime_error If the data reception fails.
 */
void Client::CloseConnection() {
  if (socket_fd_ >= 0) {
    close(socket_fd_);
    std::cout << "Connection closed." << std::endl;
  }
}

/**
 * @brief Receives data from the server as a string.
 * 
 * This function reads data from the server into a buffer and converts it
 * into a string. The buffer size is fixed at 1024 bytes. If the reception
 * fails, an exception is thrown.
 * 
 * @return A string containing the data received from the server.
 * 
 * @throw std::runtime_error If the data reception fails.
 */
std::string Client::ReceiveData(){
    constexpr int kBufferSize = 1024;
    char buffer[kBufferSize];
    std::memset(buffer, 0, kBufferSize);

    size_t bytes_received = recv(socket_fd_, buffer, kBufferSize-1, 0);
    if(bytes_received < 0){
        throw std::runtime_error("Failed to receive data");
    }
    return std::string(buffer, bytes_received);
}