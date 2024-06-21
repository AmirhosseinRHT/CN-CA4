#include"Client.hpp"




Client::Client(const std::string& serverIP, int serverPort) {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(serverIP.c_str());
        server_addr.sin_port = htons(serverPort);
}

void Client::sendMessage(const std::string& message ,int seq) {
        Packet ans;
        ans.ack =0;
        ans.psh = 1;
        ans.syn =0;
        ans.syn_seq = seq;
        ans.data_size = message.size();
        std::memcpy(ans.data ,message.c_str() ,message.size());
        sendto(sockfd, &ans, sizeof(Packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        std::cout << "Sent message: " << ans.data << std::endl;

        
        // sockaddr_in recv_addr;
        // socklen_t recv_len = sizeof(recv_addr);

        // int recv_bytes = recvfrom(sockfd, &ans, sizeof(Packet), 0, (struct sockaddr*)&recv_addr, &recv_len);
        // if (recv_bytes < 0) {
        //     std::cerr << "Failed to receive ACK" << std::endl;
        //     return;
        // }

        //buffer[recv_bytes] = '\0';
        //if(ans.ack && ans.psh)
        //std::cout << "Received ACK: "<< std::endl;
    }

void Client::handshake(){
        int seq = 10 , ack_seq =0;
        Packet *syn_packet = make_syn_packet(seq , 0);
        std::cout << syn_packet->syn << " " << syn_packet->syn_seq << syn_packet->ack << std::endl;
        Packet syn_ack_packet;
        // SYN PART
        sendto(sockfd, (void *) syn_packet, sizeof(Packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        sockaddr_in recv_addr;
        socklen_t recv_len = sizeof(recv_addr);

        // SYN_ACK PART
        int len = recvfrom(sockfd,(void *) &syn_ack_packet, sizeof(Packet), 0, (struct sockaddr*)&recv_addr, &recv_len);
        if(syn_ack_packet.ack & (syn_ack_packet.syn)){
            printl("SYN-ACK recived");
            seq++;
            ack_seq = syn_ack_packet.syn_seq +1;
        }else{
            printl("connection failed!");
            return;
        }

        // ACK PART
        Packet *ack_packet = make_ack_packet(seq ,ack_seq);
        sendto(sockfd, (void *) ack_packet, sizeof(Packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    }

std::string Client::recieveMessage(int timeoutMicroSeconds){
        char buffer[BUFFER_SIZE * 2];
        socklen_t len;
        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        timeout.tv_sec = timeoutMicroSeconds / 1000000;
        timeout.tv_usec = timeoutMicroSeconds % 1000000;
        timeval k;
        
        int retval = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        if (retval == -1)
        {
            // Handle error
        } 
        else if (retval) 
        {
            int n = recvfrom(sockfd, (char *)buffer, sizeof(Packet), MSG_WAITALL, (struct sockaddr *)&server_addr, &len);
            // buffer[n] = '\0';
            Packet *p = (Packet *) buffer;
            p->data[p->data_size] = '\0';
            return std::string(p->data);
        } 
        else
            return "TIMEOUT";
}