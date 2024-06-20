#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "defs.hpp"
#define BUFFER_SIZE 1024

class Server {
public:
    Server(int port) {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to bind socket" << std::endl;
            return;
        }

        std::cout << "Server started on port " << port << std::endl;
    }

    void run() {
        char *buffer;
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        while (true) {
            Packet packet;
            int recv_len = recvfrom(sockfd,(void *) &packet, sizeof(Packet), 0, (struct sockaddr*)&client_addr, &client_len);
            if (recv_len < 0) {
                std::cerr << "Failed to receive data" << std::endl;
                continue;
            }
            if( (packet.syn & (!packet.ack))){
                printl("SYN recived");
                handle_handshake(&packet ,&client_addr ,client_len);
            }else{
            
                // buffer = (char *) &packet;
                // buffer[recv_len] = '\0';
                // std::cout << "Received message: " << buffer << std::endl;

                // std::string ack = "ACK";
                // sendto(sockfd, ack.c_str(), ack.length(), 0, (struct sockaddr*)&client_addr, client_len);
                // std::cout << "Sent ACK to client" << std::endl;
            }
        }
    }

    void handle_handshake(Packet *packet ,sockaddr_in *client_addr ,socklen_t client_len){
        int ack_seq = packet->syn_seq +1;
        Packet *syn_ack_packet =make_syn_ack_packet(10 , ack_seq);
        Packet ack_packet;
        sendto(sockfd,(void *)syn_ack_packet, sizeof(Packet), 0, (struct sockaddr*)client_addr, client_len);
        int recv_len = recvfrom(sockfd,(void *) &ack_packet, sizeof(Packet), 0, (struct sockaddr*)client_addr, &client_len);
        if(ack_packet.ack & (!ack_packet.syn)){
            printl("ACK recived");
        }
    }

    

private:
    int sockfd;
    sockaddr_in server_addr;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    Server server(port);
    server.run();

    return 0;
}