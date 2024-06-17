#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <arpa/inet.h>

class Server {
public:
    Server(int port);
    void start();

private:
    int sockfd;
    struct sockaddr_in servaddr;
    std::map<std::string, struct sockaddr_in> clients;
    int port;

    void setupSocket();
    void bindSocket();
    void receiveAndRespond();
    void handleGreeting(const std::string& clientIP, const struct sockaddr_in& clientAddr);
    void handleMessage(const std::string& sourceIP, const std::string& destIP, const std::string& message);
    void sendMessageTo(std::string destination , std::string msg);
    std::string recieveMessage(struct sockaddr_in * clientAddr);
};

#endif // SERVER_HPP