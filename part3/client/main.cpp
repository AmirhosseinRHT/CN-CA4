#include "Client.hpp"
#include <iostream>

int main(int argc , char * argv[]) {
    if(argc != 3)
    {
        std::cout << "Wrong arguments !!" << std::endl;
        return 0;
    }
    Client client("127.0.0.1", "192.168.1." + std::string(argv[1]), 8080 , int(argv[2][0]) - 48);
    client.start();
    return 0;
}