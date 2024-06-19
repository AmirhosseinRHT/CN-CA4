#ifndef SENDER_HPP
#define SENDER_HPP

#include <string>
#include <arpa/inet.h>
#include "Client.hpp"

class Sender : Client {
public:
    Sender(const std::string& serverIp, const std::string& clientIP, int port );
    void start();
    void startSendingTask();

private:
};

#endif // SENDER_HPP