#include "Client.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 1024

Client::Client(const string _serverIp, int _port)
{
    serverIp = _serverIp;
    port = _port;
}

void Client::start() {
    setupSocket();
    communicateWithServer();
}

void Client::setupSocket() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
}

void Client::communicateWithServer() {
    char buffer[BUFFER_SIZE];
    int n;
    socklen_t len;
    
    while (true) {
        string message;
        cout << "Enter message: ";
        getline(cin, message);
        
        sendto(sockfd, message.c_str(), message.length(), MSG_CONFIRM, 
               (const struct sockaddr *)&servaddr, sizeof(servaddr));
        
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, 
                     (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        cout << "Server : " << buffer << endl;
    }
}