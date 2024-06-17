#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <arpa/inet.h>

class Client {
public:
    Client(const std::string& serverIp, const std::string& clientIP, int port);
    void start();

private:
    int sockfd;
    struct sockaddr_in servaddr;
    std::string serverIp;
    std::string clientIP;
    int port;

    void setupSocket();
    void sendGreeting();
    void communicateWithServer();
};

#endif // CLIENT_HPP