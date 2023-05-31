
#include <iostream>
#include <string>  
#include <typeinfo>
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>    
#include <sstream>
#include <arpa/inet.h>     
#include <sys/types.h> 
#include <netinet/in.h> 
#include "socket.h" 
#include "./generated/Message.pb.cc"      
#define PORT 8888 

using namespace std;

void echo_server () {
	server_socket listener (8888);
	accepted_socket client;
	listener.accept (client);
	cout <<  ": accepted connection: " << to_string (client) << endl;
	 try {
            for (;;) {
                char buffer[1 << 10];
                int nbytes = client.recv(buffer, sizeof buffer);
                if (nbytes == 0) {
                    cout << "Connection closed by client" << endl;
                    break;
                }
                client.send(buffer, nbytes);
                cout << buffer << endl;
            }
        } catch (const std::exception& err) {
            cout << err.what() << endl;
        }
}

int main(int argc, char * argv[]) {
    cout << " starting" << endl;
	while (true) echo_server ();
	cout << " finished" << endl;
    /*while ((valread = recv(new_socket, buffer, sizeof(buffer), 0)) > 0) {
        small_world::SM_Event rm;
        rm.ParseFromString(buffer);

        if (!rm.has_event_type()) {
            std::cout << "Error Cannot Parse Type" << std::endl;
        }
        else if (!rm.has_event_time()) {
            std::cout << "Error Cannot Parse Time" << std::endl;
        }
        else{
            std::cout << rm.event_type() << std::endl;
            std::cout << rm.event_time() << std::endl;
        }
        memset(buffer, 0, sizeof(buffer));
    }*/
	// use these strings to indicate the state transitions
	// the robot progresses through.  Do not modify these strings

	std::string robot_waiting = "The robot is waiting";
	std::string robot_moving = "The robot is moving";

	std::string robot_finished_waiting = "The robot finished waiting";
	std::string robot_finished_moving = "The robot finished moving";

	std::string robot_began_waiting = "The robot began waiting";
	std::string robot_begin_moving = "The robot begain moving";

	return EXIT_SUCCESS;
}