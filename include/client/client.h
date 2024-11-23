#ifndef CLIENT_H
#define CLIENT_H

#include <string>

/**
 * @brief A simple TCP client class for receiving data as a string.
 * 
 * This class connects to a server using a fixed IP address. The port is
 * provided as a parameter during initialization. The client allows receiving
 * data from the server as a string.
 */

class Client {
public:
    /**
     * @brief Constructs a Client object.
     * 
     * @param port The server's port number.
     */
    Client(int port);

    /**
     * @brief Destroys the Client object and cleans up resources.
     */
    ~Client();

    /**
     * @brief Receives data from the server.
     * 
     * @return A string containing the received data.
     */
    std::string ReceiveData();

private:   
    int socket_fd_; /**< File descriptor for the socket. */
    int server_port_; /**< The server's port number. */
    static const std::string kServerIp; /**< The fixed IP address of the server. */
    
    /**
     * @brief Establishes a connection to the server.
     * 
     * This function initializes the socket and connects to the server
     * using the specified IP and port.
     */
    void ConnectToServer();

    /**
     * @brief Closes the connection to the server.
     * 
     * Ensures that the socket is closed to prevent resource leaks.
     */
    void CloseConnection();
};

#endif // CLIENT_H