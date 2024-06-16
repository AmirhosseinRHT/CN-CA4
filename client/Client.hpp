#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <arpa/inet.h>

class Client {
public:
    Client(const std::string _serverIp, int _port);
    void start();

private:
    int sockfd;
    struct sockaddr_in servaddr;
    std::string serverIp;
    int port;

    void setupSocket();
    void communicateWithServer();
};

#endif // CLIENT_HPP