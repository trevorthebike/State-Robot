README - Robot State Machine and Echo Server

This project consists of a Robot State Machine implementation and an Echo Server program. The Robot State Machine simulates the behavior of a robot transitioning through different states based on time durations, while the Echo Server receives and processes network messages using the small_world library.

    Robot State Machine:
        The Robot State Machine is implemented using C++.
        The RobotState class represents a state of the robot and defines a pure virtual function decide_action() that needs to be implemented by derived classes.
        The StateMachine class manages the current state of the robot and triggers state transitions.
        The TimedState class is a subclass of RobotState and introduces time-based state transitions.
        The Tickable class provides a common interface for objects that can be ticked (i.e., updated) based on events.
        The main function demonstrates the usage of the Robot State Machine, simulating state transitions and printing relevant messages.

    Echo Server:
        The Echo Server is implemented using C++ and relies on the small_world library for network communication.
        The server listens on a specified port and accepts incoming connections.
        Once a connection is established, the server receives messages from the client and parses them using the SM_Event protocol buffer message.
        The parsed event type and time are displayed on the console.
        The server continues to receive messages until the client closes the connection.

To compile and run the project:

    Ensure that you have the necessary dependencies installed (e.g., C++ compiler, small_world library).
    Compile the source code using a C++ compiler with C++11 support (e.g., g++).

    c

g++ -std=c++11 -o robot_state_machine robot_state_machine.cpp socket.cpp Message.pb.cc -lprotobuf

Run the compiled executable.

bash

    ./robot_state_machine

        The Robot State Machine will simulate state transitions and print relevant messages.
        The Echo Server will listen on the specified port and display received event types and times.

Note: Make sure to update the robot_waiting and robot_moving strings in the main function to match the desired state transition messages.

Feel free to explore and modify the code as needed to suit your requirements.