#include "senderGBN.hpp"

SenderGoBackN::SenderGoBackN(int _seqNum , int _totalFrames)
{
    seqNum = _seqNum;
    totalFrames = _totalFrames;
}

std::string SenderGoBackN::start()
{

}

std::vector<std::string> SenderGoBackN::getToBeSentFrames(int x)
{

}


void SenderGoBackN::handleAckMessage(std::string message)
{

}