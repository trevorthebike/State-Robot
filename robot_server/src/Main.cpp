
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
#include "Message.h"      
#define PORT 8888 

using namespace std;
class StateMachine;

class Tickable {
    uint64_t last_tick_time = 0;
    public:
       virtual void tick(const small_world::SM_Event& event) {
            last_tick_time = (event.event_time());
        }
};

class RobotState {
    //std::string name;
    //std::string verb;
    uint64_t initial_time = 0;
    uint64_t current_time = 0;
    std::map<std::string, std::shared_ptr<RobotState>> next_states;
    std::shared_ptr<StateMachine> owner;
    uint64_t get_elapsed() {
        return current_time - initial_time;
    }
    void set_next_state(const std::string& state_name, std::shared_ptr<RobotState> state) {
        next_states[state_name] = state;
    }
    std::shared_ptr<RobotState> get_next_state(const std::string& transition_name) {
        std::map<std::string, std::shared_ptr<RobotState>>::iterator it = next_states.find(transition_name);
        if (it == next_states.end()) return nullptr;
        return it->second;
    }
    public:    

        virtual void tick(const small_world::SM_Event& e) {
            if (initial_time == 0) {
                initial_time = (e.event_time());
                current_time = (e.event_time());
                decide_action(get_elapsed());
            }
        }
        
        virtual void decide_action(uint64_t elapsed) = 0;
        /*
        void set_name(const std::string& state_name) {
            name = state_name;
        }

        void set_verb(const std::string& action_verb) {
            verb = action_verb;
        }

        void set_owner(std::shared_ptr<StateMachine> state_machine) {
            owner = state_machine;
        }*/
};

class StateMachine : public Tickable {
    std::shared_ptr<RobotState> current_state;
    public:
        virtual void tick(const small_world::SM_Event& event) {
            Tickable::tick(event);
            if (current_state != nullptr) {
                current_state->tick(event);
            }
        }
        
        void set_current_state(std::shared_ptr<RobotState> cs) {
            current_state = cs;
        }
};

class TimedState : public RobotState {
    std::string state_name;
    std::string verb_name;
    uint64_t time_to_wait = 2000;
    void set_time_to_wait (uint64_t time_to_wait);
    void set_state_name(const std::string & state_name);
    We make a subclass for TimedStates;
    this has a time the state is waiting for
    Decide action is where we decide the
    next state based on the properties in the
    class.
    virtual void decide_action(uint64_t duration) {
        if (duration < time_to_wait) {
            cout << “Robot is” << verb_name << std:::end;
            return;
        }
        std::shared_ptr<RobotState> next = get_next_state[“done”];
        if (next == nullptr) {
            cout << “Can’t get a next state to go to” << endl;
            return;
        }
        cout << “Robot has moved to state: “ << next->get_state_name();
        owner->set_current_state(next);
    }
};

/*void state_machine( char buffer[]){
    cout << buffer << endl;
}*/

void echo_server () {
    while(true){
        server_socket listener (PORT);
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
                }
            } catch (const std::exception& err) {
                cout << err.what() << endl;
            }
    }
}

int main(int argc, char * argv[]) {
    
    std::shared_ptr<StateMachine> sm = std::make_shared<StateMachine>();
    std::shared_ptr<RobotState> s0 = std::make_shared<RobotState>();
    s0->set_name("wait state");
    s0->set_verb(robot_waiting);
    s0->set_owner(sm);

    std::shared_ptr<RobotState> s1 = std::make_shared<RobotState>();
    s1->set_name("moving state");
    s1->set_verb(robot_moving);
    s1->set_owner(sm);

    std::shared_ptr<RobotState> s2 = std::make_shared<RobotState>();
    s2->set_name("done state");

    s0->set_next_state("done", s1);
    s1->set_next_state("done", s2);

    sm->set_current_state(s0);
    // Simulate ticks
    sm->tick(); // Current state: wait state - The robot is waiting
    sm->tick(); // Current state: moving state - The robot is moving
    sm->tick(); // No state transition. //*/
    
	echo_server ();
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
