#include "Node.h"
#include "NetworkMedium.h"
#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::seconds

int main(){
    std::cout << "--- Starting Network Emulator Phase 1 ---" << std::endl;
    // 1. Create the shared Network Medium
    NetworkMedium medium;

    // 2. Create two Nodes, connecting them to the same medium
    Node node1(1, medium);
    Node node2(2, medium);

    std::cout << "\n--- Node 1 sending message to Node 2 ---" << std::endl;
    node1.sendMessage(node2.getNodeID(), "Hello Node 2 from Node 1!");

    // Simulate some time passing for the packet to "travel"
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::cout << "\n--- Node 2 checking for messages ---" << std::endl;
    if(node2.hasIncomingMessages()){
        node2.receiveMessage();
    } else {
        std::cout << "[Main] Node 2 has no messages." << std::endl;
    }

    std::cout << "\n--- Node 2 sending reply to Node 1 ---" << std::endl;
    node2.sendMessage(node1.getNodeID(), "Hi Node 1! Got your message. Greeting from Node 2!");

    // Simulate some time passing
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::cout << "\n--- Node 1 checking for messages ---" << std::endl;
    if(node1.hasIncomingMessages()){
        node1.receiveMessage();
    } else {
        std::cout << "[Main] Node 1 has no messages." << std::endl;
    }

    std::cout << "\n--- End of Network Emulator Phase 1 ---" << std::endl;
    return 0;
}