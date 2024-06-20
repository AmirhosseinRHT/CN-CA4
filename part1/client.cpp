#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Target IP and port
    std::string target_ip = "127.0.0.1";
    int target_port = 8000;  // Same port as the server

    // Create a UDP socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Send data


    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(target_ip.c_str());
    server_addr.sin_port = htons(target_port);

    while(true){    
        std::string message;
        std::cout << "Enter a message to send: ";
        std::getline(std::cin, message);

        ssize_t bytes_sent = sendto(client_socket, message.c_str(), message.length(), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        if (bytes_sent == -1) {
            std::cerr << "Failed to send data" << std::endl;
        } else {
            std::cout << "Message sent successfully" << std::endl;
        }
    }

    // Close the socket
    close(client_socket);
    return 0;
}