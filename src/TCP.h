#ifndef TCP_H
#define TCP_H

#include <string>
#include <functional> // For std::function
#include <netinet/in.h>

class TCP {
public:
    TCP();                                      // Constructor
    ~TCP();                                     // Destructor

    // general
    bool sendData(const std::string& data);     // Send data
    void closeConnection();                     // Close the socket

    // Server-specific
    bool bindToPort(int port);                  // Bind socket to port
    bool listenForClients(int backlog);         // Start listening for connections
    TCP* acceptClient();                        // Accept a client connection

    // Client-specific
    bool connectToServer(const std::string& ip, int port); // Connect to server

    // Callback
    void callBack(const std::function<void(const std::string&)>& callback);

    // Event loop for receiving data and invoking the callback
    void startReceiving();

private:
    int sock;                                   // Socket file descriptor
    struct sockaddr_in addr;                    // Address structure
    std::function<void(const std::string&)> onDataReceived; // Callback for received data
    bool isReceiving;                           // Flag to control receiving loop
};

#endif // TCP_H