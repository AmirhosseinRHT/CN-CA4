#include "Client.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>

using namespace std;

#define BUFFER_SIZE 1024

Client::Client(const string& _serverIp, const string& _clientIP, int _port)
    : serverIp(_serverIp), clientIP(_clientIP), port(_port) {}

void Client::start() {
    setupSocket();
    sendGreeting();
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

void Client::sendGreeting() {
    string greeting = "GREETING " + clientIP;
    sendto(sockfd, greeting.c_str(), greeting.length(), MSG_CONFIRM, 
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
    
    char buffer[BUFFER_SIZE];
    socklen_t len;
    int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, 
                     (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    cout << "Server response: " << buffer << endl;
}

void Client::communicateWithServer() {
    char buffer[BUFFER_SIZE];
    int n;
    socklen_t len;
    
    while (true) {
        string input;
        cout << "Enter message (FORMAT: DEST_IP MESSAGE) : ";
        getline(cin, input);
        
        if (input == "quit") break;
        
        istringstream iss(input);
        string destIP, message;
        iss >> destIP;
        getline(iss, message);
        
        string fullMessage = "MESSAGE " + clientIP + " " + destIP + message;
        sendto(sockfd, fullMessage.c_str(), fullMessage.length(), MSG_CONFIRM, 
               (const struct sockaddr *)&servaddr, sizeof(servaddr));
        
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, 
                     (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        cout << "Received: " << buffer << endl;
    }
}