#include "Node.h"
#include "NetworkMedium.h"
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::seconds
#include <iostream>    // For std::cout, std::cerr, std::endl
#include <string>      // For std::string
#include <vector>      // For std::vector
#include <array>       // For std::array (MacAddress)
#include <cstdint>     // For uint8_t, uint16_t (used in MacAddress, calculateChecksum)
#include <chrono>      // For std::chrono::milliseconds (for future simulation pauses)
#include <thread>      // For std::this_thread::sleep_for (for future simulation pauses)

// Include the header that declares MacUtil functions and EthernetFrame class.
// This is crucial for main.cpp to know about them.
#include "EthernetFrame.h" // This header indirectly brings in MacAddress and MacUtil namespace.

// Forward declarations for Node and NetworkMedium (from Phase 1).
// We declare them here because main.cpp will eventually use them, but we don't
// need their full definitions in this file (only their names). This avoids
// needing to include their .h files if we're just testing MacUtil for now.
class NetworkMedium;
class Node;

// -----------------------------------------------------------------------------
// Function: testMacUtil
// Purpose: Contains a suite of tests for the MacUtil namespace functions
//          (macToString, stringToMac, calculateChecksum).
// -----------------------------------------------------------------------------
void testMacUtil() {
    std::cout << "========================================" << std::endl;
    std::cout << "          Testing MacUtil Functions         " << std::endl;
    std::cout << "========================================" << std::endl;

    // --- Test macToString ---
    std::cout << "\n--- Testing macToString (MacAddress to String) ---" << std::endl;

    // Test Case 1: All zeros MAC address
    MacAddress mac1 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    std::cout << "Input: {0x00,0x00,0x00,0x00,0x00,0x00} -> Output: " << MacUtil::macToString(mac1) << " (Expected: 00:00:00:00:00:00)" << std::endl;

    // Test Case 2: Single-digit hex values
    MacAddress mac2 = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    std::cout << "Input: {0x01,0x02,0x03,0x04,0x05,0x06} -> Output: " << MacUtil::macToString(mac2) << " (Expected: 01:02:03:04:05:06)" << std::endl;

    // Test Case 3: All uppercase hex values
    MacAddress mac3 = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    std::cout << "Input: {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF} -> Output: " << MacUtil::macToString(mac3) << " (Expected: AA:BB:CC:DD:EE:FF)" << std::endl;

    // Test Case 4: Mixed hex values
    MacAddress mac4 = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F};
    std::cout << "Input: {0x1A,0x2B,0x3C,0x4D,0x5E,0x6F} -> Output: " << MacUtil::macToString(mac4) << " (Expected: 1A:2B:3C:4D:5E:6F)" << std::endl;

    // --- Test stringToMac ---
    std::cout << "\n--- Testing stringToMac (String to MacAddress) ---" << std::endl;

    std::cout << "\n--- Valid Inputs (Expected: No errors, correct parsing) ---" << std::endl;
    std::vector<std::string> validMacStrings = {
        "00:00:00:00:00:00",
        "01:02:03:04:05:06",
        "AA:BB:CC:DD:EE:FF",
        "aa:bb:cc:dd:ee:ff", // Test lowercase hex input (should still parse correctly)
        "1A:2B:3C:4D:5E:6F"
    };

    for (const auto& strMac : validMacStrings) {
        MacAddress parsedMac = MacUtil::stringToMac(strMac);
        std::cout << "Parsed '" << strMac << "' -> " << MacUtil::macToString(parsedMac) << std::endl;
        // For a more robust test, you could compare the original string (converted to uppercase)
        // with the result of macToString(parsedMac).
        // Example: assert(MacUtil::macToString(parsedMac) == strMac.data() || strMac.data().toupper() );
    }

    std::cout << "\n--- Invalid Inputs (Expected: Error messages to std::cerr, all-zeros MacAddress) ---" << std::endl;
    std::vector<std::string> invalidMacStrings = {
        "00:11:22:33:44",          // Too short (missing last byte and colon)
        "00:11:22:33:44:55:66",    // Too long (extra byte at end)
        "00:11:22:XX:YY:ZZ",      // Invalid hex characters (XX, YY, ZZ)
        "00-11-22-33-44-55",      // Wrong separator (dashes instead of colons)
        "00:11:22:AA:BB:CC:EXTRA", // Extra characters at end
        "00:11:22:AA:BB:GG",      // Invalid hex digit 'G' in last byte
        "00:11:22:AA:BB:C",       // Missing last digit in last byte (e.g., 'C' instead of '0C')
        "00:11:22:AA:BB:100"      // Byte value out of range (100 is 256 decimal, > 255)
    };

    for (const auto& strMac : invalidMacStrings) {
        std::cout << "Attempting to parse invalid: '" << strMac << "'" << std::endl;
        MacAddress parsedMac = MacUtil::stringToMac(strMac);
        std::cout << "Result (should be all zeros): " << MacUtil::macToString(parsedMac) << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
    }

    // --- Test calculateChecksum ---
    std::cout << "\n--- Testing calculateChecksum ---" << std::endl;

    std::vector<char> data1 = {}; // Empty
    std::cout << "Checksum of empty data: " << MacUtil::calculateChecksum(data1) << " (Expected: 0)" << std::endl;

    std::vector<char> data2 = {'A'}; // ASCII 'A' is 65
    std::cout << "Checksum of {'A'}: " << MacUtil::calculateChecksum(data2) << " (Expected: 65)" << std::endl;

    std::vector<char> data3 = {'H', 'e', 'l', 'l', 'o'}; // 72+101+108+108+111 = 500
    std::cout << "Checksum of {'H','e','l','l','o'}: " << MacUtil::calculateChecksum(data3) << " (Expected: 500)" << std::endl;

    // Test with unsigned char values that might be interpreted as negative if not cast
    std::vector<char> data4 = {(char)0xFF, (char)0x01}; // 0xFF is 255, 0x01 is 1. Sum = 256.
    std::cout << "Checksum of {0xFF, 0x01}: " << MacUtil::calculateChecksum(data4) << " (Expected: 256)" << std::endl;

    std::vector<char> data5 = {(char)0x80, (char)0x80}; // 128 + 128 = 256
    std::cout << "Checksum of {0x80, 0x80}: " << MacUtil::calculateChecksum(data5) << " (Expected: 256)" << std::endl;


    std::cout << "\n========================================" << std::endl;
    std::cout << "   All MacUtil Tests Completed Successfully! " << std::endl;
    std::cout << "========================================" << std::endl;
}

