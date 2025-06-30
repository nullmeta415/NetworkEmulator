#ifndef NETWORK_MEDIUM_H
#define NETWORK_MEDIUM_H

#include"RawPacket.h" // We'll be sending/receiving RawPackets
#include<map> // To store queues for each node (NodeID -> Inbox Queue)
#include<queue> // For the inbox queue of each node
#include<string> // For logging messages
#include<iostream> // For logging to console

// The NetworkMedium simulates the physical layer and basic data link layer
// by providing a way for nodes to exchange raw packets.
class NetworkMedium{
private:
    // A map where key is NodeID and value is a queue of packets for the node.
    std::map<int , std::queue<RawPacket>> nodeInboxes;
    // A pair between int <---> queue of raw packets

public:
    NetworkMedium(); // Constructor

    // Register a node with the medium so it can receive packets
    void registerNode(int nodeID);

    // Send a packet from sourceNodeID to destNodeID
    // In Phase 1, we just put it directly into the destination's inbox.
    void sendPacket(int sourceNodeID, int destNodeID, const RawPacket &packet);

    // Receive a packet for a given nodeID
    // Returns an empty packet if no packets are available.
    RawPacket receivePacket(int nodeID);

    // Check if a node's inbox has any packets
    bool hasPackets(int nodeID) const;

    // Fod debugging/logging
    void log(const std::string &message) const {
        std::cout << "[NetworkMedium]" << message << std::endl;
    }
};

#endif // NETWORK_MEDIUM_H

