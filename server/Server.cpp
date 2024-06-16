#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 1024

Server::Server(int port) : port(port) {}

void Server::start() {
    setupSocket();
    bindSocket();
    cout << "Server listening on port " << port << endl;
    receiveAndRespond();
}

void Server::setupSocket() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
}

void Server::bindSocket() {
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Server::receiveAndRespond() {
    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(cliaddr);
    int n;
    
    while (true) {
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, 
                     (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        cout << "Client : " << buffer << endl;
        sendto(sockfd, (const char *)"Message received", strlen("Message received"), 
               MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }
}