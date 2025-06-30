#ifndef NODE_H
#define NODE_H

#include "NetworkMedium.h" // Node communicates via the medium
#include "RawPacket.h" // Node send/receives raw packets
#include <string>
#include <iostream>

// Represent a single computer/host in our simulated network.
class Node{
private:
    int nodeID;
    NetworkMedium &medium; // Reference to the shared network medium

public:
    Node(int id, NetworkMedium &networkMedium);

    int getNodeID() const;

    // Send a message (string) to another node by its ID
    void sendMessage(int destNodeID, const std::string &message);

    // Check if there are any pending messages for this node
    bool hasIncomingMessages() const;

    // Receive a message and return it as a string
    std::string receiveMessage();

    // For debugging/logging
    void log(const std::string &message) const{
        std::cout << "[Node " << nodeID << "] " << message << std::endl;
    }
};

#endif // NODE_H

/*
Alright, let's break down the `Node.h` file. This file defines our `Node` class, which is a very important part of our simulation.

-----

### **Purpose of the `Node.h` File**

The `Node.h` file declares the `Node` class, which represents a **single independent computer or host** in our simulated network.

Think of a `Node` as:

  * **Your Personal Computer (PC):** It's an endpoint that wants to send and receive messages.
  * **A Participant:** It's the entity that initiates communication and receives communication.

**Why is this class important in a network emulator?**

1.  **User-Facing Entity:** In a real network, applications run on hosts. The `Node` class serves as our fundamental "host" where our simulated applications (like our simple message sending/receiving) will reside. It's the entry and exit point for application data.
2.  **Encapsulation of the Stack (Conceptual for now):** In later phases, the `Node` class will "contain" instances of our `DataLinkLayer`, `NetworkLayer`, and `TransportLayer`. It will be responsible for orchestrating the flow of data up and down its own protocol stack. Even in Phase 1, it already *uses* the `NetworkMedium`, which is a step towards building the stack.
3.  **Unique Identification:** Each `Node` has a `nodeId`, which in this phase serves as its unique address on our simple network medium. This sets the stage for more complex addressing schemes (MAC, IP) later.

-----

### **Line-by-Line Explanation of `Node.h`**

**Explanation:**

1.  `#ifndef NODE_H`: This is the start of an **include guard**.

2.  `#define NODE_H`: This defines the include guard macro.

      * **Purpose:** To prevent the contents of `Node.h` from being processed multiple times if included in different parts of the project, which could lead to redefinition errors.

3.  `#include "NetworkMedium.h"`: Includes the header for our `NetworkMedium` class.

      * **Purpose:** The `Node` class needs to interact with the `NetworkMedium` to send and receive packets. Therefore, it needs to know the declaration of the `NetworkMedium` class and its public methods.

4.  `#include "RawPacket.h"`: Includes the header for our `RawPacket` class.

      * **Purpose:** When a `Node` sends a message, it converts that message into a `RawPacket`. When it receives, it gets a `RawPacket`. So, it needs to know the `RawPacket` class definition.

5.  `#include <string>`: Includes the standard C++ header for `std::string`.

      * **Purpose:** The `Node` class directly handles `std::string` messages (both sending and receiving) in this phase. Also used for building logging messages with `std::to_string`.

6.  `#include <iostream>`: Includes the standard C++ header for `std::cout` and `std::endl`.

      * **Purpose:** Used for logging messages from the `Node` to the console, providing feedback on its actions.

7.  `// Represents a single computer/host in our simulated network.`: This is a **comment**.

      * **Purpose:** Provides a high-level explanation of what the `Node` class represents in the simulation.

8.  `class Node { ... };`: This declares the `Node` **class**.

      * **Purpose:** This is the blueprint for creating individual "node" objects in our simulated network.

9.  `private:`: This is an **access specifier**.

      * **Purpose:** Members declared under `private:` are only accessible from within the `Node` class itself. This enforces **encapsulation**, meaning the internal state (`nodeId`, `medium`) is protected from direct external modification, ensuring data integrity and better design.

10. `int nodeId;`: Declares a **private member variable** named `nodeId` of type `int`.

      * **Purpose:** This stores the unique identifier for this specific node in the network. In Phase 1, this is how the `NetworkMedium` knows which inbox belongs to which node.

11. `NetworkMedium& medium;`: Declares a **private member variable** named `medium` of type `NetworkMedium&`.

      * **Purpose:** This is a **reference** to the *single, shared* `NetworkMedium` object that all nodes will use to communicate. Using a reference (`&`) is important here:
          * It means `medium` is an alias to an *existing* `NetworkMedium` object, not a copy.
          * It ensures all nodes are talking through the *same* simulated network infrastructure.
          * It must be initialized in the constructor's member initializer list.

12. `public:`: This is an **access specifier**.

      * **Purpose:** Members declared under `public:` can be accessed from outside the `Node` class. These are the interfaces through which other parts of your program (like `main()`) interact with a `Node` object.

13. `Node(int id, NetworkMedium& networkMedium);`: This declares the **constructor** for the `Node` class.

      * **Purpose:** This special public method is called automatically when a `Node` object is created (e.g., `Node node1(1, medium);`). It takes an `id` for the node and a reference to the `networkMedium` it will use. This is where the `nodeId` and `medium` private members will be initialized.

14. `int getNodeId() const;`: This declares a **public member function**.

      * **Purpose:** Allows external code (like `main()`) to retrieve the unique ID of the node. The `const` indicates that this method does not modify the object's state.

15. `void sendMessage(int destNodeId, const std::string& message);`: This declares a **public member function**.

      * **Purpose:** This is the primary method for a `Node` to send a message. It takes the ID of the `destNodeId` and the actual `message` (as a `std::string`). `void` means it doesn't return anything. `const std::string&` passes the message efficiently without copying and ensures it's not modified.

16. `bool hasIncomingMessages() const;`: This declares a **public member function**.

      * **Purpose:** Allows external code to check if this `Node` has any pending messages in its inbox on the `NetworkMedium` without actually trying to receive one. This is useful for polling. `const` means it doesn't modify the object.

17. `std::string receiveMessage();`: This declares a **public member function**.

      * **Purpose:** This is the primary method for a `Node` to receive a message. It will retrieve a `RawPacket` from the `NetworkMedium`, convert its content back to a `std::string`, and return it.

18. `void log(const std::string& message) const { ... }`: This declares a **public member function** defined directly within the header (an "inline" function).

      * **Purpose:** A utility function for logging messages specifically from this `Node` instance. It automatically prefixes messages with `"[Node X]"` (where X is the `nodeId`), making the output clear when multiple nodes are active. `const` means it doesn't modify the object.

19. `#endif // NODE_H`: This is the end of the **include guard**.

-----

By understanding the `Node.h` file, you now grasp how we define the basic "actors" in our network simulation. These `Node` objects will be the entities that experience and interact with all the layers we build in subsequent phases. They are the "users" of our simulated network services.
*/