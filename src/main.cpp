#include "../include/client/client.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Client client(9997);
        std::string received_data = client.ReceiveData();

        std::cout << "data rec: " << received_data << std::endl;

    } catch (const std::runtime_error& e) {
        
        std::cerr << "Error: " << e.what() << std::endl;
        return 1; 
    }

    return 0;
}