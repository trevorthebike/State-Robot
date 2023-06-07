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
class RobotState;

class Tickable {
    uint64_t last_tick_time = 0;
    public:
       virtual void tick(const small_world::SM_Event& event) {
            last_tick_time = (event.event_time());
        }
};

class RobotState {
    public:
    uint64_t initial_time = 0;
    uint64_t current_time = 0;
    std::map<std::string, std::shared_ptr<RobotState>> next_states;

    std::shared_ptr<StateMachine> owner;
    uint64_t get_elapsed() {
        //cout << "get elapsed. current time: " << current_time << "initial time: " << initial_time << endl;
        return current_time - initial_time;
    }
    void set_next_state(const std::string& state_name, std::shared_ptr<RobotState> state) {
        next_states[state_name] = state;
    }
    std::shared_ptr<RobotState> get_next_state(const std::string& transition_name) {
        std::map<std::string, std::shared_ptr<RobotState>>::iterator it = next_states.find(transition_name);
        if (it == next_states.end()){return nullptr; }
        return it->second;
    }    
    virtual void tick(const small_world::SM_Event& e) {
        current_time = (e.event_time());
        if (initial_time == 0) {  initial_time = (e.event_time()); }
        if(get_elapsed() > 2100 ){
            //cout << "resetting time" << endl; 
            initial_time = (e.event_time());   
        }
        //cout << "get_elapsed: "<< get_elapsed() << endl;
        decide_action(get_elapsed());
    }
    virtual void decide_action(uint64_t elapsed) = 0;
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
    public:
    std::string robot_waiting = "The robot is waiting";
	std::string robot_moving = "The robot is moving";
	std::string robot_finished_waiting = "The robot finished waiting";
	std::string robot_finished_moving = "The robot finished moving";
	std::string robot_began_waiting = "The robot began waiting";
	std::string robot_begin_moving = "The robot began moving";
    std::string state_name;
    std::string verb_name;
    uint64_t time_to_wait = 2000;
    void set_time_to_wait (uint64_t time_to_wait);
    void set_name(const std::string & state_name){ this->state_name = state_name; }
    void set_verb(const std::string & verb_name) { this->verb_name = verb_name; }
    void set_owner(std::shared_ptr<StateMachine> state_machine) {owner = state_machine;}

    virtual void decide_action(uint64_t duration) {
        if (duration < time_to_wait) {
            cout <<  verb_name << std::endl;
            return;
        }
        if(verb_name == robot_waiting){
            cout << robot_finished_waiting << endl;
            cout << robot_begin_moving << endl;
        }
        else if(verb_name ==  robot_moving){
            cout << robot_finished_moving << endl;
            cout << robot_began_waiting << endl;
        }
        std::shared_ptr<RobotState> next = get_next_state("done");
        if (next == nullptr) {
            cout << "Cant get a next state to go to" << endl;
            return;
        }
        owner->set_current_state(next);
    }
};


int main(int argc, char * argv[]) {
    std::string robot_waiting = "The robot is waiting";
	std::string robot_moving = "The robot is moving";
	std::string robot_finished_waiting = "The robot finished waiting";
	std::string robot_finished_moving = "The robot finished moving";
	std::string robot_began_waiting = "The robot began waiting";
	std::string robot_begin_moving = "The robot begain moving";

    std::shared_ptr<StateMachine> sm = std::make_shared<StateMachine>();
    std::shared_ptr<TimedState> s0 = std::make_shared<TimedState>();
    s0->set_name("wait state");
    s0->set_verb(robot_waiting);
    s0->set_owner(sm);

    std::shared_ptr<TimedState> s1 = std::make_shared<TimedState>();
    s1->set_name("moving state");
    s1->set_verb(robot_moving);
    s1->set_owner(sm);

    s0->set_next_state("done", s1);
    s1->set_next_state("done", s0);
    sm->set_current_state(s0);

    //cout << "s0 get next state: " << s1->get_next_state("wait state") << endl;

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
                    memset(buffer, 0, sizeof(buffer));
                    sm->tick(rm);
                }
            } catch (const std::exception& err) {
                cout << err.what() << endl;
            }
    }
	return EXIT_SUCCESS;
}