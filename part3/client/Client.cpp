#include "Client.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include "../defs.hpp"

using namespace std;

#define BUFFER_SIZE 1024

Client::Client(const string& _serverIp, const string& _clientIP, int _port , int type)
    : serverIp(_serverIp), clientIP(_clientIP), port(_port) ,clientType(type) {}

void Client::start() {
    setupSocket();
    sendGreeting();
    communicateWithServer();
}

void Client::sendMessage(string msg)
{
    sendto(sockfd, msg.c_str(), msg.length(), MSG_CONFIRM, 
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
}

string Client::recieveMessage()
{
    char buffer[BUFFER_SIZE];
    socklen_t len;
    int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    return string(buffer);
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
    sendMessage(greeting);
    string answer = recieveMessage();
    cout << "Server response: " << answer << endl;
}

void Client::communicateWithServer() {
    while (true) 
    {
        if(clientType == SENDER)
            for(int i = 0 ; i < 100 ; i++)
            {
                sendMessage("MESSAGE " + clientIP + " " + "192.168.1.3 " + to_string(i));
                sleep(1);
            }
        else
        {
            string recievedMessage = recieveMessage();
            cout << "Received: " << recievedMessage << endl;
        }
        // string input;
        // cout << "Enter message (FORMAT: DEST_IP MESSAGE) : ";
        // getline(cin, input);
        // vector<string> splitted = splitString(input , ' ');
        // if(splitted.size() == 2)
        // {
        //     string fullMessage = "MESSAGE " + clientIP + " " + splitted[0] + splitted[1];
        //     sendMessage(fullMessage);
        //     string recievedMessage = recieveMessage();
        //     cout << "Received: " << recievedMessage << endl;
        // }
    }
}