// RawServer.cpp
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
    int server_fd;
    struct sockaddr_in address;
    CustomPacket packet;
    
    // Creating raw socket
    if ((server_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    std::cout << "Server listening on port " << PORT << std::endl;
    
    while(true) {
        socklen_t addrlen = sizeof(address);
        
        // Receive SYN
        recvfrom(server_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&address, &addrlen);
        if (packet.flags & SYN_FLAG) {
            std::cout << "Received SYN" << std::endl;
            
            // Send SYN-ACK
            packet.flags = SYN_FLAG | ACK_FLAG;
            packet.ack_num = packet.seq_num + 1;
            sendto(server_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&address, addrlen);
            std::cout << "Sent SYN-ACK" << std::endl;
            
            // Receive ACK
            recvfrom(server_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&address, &addrlen);
            if (packet.flags & ACK_FLAG) {
                std::cout << "Received ACK, connection established" << std::endl;
                
                // Enter message exchange loop
                while(true) {
                    recvfrom(server_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&address, &addrlen);
                    if (packet.flags & FIN_FLAG) {
                        std::cout << "Received FIN, closing connection" << std::endl;
                        break;
                    }
                    std::cout << "Client: " << packet.data << std::endl;
                    
                    std::string message;
                    std::cout << "Server: ";
                    std::getline(std::cin, message);
                    strncpy(packet.data, message.c_str(), BUFFER_SIZE);
                    packet.flags = ACK_FLAG;
                    sendto(server_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&address, addrlen);
                }
            }
        }
    }
    
    close(server_fd);
    return 0;
}