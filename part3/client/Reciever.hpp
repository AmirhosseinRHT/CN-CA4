#ifndef RECIEVER_HPP
#define RECIEVER_HPP

#include <string>
#include <arpa/inet.h>
#include "Client.hpp"

class Reciever : public Client{
public:
    Reciever(const std::string& serverIp, const std::string& clientIP, int port);
    void start();
    void handleRecievingTask();

private:
};

#endif // RECIEVER_HPP