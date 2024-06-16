#include "Client.hpp"
#include <iostream>

int main(int argc , char * argv[]) {
    Client client("127.0.0.1", "192.168.1." + std::string(argv[1]),  8080);
    std::cout << argv[1] << std::endl;
    client.start();
    return 0;
}