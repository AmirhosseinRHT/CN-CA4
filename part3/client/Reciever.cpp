#include "Reciever.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include "../defines/defs.hpp"
#include "Reciever.hpp"

using namespace std;

#define BUFFER_SIZE 1024

Reciever::Reciever(const string& _serverIp, const string& _clientIP, int _port)
: Client(_serverIp , _clientIP  ,_port) {}

void Reciever::start() {
    setupSocket();
    sendGreeting();
    handleRecievingTask();
}


void Reciever::handleRecievingTask() 
{
    while (true) 
    {
        string recievedMessage = recieveMessage();
        cout << "Received: " << recievedMessage << endl;
    }
}