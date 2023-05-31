#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <cstdint>
#include <netinet/in.h>
#include "./generated/Message.pb.cc"  
#define PORT 8888
#define TICK_INTERVAL 500

uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main()
{   int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)  {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)   {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }
    while (true){
        small_world::SM_Event tick_event;
        tick_event.set_event_type("tick");
        tick_event.set_event_time(std::to_string(timeSinceEpochMillisec()));
        std::string serialized_event;
        tick_event.SerializeToString(&serialized_event);
        if (send(sock, serialized_event.c_str(), serialized_event.size(), 0) != serialized_event.size())  {
            std::cout << "Error sending tick message" << std::endl;
        }
        usleep(TICK_INTERVAL * 5000); 
    }
    close(sock);
    return 0;
}