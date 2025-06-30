# include "NetworkMedium.h"
#include <string>

NetworkMedium::NetworkMedium() {
    log("NetworkMedium initialized...");
}

void NetworkMedium::registerNode(int nodeID){
    nodeInboxes[nodeID]; // Creates an empty for the nodeID if not exists
    log("Node " + std::to_string(nodeID) + " registered with the medium.");
}

void NetworkMedium::sendPacket(int sourceNodeID, int destNodeID, const RawPacket &packet){
    if (nodeInboxes.count(destNodeID) == 0) {
        log("Error: Destination Node " + std::to_string(destNodeID) + " not registered.");
        return;
    }
    nodeInboxes[destNodeID].push(packet);
    log("Packet from Node " + std::to_string(sourceNodeID) 
    + " send to Node " + std::to_string(destNodeID) 
    + " (Size: " + std::to_string(packet.size())+ " bytes).");
}

RawPacket NetworkMedium::receivePacket(int nodeID) {
    if(nodeInboxes.count(nodeID) == 0){
        log("Error: Node " + std::to_string(nodeID) + " not registered.");
        return RawPacket(); // Return an empty packet
    }

    if(nodeInboxes[nodeID].empty()){
        // log("Node " + std::to_string(nodeID) + " inbox is empty.")
        return RawPacket();
    }

    RawPacket receivedPacket = nodeInboxes[nodeID].front();
    nodeInboxes[nodeID].pop();
    log("Node " + std::to_string(nodeID) + " recieved a packet.");
    return receivedPacket;
}

bool NetworkMedium::hasPackets(int nodeID) const{
    auto it = nodeInboxes.find(nodeID);
    if(it != nodeInboxes.end()){
        return !it->second.empty();
    }
    return false; // Node not even registered, so no packets.
}

