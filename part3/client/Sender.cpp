#include "Client.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include "../defines/defs.hpp"
#include "Sender.hpp"

using namespace std;

#define BUFFER_SIZE 1024

Sender::Sender(const string& _serverIp, const string& _clientIP, int _port)
: Client(_serverIp , _clientIP  ,_port) {}

void Sender::start() 
{
    setupSocket();
    sendGreeting();
    startSendingTask();
}

void Sender::startSendingTask() 
{
    for(int i = 0 ; i < 100 ; i++)
    {
        sendMessage("MESSAGE " + clientIP + " " + "192.168.1.3 " + to_string(i));
        sleep(1);
    }
}