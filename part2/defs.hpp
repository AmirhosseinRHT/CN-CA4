#ifndef DEFS_HPP
#define DEFS_HPP


#define MESSAGE_SIZE 1024


struct Packet{
    unsigned int syn_seq;
    unsigned int ack_seq;
    unsigned short syn;
    unsigned short ack;
    unsigned short psh;
    unsigned int data_size;
    char data[1024];      
};


void printl(std::string s){
            std::cout << s << std::endl;
}

Packet *make_syn_ack_packet(int seq , int ack_seq){
    Packet * packet = new Packet;    
    packet->ack_seq = ack_seq;
    packet->syn_seq = seq;
    packet->syn=1;
    packet->ack=1;
    return packet;
}

Packet *make_syn_packet(int seq , int ack_seq){
    Packet * packet = new Packet;    
    packet->ack_seq = ack_seq;
    packet->syn_seq = seq;
    packet->syn=1;
    packet->ack=0;
    return packet;
}


Packet *make_ack_packet(int seq , int ack_seq){
    Packet * packet = new Packet;    
    packet->ack_seq = ack_seq;
    packet->syn_seq = seq;
    packet->syn=0;
    packet->ack=1;
    return packet;
}


#endif