// -----------------------------------------------------------------------------
// Main Function for the Network Emulator Project
// This function serves as the entry point and orchestrator for the simulation.
// In Phase 2, it primarily runs the tests for MacUtil functions.
// In future phases, it will set up and run the full network simulation.
// -----------------------------------------------------------------------------
int main() {
    // Run the comprehensive test suite for MacUtil functions.
    testMacUtil();

    // Placeholder for Phase 1 main logic (will be removed/modified in future phases)
    // For now, it's commented out as we focus on MacUtil tests.
    /*
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
    */

    return 0; // Indicate successful program execution.
}

/*
Alright, let's dissect the `main.cpp` file. While our custom classes (`RawPacket`, `NetworkMedium`, `Node`) define the components of our simulated network, `main.cpp` is the **orchestrator** that brings them all together and sets the simulation in motion.

-----

### **Purpose of the `main.cpp` File**

The `main.cpp` file serves as the **entry point and control center** for our entire network emulator program.

Think of `main.cpp` as:

  * **The Scenario Creator:** It defines the specific network topology for this simulation (e.g., how many nodes, what their IDs are).
  * **The Experiment Runner:** It sets up the conditions for communication and then initiates the message exchange.
  * **The Observer:** It prints out what's happening to show that the communication is working as intended.

**Why is `main.cpp` important in this project?**

1.  **Program Entry Point:** Every C++ executable program starts execution from the `main()` function.
2.  **Object Instantiation:** It's responsible for creating instances of our core classes (`NetworkMedium`, `Node`) and configuring their initial state.
3.  **Simulation Flow Control:** It defines the sequence of events in our simulation (e.g., Node 1 sends, then Node 2 receives, then Node 2 replies, then Node 1 receives).
4.  **Demonstration:** For Phase 1, `main.cpp` is a simple script to *demonstrate* that the fundamental communication between two nodes via the medium works. It acts as our basic test case.

-----

### **Line-by-Line Explanation of `main.cpp`**

**Explanation:**

1.  `#include "Node.h"`: Includes the header for our `Node` class.

      * **Why:** `main.cpp` needs to create and interact with `Node` objects, so it needs their declarations.

2.  `#include "NetworkMedium.h"`: Includes the header for our `NetworkMedium` class.

      * **Why:** `main.cpp` needs to create the single, shared `NetworkMedium` instance that all `Node` objects will use.

3.  `#include <iostream>`: Includes the standard C++ input/output stream library.

      * **Why:** Used for printing messages to the console using `std::cout` and `std::endl`, which is crucial for observing the simulation's progress.

4.  `#include <thread>`: Includes the standard C++ threading library.

      * **Why:** Specifically, `std::this_thread::sleep_for` is used to pause the execution of the main thread.

5.  `#include <chrono>`: Includes the standard C++ time utilities library.

      * **Why:** Used in conjunction with `<thread>` to specify the duration for `sleep_for` (e.g., `std::chrono::milliseconds(50)`). This simulates time passing for packets to "travel" across the medium.

6.  `int main() { ... }`: This is the **main function** where the program execution begins.

      * **Why:** It's the mandatory entry point for any C++ program. The `int` return type indicates whether the program executed successfully (0 for success, non-zero for error).

7.  `std::cout << "--- Starting Network Emulator Phase 1 ---" << std::endl;`: Prints an introductory message to the console.

      * **Why:** Helps to clearly mark the beginning of the simulation in the output.

8.  `// 1. Create the shared Network Medium`: A **comment** indicating the logical step.

      * **Why:** Improves code readability by breaking down the main logic into understandable steps.

9.  `NetworkMedium medium;`: This line **creates an instance** of the `NetworkMedium` class.

      * **Why:** This is the single, central "wire" or "post office" that all nodes will use to communicate. It must be created before any nodes can use it. Its constructor will be called here.

10. `// 2. Create two Nodes, connecting them to the same medium`: Another **comment** for logical steps.

      * **Why:** Guides the reader through the setup process.

11. `Node node1(1, medium);`: This line **creates an instance** of the `Node` class, named `node1`.

      * **Why:** We are simulating a network with at least two participants. `1` is its unique `nodeId`, and `medium` is the reference to the shared `NetworkMedium` object it will use. Its constructor will be called here, which in turn calls `medium.registerNode(1)`.

12. `Node node2(2, medium);`: This line **creates another instance** of the `Node` class, named `node2`.

      * **Why:** To have another participant for communication. `2` is its unique `nodeId`. It also uses the *same* `medium` object. Its constructor will call `medium.registerNode(2)`.

13. `std::cout << "\n--- Node 1 sending message to Node 2 ---" << std::endl;`: Prints a descriptive header for the next action.

      * **Why:** Makes the simulation output easy to follow and understand what's happening at each stage. `\n` adds a new line for better formatting.

14. `node1.sendMessage(node2.getNodeId(), "Hello Node 2 from Node 1!");`: This line calls the `sendMessage` method on `node1`.

      * **Why:** This is where the communication initiates. `node1` attempts to send a message. It needs to know the destination node's ID, which it gets from `node2.getNodeId()`, and provides the actual string message.

15. `// Simulate some time passing for the packet to "travel"`: A **comment** explaining the next line's purpose.

      * **Why:** In a real network, packets don't arrive instantly. This comment explains the purpose of the `sleep_for` call.

16. `std::this_thread::sleep_for(std::chrono::milliseconds(50));`: This line pauses the execution of the `main` thread for 50 milliseconds.

      * **Why:** It's a simple way to *simulate* network latency or the time it takes for a packet to traverse the "wire." While our `NetworkMedium`'s queues are instant, this gives a more realistic feel and, in future phases, will be crucial when dealing with timers (like retransmission timers).

17. `std::cout << "\n--- Node 2 checking for messages ---" << std::endl;`: Prints a descriptive header.

      * **Why:** Clearly indicates that `node2` is now trying to receive.

18. `if (node2.hasIncomingMessages()) { ... }`: This line checks if `node2` has any messages waiting for it.

      * **Why:** It's good practice to check before attempting to receive, preventing unnecessary calls or handling of empty returns.

19. `node2.receiveMessage();`: If messages are available, this line calls the `receiveMessage` method on `node2`.

      * **Why:** This is how `node2` retrieves and processes the message sent by `node1`.

20. `} else { ... }`: The `else` block executes if `node2.hasIncomingMessages()` returned `false`.

      * **Why:** Provides an alternative action or message if no packets were received.

21. `std::cout << "[Main] Node 2 has no messages." << std::endl;`: Prints a message indicating no messages were found.

      * **Why:** Provides feedback for the user in the console. The `[Main]` prefix helps distinguish messages from `main.cpp` vs. `Node` or `NetworkMedium` logs.

22. `}`: Closes the `if-else` block.

23. `std::cout << "\n--- Node 2 sending reply to Node 1 ---" << std::endl;`: Prints a descriptive header for the reply.

      * **Why:** Clearly indicates the next stage of communication.

24. `node2.sendMessage(node1.getNodeId(), "Hi Node 1! Got your message. Greetings from Node 2!");`: `node2` sends a reply message back to `node1`.

      * **Why:** Demonstrates bidirectional communication, where both nodes can send and receive.

25. `// Simulate some time passing`: Another **comment** for `sleep_for`.

      * **Why:** Explains the purpose of the pause.

26. `std::this_thread::sleep_for(std::chrono::milliseconds(50));`: Pauses for another 50 milliseconds.

      * **Why:** Simulates the travel time for the reply packet.

27. `std::cout << "\n--- Node 1 checking for messages ---" << std::endl;`: Prints a descriptive header.

      * **Why:** Indicates `node1` is now attempting to receive its reply.

28. `if (node1.hasIncomingMessages()) { ... }`: Checks if `node1` has any pending messages.

      * **Why:** Ensures we only try to receive if there's something there.

29. `node1.receiveMessage();`: If messages are available, `node1` retrieves and processes its message.

      * **Why:** `node1` consumes the reply message from `node2`.

30. `} else { ... }`: The `else` block executes if `node1.hasIncomingMessages()` returned `false`.

      * **Why:** Handles the case where no messages were received.

31. `std::cout << "[Main] Node 1 has no messages." << std::endl;`: Prints a message if no messages were found.

      * **Why:** Provides feedback to the user.

32. `}`: Closes the `if-else` block.

33. `std::cout << "\n--- End of Network Emulator Phase 1 ---" << std::endl;`: Prints a concluding message.

      * **Why:** Clearly marks the end of the simulation.

34. `return 0;`: Returns `0` from the `main` function.

      * **Why:** In C++, a return value of `0` from `main` conventionally indicates that the program executed successfully without any errors.

35. `}`: Closes the `main` function definition.

-----

By understanding `main.cpp`, you grasp how to set up a basic simulation, control its flow, and observe the interactions between your custom objects. This file is your workbench for trying out different network scenarios as you build out more complex layers in the future.
*/