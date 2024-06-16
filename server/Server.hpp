#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <arpa/inet.h>

class Server {
public:
    Server(int port);
    void start();

private:
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int port;

    void setupSocket();
    void bindSocket();
    void receiveAndRespond();
};

#endif // SERVER_HPP