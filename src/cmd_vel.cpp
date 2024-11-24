#ifndef SEND_CMD_VEL_H_
#define SEND_CMD_VEL_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

// Sends a velocity command as a formatted string over TCP to the specified port.
void SendCmdVel(int port) {
    const std::string kIpAddress = "192.168.100.53";
    /*
    message is of this type:
    "---START---{\"linear\": 0.0, \"angular\": 0}___END___";
    so a messagebuffer can be used to construct this message

    for this snprintf can be used
        -> only stack allocated
        -> avoiding heap allocation
    */
    /*
        ---START---             -> 11 characters
        {\"linear\":            -> 12 characters                  
        <linear_velocity>       -> when used as a double: 12 characters
        ,\"angular\":           -> 13 characters
        <angular_velocity>      -> when used as a double: 12 characters
        }___END___              -> 10 characters
    
    buffer needs to have at least 70 characters long
    */
    char buffer[70];
    memset(buffer, 0, 70);
    
    //only for testing
    double linear_velocity = 0;
    double angular_velocity = 0;
    
    while(1){
        int message_length = snprintf(buffer, sizeof(buffer),
                                    "---START---{\"linear\": %.2f, \"angular\": %.2f}___END___",
                                    linear_velocity, angular_velocity);
        //needs error handling when message is longer then buffer size or if it is smaller then 0
        linear_velocity += 1; angular_velocity += 1;
        std::string message(buffer, message_length);
        std::cout << message << std::endl;

    }

}

#endif  // SEND_CMD_VEL_H_