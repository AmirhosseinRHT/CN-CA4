#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

class UDPClient {
public:
    UDPClient(const std::string& serverIP, int serverPort) {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(serverIP.c_str());
        server_addr.sin_port = htons(serverPort);
    }

    void sendMessage(const std::string& message) {
        sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        std::cout << "Sent message: " << message << std::endl;

        char buffer[BUFFER_SIZE];
        sockaddr_in recv_addr;
        socklen_t recv_len = sizeof(recv_addr);

        int recv_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&recv_addr, &recv_len);
        if (recv_bytes < 0) {
            std::cerr << "Failed to receive ACK" << std::endl;
            return;
        }

        buffer[recv_bytes] = '\0';
        std::cout << "Received ACK: " << buffer << std::endl;
    }

private:
    int sockfd;
    sockaddr_in server_addr;
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        return 1;
    }

    std::string serverIP = argv[1];
    int serverPort = std::stoi(argv[2]);

    UDPClient client(serverIP, serverPort);

    while (true) {
        std::string message;
        std::cout << "Enter a message to send (or 'q' to quit): ";
        std::getline(std::cin, message);

        if (message == "q") {
            break;
        }

        client.sendMessage(message);
    }

    return 0;
}