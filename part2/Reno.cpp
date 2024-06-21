
#include"Reno.hpp"


Reno::Reno(/* args */)
{
}

Reno::~Reno()
{

}

void wait_time(int t){
    std::this_thread::sleep_for(std::chrono::microseconds(t));
}

void Reno::run(Client &client){
    client.handshake();
    int rec_ack[1000];
    int pn = 1 , ans_num=-1;
    int speed_rev = threshold * 4;
    int first_speed  = speed_rev;
    while (pn < 1000) {

        for(int s =0 ; s< 3; s++){
            std::string message = std::to_string(pn);
            client.sendMessage(message , pn);
            std::thread w(&wait_time , speed_rev);
            auto ans = client.recieveMessage(speed_rev);
            w.join();
            try{
                ans_num = std::stoi(ans);
            }catch(...){
                ans_num = -1;
            }

            if(ans == "TIMEOUT"){
                    speed_rev = first_speed * 2;
                    threshold = speed_rev * 2;
                    break;
            }else{
                rec_ack[ans_num]++;
                printl("ACK:" + ans + " " +std::to_string(pn));
                pn++;
                if(pn >= 1000)
                    break;
            }
        }
        speed_rev /=2;
    }
}