/*
Okay, let's dissect the `NetworkMedium.cpp` file. While the `.h` file declares *what* the `NetworkMedium` class is and *what* it can do, the `.cpp` file defines *how* it does those things. It contains the actual implementation (the "recipe") for the methods declared in `NetworkMedium.h`.

-----

### **Purpose of the `NetworkMedium.cpp` File**

The `NetworkMedium.cpp` file provides the **concrete implementation of the `NetworkMedium` class's methods**. It brings the blueprint defined in `NetworkMedium.h` to life.

In this file, we define the exact steps taken when:

  * A `NetworkMedium` object is created.
  * A `Node` registers itself.
  * A packet is sent from one node to another.
  * A node attempts to receive a packet.
  * A node checks its inbox for packets.

This file is where the "logic" of our simulated network's underlying communication (the "wire" and "post office" functions) resides.

-----

### **Line-by-Line Explanation of `NetworkMedium.cpp`**

**Explanation:**

1.  `#include "NetworkMedium.h"`: This line includes the corresponding header file.

      * **Why:** It's essential because this `.cpp` file implements the methods declared in `NetworkMedium.h`. Without it, the compiler wouldn't know what `NetworkMedium` or its methods are.

2.  `NetworkMedium::NetworkMedium() { ... }`: This is the **definition of the constructor** for the `NetworkMedium` class.

      * **Why:** When an object of `NetworkMedium` is created (e.g., `NetworkMedium medium;` in `main.cpp`), this code block is executed. It's where you put any initial setup logic.

3.  `log("NetworkMedium initialized.");`: This calls the `log` helper method (defined directly in the header) to print a message to the console.

      * **Why:** Provides feedback that the network medium has been successfully set up, which is helpful for debugging and understanding program flow.

4.  `}`: Closes the constructor definition.

5.  `void NetworkMedium::registerNode(int nodeId) { ... }`: This is the **definition of the `registerNode` method**.

      * **Why:** This method allows a `Node` to tell the `NetworkMedium` that it exists and wants to be part of the network.

6.  `nodeInboxes[nodeId];`: This is a crucial line. It accesses the `nodeInboxes` `std::map` using `nodeId` as the key.

      * **Why:** If `nodeId` is not already a key in `nodeInboxes`, `std::map` will automatically *insert* a new entry with `nodeId` as the key and a **default-constructed (empty) `std::queue<RawPacket>`** as its value. If `nodeId` already exists, it simply returns a reference to its existing queue. This effectively creates an "inbox" for the new node.

7.  `log("Node " + std::to_string(nodeId) + " registered with the medium.");`: Logs the successful registration.

      * **Why:** Provides clear output during runtime, making it easy to see which nodes have joined the network. `std::to_string(nodeId)` converts the integer `nodeId` into a string for concatenation.

8.  `}`: Closes the `registerNode` method definition.

9.  `void NetworkMedium::sendPacket(int sourceNodeId, int destNodeId, const RawPacket& packet) { ... }`: This is the **definition of the `sendPacket` method**.

      * **Why:** This method implements the core functionality of sending a packet from one node to another.

10. `if (nodeInboxes.count(destNodeId) == 0) { ... }`: This line checks if the `destNodeId` (the recipient) exists as a key in the `nodeInboxes` map. `count(key)` returns 1 if the key exists, 0 otherwise.

      * **Why:** It's a basic form of error handling. A packet cannot be delivered to a node that hasn't registered its inbox with the medium. This prevents trying to access a non-existent queue, which would lead to a runtime error.

11. `log("Error: Destination Node " + std::to_string(destNodeId) + " not registered.");`: Logs an error if the destination node isn't registered.

      * **Why:** Informs the user/debugger that a packet couldn't be delivered due to an unregistered recipient.

12. `return;`: If the destination node is not registered, this statement immediately exits the `sendPacket` function.

      * **Why:** Prevents further execution of the method if an invalid destination is provided, ensuring that the packet is not "sent" to a non-existent inbox.

13. `nodeInboxes[destNodeId].push(packet);`: This is the core "delivery" mechanism. It accesses the `std::queue<RawPacket>` associated with `destNodeId` and calls its `push()` method.

      * **Why:** The `push()` method adds the `packet` to the *back* of the destination node's queue. This models the packet being put into the recipient's "inbox."

14. `log("Packet from Node " + std::to_string(sourceNodeId) + ...);`: Logs the successful sending of the packet.

      * **Why:** Provides valuable tracing information, indicating which node sent what to whom and the packet's size.

15. `}`: Closes the `sendPacket` method definition.

16. `RawPacket NetworkMedium::receivePacket(int nodeId) { ... }`: This is the **definition of the `receivePacket` method**.

      * **Why:** This method allows a `Node` to retrieve a packet that has been sent to it.

17. `if (nodeInboxes.count(nodeId) == 0) { ... }`: Similar to `sendPacket`, this checks if the `nodeId` (the receiver) is registered.

      * **Why:** Ensures we don't try to access a non-existent inbox.

18. `log("Error: Node " + std::to_string(nodeId) + " not registered.");`: Logs an error.

      * **Why:** Informs if the requesting node itself isn't registered.

19. `return RawPacket();`: If the node is not registered, an empty `RawPacket` is returned.

      * **Why:** Provides a safe return value. The `RawPacket()` default constructor creates an empty packet, which the caller can check (e.g., `if (packet.size() == 0)`).

20. `if (nodeInboxes[nodeId].empty()) { ... }`: Checks if the node's specific inbox queue is empty.

      * **Why:** Prevents trying to access the `front()` element of an empty queue, which would lead to undefined behavior/crash.

21. `// log("Node " + std::to_string(nodeId) + " inbox is empty.");`: A commented-out log.

      * **Why:** Often, repeatedly checking for empty inboxes can generate a lot of log spam. This line is usually commented out for cleaner output during normal operation, but can be uncommented for specific debugging.

22. `return RawPacket();`: If the inbox is empty, an empty `RawPacket` is returned.

      * **Why:** Indicates that no packets were available for the node.

23. `RawPacket receivedPacket = nodeInboxes[nodeId].front();`: If packets exist, this line retrieves a copy of the packet at the *front* of the queue.

      * **Why:** `front()` gives access to the oldest packet (FIFO). We make a copy here.

24. `nodeInboxes[nodeId].pop();`: This line removes the packet from the *front* of the queue.

      * **Why:** After a packet is retrieved, it's removed from the inbox so it's not processed again. This is the "First-In, First-Out" (FIFO) behavior of a queue.

25. `log("Node " + std::to_string(nodeId) + " received a packet.");`: Logs the successful reception.

      * **Why:** Provides real-time feedback on packet reception.

26. `return receivedPacket;`: Returns the packet that was just retrieved and removed from the queue.

      * **Why:** This is the actual data the calling `Node` requested.

27. `}`: Closes the `receivePacket` method definition.

28. `bool NetworkMedium::hasPackets(int nodeId) const { ... }`: This is the **definition of the `hasPackets` method**.

      * **Why:** Provides a way for a `Node` to non-destructively check if there are packets waiting.

29. `auto it = nodeInboxes.find(nodeId);`: Uses the `find()` method of `std::map` to search for `nodeId` as a key. `find()` returns an iterator to the element if found, or `nodeInboxes.end()` if not found.

      * **Why:** This is a safe way to check for a key's existence in a map without potentially inserting a new element (like `nodeInboxes[nodeId]` would if the key didn't exist).

30. `if (it != nodeInboxes.end()) { ... }`: Checks if the `find()` operation was successful (i.e., the iterator `it` is not the "end" iterator, meaning the node was found).

      * **Why:** Proceed only if the node is actually registered.

31. `return !it->second.empty();`: If the node is found, `it->second` gives us access to the `std::queue<RawPacket>` associated with that node. `!it->second.empty()` returns `true` if the queue is *not* empty, and `false` if it *is* empty.

      * **Why:** This is the direct check for whether the specific node's inbox contains any packets.

32. `return false;`: If the `nodeId` was not found in the map (meaning the node isn't registered), this line is executed.

      * **Why:** An unregistered node cannot have any packets waiting for it on the medium.

33. `}`: Closes the `hasPackets` method definition.

-----

This comprehensive breakdown covers every line of `NetworkMedium.cpp`. You can see how these definitions bring the declared functionalities to life, enabling nodes to communicate through the simulated medium. Understanding this implementation detail is key to appreciating how network communication works at a fundamental level.
*/