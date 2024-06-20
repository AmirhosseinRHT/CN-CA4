#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create a UDP socket
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Bind the socket to all available interfaces
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8000);  // Set the desired port number

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(server_socket);
        return 1;
    }

    // Receive data
    std::cout << "Server is listening for incoming messages..." << std::endl;
    char buffer[1024];
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (true) {
        ssize_t bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr, &client_addr_len);
        if (bytes_received == -1) {
            std::cerr << "Failed to receive data" << std::endl;
            break;
        }

        std::string message(buffer, bytes_received);
        std::string client_ip = inet_ntoa(client_addr.sin_addr);
        std::cout << "Received message from " << client_ip << ": " << message << std::endl;
    }

    // Close the socket
    close(server_socket);
    return 0;
}