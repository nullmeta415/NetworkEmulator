#include "Node.h"

Node::Node(int id, NetworkMedium &networkMedium)
    : nodeID(id), medium(networkMedium) {
        medium.registerNode(nodeID); // Register this node with the medium
        log("Initialized.");
}

int Node::getNodeID() const{
    return nodeID;
}

void Node::sendMessage(int destNodeID, const std::string &message) {
    log("Attempting to send message '" + message + "' to Node " + std::to_string(destNodeID));
    RawPacket packet(message); // Create a RawPacket form the string
    medium.sendPacket(nodeID, destNodeID, packet);
}

bool Node::hasIncomingMessages() const{
    return medium.hasPackets(nodeID);
}

std::string Node::receiveMessage() {
    if(hasIncomingMessages()){
        RawPacket receivedPacket = medium.receivePacket(nodeID);
        std::string message = receivedPacket.toString();
        log("Received message: '" + message + "'");
        return message;
    }
    log("No incoming messages.");
    return ""; // Return empty string if no message
}

/*
Alright, let's break down the `Node.cpp` file. Just as `NetworkMedium.cpp` provides the implementation for the network medium's behavior, `Node.cpp` defines the actual actions and logic for our individual simulated computers.

-----

### **Purpose of the `Node.cpp` File**

The `Node.cpp` file contains the **concrete implementation of the `Node` class's methods** that were declared in `Node.h`. It dictates *how* a `Node` behaves in our simulated network.

This file defines:

  * How a `Node` is created and initialized.
  * How a `Node` sends a message to another node.
  * How a `Node` checks for incoming messages.
  * How a `Node` retrieves an incoming message.

Essentially, `Node.cpp` is where we define the "application logic" for our simple nodes in Phase 1, making them capable of basic communication through the `NetworkMedium`.

-----

### **Line-by-Line Explanation of `Node.cpp`**

**Explanation:**

1.  `#include "Node.h"`: This line includes the corresponding header file for `Node`.

      * **Why:** It's essential because this `.cpp` file defines the methods declared in `Node.h`. It needs access to the class definition and method prototypes.

2.  `Node::Node(int id, NetworkMedium& networkMedium)`: This is the **definition of the constructor** for the `Node` class.

      * **Why:** This code block is executed whenever a `Node` object is created (e.g., `Node node1(1, medium);` in `main.cpp`). It's responsible for initializing the `Node`'s unique ID and setting up its connection to the shared `NetworkMedium`.

3.  `: nodeId(id), medium(networkMedium)`: This is a **member initializer list**.

      * **Why:** This is the preferred and most efficient way to initialize member variables in C++ constructors, especially for references (`medium`) and `const` members. `nodeId(id)` initializes the `nodeId` member with the value passed as `id`. `medium(networkMedium)` initializes the `medium` reference to bind to the `networkMedium` object passed into the constructor. For references, this *must* be used, as references cannot be assigned to after construction.

4.  `medium.registerNode(nodeId);`: This line calls the `registerNode` method on the `medium` object, passing the `Node`'s own `nodeId`.

      * **Why:** When a `Node` is created, it immediately "tells" the `NetworkMedium` about its existence. This ensures that the `NetworkMedium` can prepare an inbox (`std::queue`) specifically for this `nodeId` to receive packets. It's the node's way of "joining the network."

5.  `log("Initialized.");`: This calls the `log` helper method (defined directly in `Node.h`) to print an initialization message.

      * **Why:** Provides clear feedback in the console when a new `Node` object is successfully set up.

6.  `}`: Closes the constructor definition.

7.  `int Node::getNodeId() const { ... }`: This is the **definition of the `getNodeId` method**.

      * **Why:** This simple accessor method allows other parts of the program (like `main.cpp` when calling `node2.getNodeId()`) to retrieve the unique identifier of this `Node` object.

8.  `return nodeId;`: Returns the integer value stored in the `nodeId` member variable.

      * **Why:** This is the actual data requested by the caller.

9.  `}`: Closes the `getNodeId` method definition.

10. `void Node::sendMessage(int destNodeId, const std::string& message) { ... }`: This is the **definition of the `sendMessage` method**.

      * **Why:** This method encapsulates the logic for a `Node` to send a string message to another specified `NodeID`. This is our application's "send" function in Phase 1.

11. `log("Attempting to send message '" + message + "' to Node " + std::to_string(destNodeId));`: Logs the action the node is taking.

      * **Why:** Provides clear runtime tracing, showing what message is being sent and to which destination. `std::to_string` converts `destNodeId` to a string for concatenation.

12. `RawPacket packet(message);`: This line creates a new `RawPacket` object.

      * **Why:** The `NetworkMedium` (our simulated physical layer) only understands `RawPacket`s. So, the application-level `std::string` message needs to be wrapped inside a `RawPacket` container before it can be sent over the medium. This is the first conceptual step of **encapsulation** – putting the application data into a packet.

13. `medium.sendPacket(nodeId, destNodeId, packet);`: This line delegates the actual sending to the `NetworkMedium`.

      * **Why:** The `Node` itself doesn't directly handle the "wire." It relies on the `NetworkMedium` to perform the actual packet transmission. It passes its own `nodeId` (as the source), the `destNodeId`, and the `packet` it just created. This is a crucial separation of concerns: the `Node` focuses on *what* to send, the `NetworkMedium` on *how* to send it.

14. `}`: Closes the `sendMessage` method definition.

15. `bool Node::hasIncomingMessages() const { ... }`: This is the **definition of the `hasIncomingMessages` method**.

      * **Why:** This method allows a `Node` to check if there are any packets waiting for it in its inbox on the `NetworkMedium` before attempting to retrieve one. This prevents trying to `receivePacket()` when the inbox is empty.

16. `return medium.hasPackets(nodeId);`: This line delegates the check to the `NetworkMedium`.

      * **Why:** The `Node` doesn't directly manage its inbox; the `NetworkMedium` does. So, the `Node` asks the `NetworkMedium` if it has any packets for its `nodeId`. This is another example of **delegation** and separation of concerns.

17. `}`: Closes the `hasIncomingMessages` method definition.

18. `std::string Node::receiveMessage() { ... }`: This is the **definition of the `receiveMessage` method**.

      * **Why:** This method implements the core functionality for a `Node` to retrieve and process a message that has been sent to it. This is our application's "receive" function in Phase 1.

19. `if (hasIncomingMessages()) { ... }`: This line checks if there are any messages before proceeding.

      * **Why:** It's good practice to verify that there's data to receive to avoid errors or returning empty data unnecessarily. It calls the `hasIncomingMessages()` method we just defined.

20. `RawPacket receivedPacket = medium.receivePacket(nodeId);`: If there are messages, this line retrieves the `RawPacket` from the `NetworkMedium`.

      * **Why:** The `Node` asks the `NetworkMedium` to deliver the next packet from its inbox. The received data is still in its raw `RawPacket` form.

21. `std::string message = receivedPacket.toString();`: This line converts the `RawPacket`'s internal data back into a `std::string`.

      * **Why:** Since our "application" in Phase 1 deals with string messages, we need to convert the raw bytes back into a human-readable string for processing or display. This is the first conceptual step of **decapsulation** – extracting the application data from the packet.

22. `log("Received message: '" + message + "'");`: Logs the successfully received message.

      * **Why:** Provides clear runtime feedback that a message has arrived and what its content is.

23. `return message;`: Returns the extracted string message to the caller (e.g., `main.cpp`).

      * **Why:** This is the final result of the receive operation at the application level.

24. `log("No incoming messages.");`: If `hasIncomingMessages()` returned `false`, this line logs that no messages were found.

      * **Why:** Provides feedback to the user/debugger in cases where a node tries to receive but nothing is available.

25. `return "";`: Returns an empty string if no message was received.

      * **Why:** Provides a clear indication to the caller that the operation did not result in a new message.

26. `}`: Closes the `receiveMessage` method definition.

-----

By going through `Node.cpp` line by line, you can see how the `Node` acts as the "user" of our simulated network. It converts application data into a packet, relies on the `NetworkMedium` to transfer it, and then extracts the application data from received packets. This demonstrates the basic interaction between an "application" (our simple message sending) and the underlying "network infrastructure" (`NetworkMedium`).
*/