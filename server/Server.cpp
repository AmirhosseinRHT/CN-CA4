#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>

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
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int n;
    
    while (true) {
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, 
                     (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        
        string message(buffer);
        istringstream iss(message);
        string messageType, clientIP;
        
        iss >> messageType >> clientIP;
        
        if (messageType == "GREETING") {
            handleGreeting(clientIP, cliaddr);
        } else if (messageType == "MESSAGE") {
            string destIP, content;
            iss >> destIP;
            getline(iss, content);
            handleMessage(clientIP, destIP, content.substr(1)); // remove leading space
        }
    }
}

void Server::handleGreeting(const string& clientIP, const struct sockaddr_in& clientAddr) {
    clients[clientIP] = clientAddr;
    cout << "New client connected: " << clientIP << endl;
    string response = "GREETING_ACK";
    sendto(sockfd, response.c_str(), response.length(), MSG_CONFIRM, 
           (const struct sockaddr *)&clientAddr, sizeof(clientAddr));
}

void Server::handleMessage(const string& sourceIP, const string& destIP, const string& message) {
    cout << "Message from " << sourceIP << " to " << destIP << ": " << message << endl;
    
    if (clients.find(destIP) != clients.end()) {
        string fullMessage = "MESSAGE " + sourceIP + " " + message;
        sendto(sockfd, fullMessage.c_str(), fullMessage.length(), MSG_CONFIRM, 
               (const struct sockaddr *)&clients[destIP], sizeof(clients[destIP]));
    } else {
        cout << "Destination client not found: " << destIP << endl;
    }
}