/*
Let's break down the `NetworkMedium.h` file, its purpose, and every line of code within it. This class is absolutely critical as it provides the "virtual wire" for our simulated network.

-----

### **Purpose of the `NetworkMedium.h` File**

The `NetworkMedium.h` file declares the `NetworkMedium` class, which serves as the **central hub for all packet transmission** in our simulated network during Phase 1.

**Think of the `NetworkMedium` as the simplified representation of the entire physical and (very basic) data link infrastructure.** It's like:

  * **The "Wire" / "Cable":** It's the path through which packets travel.
  * **A Central Switch/Hub (without intelligence):** It receives packets from one "port" (node) and delivers them to the correct "destination port" (another node's inbox). It doesn't perform any routing or complex forwarding; it simply knows which node should receive which packet.
  * **A "Post Office":** Each node has an "inbox" (a queue) at this central post office. When a node sends a packet, the post office puts it in the recipient's inbox. When a node wants mail, it asks the post office for its inbox contents.

**Why is this class important in a network emulator?**

1.  **Decoupling Nodes:** It allows `Node` objects to communicate without knowing directly about each other's internal mechanisms. A `Node` just tells the `NetworkMedium` "send this to Node X," and the `NetworkMedium` handles the delivery. This mimics how real network interfaces (like Ethernet cards) abstract away the physical transmission.
2.  **Centralized Control:** By having a single `NetworkMedium` instance, we can easily introduce network impairments (like packet loss, delay, or corruption) in later phases, as all packets pass through this central point. This is crucial for realistic network simulation.
3.  **Simplicity for Phase 1:** In this phase, it keeps the `Node` class simple, as nodes don't need to directly manage connection states or complex addressing. They just interact with the medium.


**Explanation:**

1.  `#ifndef NETWORK_MEDIUM_H`: Start of the **include guard**.

2.  `#define NETWORK_MEDIUM_H`: Definition of the include guard macro.

      * **Purpose:** Prevents multiple inclusions of this header file within the same compilation unit, avoiding redefinition errors.

3.  `#include "RawPacket.h"`: Includes our custom `RawPacket` header.

      * **Purpose:** The `NetworkMedium` class will handle `RawPacket` objects (sending them, storing them in queues), so it needs to know the definition of the `RawPacket` class.

4.  `#include <map>`: Includes the standard C++ header for `std::map`.

      * **Purpose:** `std::map` is an associative container that stores elements formed by a combination of a key value and a mapped value. Here, we'll use it to map `NodeID`s (integer keys) to their respective packet inboxes (`std::queue<RawPacket>` values). This allows the medium to manage separate inboxes for each simulated node.

5.  `#include <queue>`: Includes the standard C++ header for `std::queue`.

      * **Purpose:** `std::queue` is a container adaptor that provides a FIFO (First-In, First-Out) data structure. We use it to model a node's inbox, where packets are received in the order they were sent.

6.  `#include <string>`: Includes the standard C++ header for `std::string`.

      * **Purpose:** Used for logging messages to the console (e.g., converting integers to strings with `std::to_string`).

7.  `#include <iostream>`: Includes the standard C++ header for `std::cout` and `std::endl`.

      * **Purpose:** Used for printing logging messages directly to the console from within the `NetworkMedium` class.

8.  `// The NetworkMedium simulates the physical layer ...`: This is a **comment**.

      * **Purpose:** Provides a high-level explanation of the class's role and what it simulates in the context of network layers.

9.  `class NetworkMedium { ... };`: Declares the `NetworkMedium` **class**.

      * **Purpose:** Defines the blueprint for creating `NetworkMedium` objects, specifying their data members and member functions.

10. `private:`: This is an **access specifier**.

      * **Purpose:** Members declared under `private:` can only be accessed from *within* the `NetworkMedium` class itself. This enforces encapsulation, meaning the internal workings (like `nodeInboxes`) are hidden from outside users of the class, promoting cleaner design and preventing accidental misuse.

11. `std::map<int, std::queue<RawPacket>> nodeInboxes;`: This declares a **private member variable** named `nodeInboxes`.

      * **Purpose:** This is the core data structure of the `NetworkMedium`. It holds the individual message queues (inboxes) for all registered nodes. Each integer key (`int`) uniquely identifies a `NodeID`, and its corresponding value (`std::queue<RawPacket>`) is the queue where packets destined for that node are stored.

12. `public:`: This is an **access specifier**.

      * **Purpose:** Members declared under `public:` can be accessed from *anywhere* outside the `NetworkMedium` class. These are the interfaces (methods) through which other classes (like `Node`) interact with the network medium.

13. `NetworkMedium();`: This declares the **constructor** for the `NetworkMedium` class.

      * **Purpose:** This special public method is called automatically when an object of `NetworkMedium` is created (`NetworkMedium medium;`). It's used for any initial setup the medium might need (e.g., printing an initialization message, as seen in the `.cpp` file).

14. `void registerNode(int nodeId);`: This declares a **public member function**.

      * **Purpose:** Before a node can send or receive packets, it needs to tell the `NetworkMedium` that it exists. This method allows a `Node` (identified by `nodeId`) to "register" itself, which typically involves the `NetworkMedium` preparing an inbox for that node. `void` means it doesn't return any value.

15. `void sendPacket(int sourceNodeId, int destNodeId, const RawPacket& packet);`: This declares a **public member function**.

      * **Purpose:** This is the method that a `Node` calls to send a `RawPacket`. It takes the `sourceNodeId` (for logging/identification), the `destNodeId` (to know which inbox to put the packet into), and a `const RawPacket& packet` (the packet to send, passed by constant reference for efficiency, meaning it's not copied and won't be modified by `sendPacket`).

16. `RawPacket receivePacket(int nodeId);`: This declares a **public member function**.

      * **Purpose:** This is the method a `Node` calls to retrieve a packet from its own inbox. It takes the `nodeId` to specify which inbox to check. It returns a `RawPacket` object. If the inbox is empty, it should return an "empty" `RawPacket` to indicate no data.

17. `bool hasPackets(int nodeId) const;`: This declares a **public member function**.

      * **Purpose:** Provides a way for a `Node` to check if its inbox on the `NetworkMedium` has any packets waiting without actually trying to retrieve one. This prevents trying to `receivePacket` when there's nothing there. The `const` at the end means this method does not modify the object's internal state.

18. `void log(const std::string& message) const { ... }`: This declares a **public member function** defined directly within the header (an "inline" function).

      * **Purpose:** A utility function for logging messages specifically from the `NetworkMedium`. By making it a member function, all log messages from this class will automatically be prefixed with `"[NetworkMedium]"`, making the output clearer during debugging. `const` means it doesn't modify the object.

19. `#endif // NETWORK_MEDIUM_H`: End of the include guard.

-----

This detailed breakdown should give you a solid understanding of the `NetworkMedium.h` file and the vital role the `NetworkMedium` class plays in our simulated network. It acts as the backbone, connecting all our independent nodes and providing the essential communication channel for `RawPacket` objects.
*/