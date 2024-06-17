// RawClient.cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

struct CustomPacket {
    uint32_t seq_num;
    uint32_t ack_num;
    uint16_t flags;
    char data[BUFFER_SIZE];
};

#define SYN_FLAG 0x1
#define ACK_FLAG 0x2
#define FIN_FLAG 0x4

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    CustomPacket packet;
    
    // Creating raw socket
    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("Socket creation error");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }
    
    // Send SYN
    packet.seq_num = 1000;
    packet.flags = SYN_FLAG;
    sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    std::cout << "Sent SYN" << std::endl;
    
    // Receive SYN-ACK
    socklen_t addrlen = sizeof(serv_addr);
    recvfrom(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, &addrlen);
    if (packet.flags & (SYN_FLAG | ACK_FLAG)) {
        std::cout << "Received SYN-ACK" << std::endl;
        
        // Send ACK
        packet.flags = ACK_FLAG;
        packet.seq_num++;
        sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        std::cout << "Sent ACK, connection established" << std::endl;
        
        // Enter message exchange loop
        while(true) {
            std::string message;
            std::cout << "Client: ";
            std::getline(std::cin, message);
            
            if (message == "exit") {
                packet.flags = FIN_FLAG;
                sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
                std::cout << "Sent FIN, closing connection" << std::endl;
                break;
            }
            
            strncpy(packet.data, message.c_str(), BUFFER_SIZE);
            packet.flags = 0;
            sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            
            recvfrom(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, &addrlen);
            std::cout << "Server: " << packet.data << std::endl;
        }
    }
    
    close(sock);
    return 0;
}