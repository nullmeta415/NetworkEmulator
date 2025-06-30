# Network Emulator Project in C++

## Project Overview

This project aims to build a **Miniature Protocol Stack / Network Emulator** from scratch using C++. The core idea is to simulate the various layers of network communication (inspired by the TCP/IP and OSI models) to gain a deep, practical understanding of computer networks. This hands-on experience is invaluable for solidifying theoretical knowledge (e.g., for GATE exams) and for demonstrating practical skills in technical interviews.

The project is structured into progressive phases, each building upon the previous one by adding more complexity, implementing new network layers, and introducing advanced functionalities.

## Phase 1: Barebones "Hello World" & Basic Medium

This phase establishes the absolute fundamental building blocks for network communication, focusing on getting two simple "nodes" to exchange raw messages through a shared medium.

### Goals of Phase 1

- **Data Container:** Implement a `RawPacket` class to hold raw byte data.
    
- **Network Infrastructure Simulation:** Develop a `NetworkMedium` class to simulate the physical layer and basic data link layer, enabling packet transport between nodes.
    
- **Network Endpoints:** Create a `Node` class to represent individual computers/hosts capable of sending and receiving messages.
    
- **Basic Communication Flow:** Demonstrate a successful exchange of simple string messages between two `Node` objects via the `NetworkMedium`.
    

### Key Concepts Covered in Phase 1

- **Object-Oriented Programming (OOP) Fundamentals:** Classes, Objects, Methods, Attributes, Encapsulation, Access Specifiers (`public`, `private`).
    
- **C++ Standard Library Containers:** `std::vector<char>`, `std::string`, `std::map`, `std::queue`.
    
- **Reference Semantics:** Understanding and using `const&` for efficient parameter passing and shared object access.
    
- **Include Guards:** Preventing multiple header inclusions.
    
- **Basic I/O and Logging:** Using `std::cout` for program output and debugging.
    
- **Foundational Network Concepts (Simplified):**
    
    - **Nodes/Hosts:** Endpoints of communication.
        
    - **Network Medium:** The shared channel for data transfer.
        
    - **Packet/Frame (Simplified):** The basic unit of data exchanged.
        
    - **Initial Encapsulation/Decapsulation:** Wrapping/unwrapping application data into/from a raw packet.
        
    - **Delegation:** How classes rely on each other for specific tasks.
        

### Project Structure (Phase 1)

```
NetworkEmulator/
├── .gitignore
├── README.md
└── src/
    ├── main.cpp
    ├── Node.h
    ├── Node.cpp
    ├── NetworkMedium.h
    ├── NetworkMedium.cpp
    ├── RawPacket.h
    └── RawPacket.cpp
```

### Setup, Compile, and Run (Phase 1)

Follow these steps to get the Phase 1 emulator running on your system.

#### 1. Prerequisites

- **C++ Compiler:** `g++` (recommended for Linux/macOS/MinGW on Windows) or MSVC (for Visual Studio on Windows).
    
- **Git:** For cloning the repository and managing versions.
    

#### 2. Clone the Repository (if not already done)

```bash
git clone <your-repository-url> # Replace with your actual GitHub repository URL
cd NetworkEmulator
```

#### 3. Compile the Project

Navigate to the root of the `NetworkEmulator` directory in your terminal or command prompt.

```bash
g++ src/*.cpp -o network_emulator -std=c++17 -Wall -Wextra
```

- `src/*.cpp`: Compiles all `.cpp` files in the `src` directory.
    
- `-o network_emulator`: Specifies the output executable name as `network_emulator`.
    
- `-std=c++17`: Compiles using the C++17 standard (recommended for modern C++ features).
    
- `-Wall -Wextra`: Enables most common and extra warning messages, which is crucial for writing robust and clean C++ code.
    

#### 4. Run the Emulator

After successful compilation, run the executable:

```bash
./network_emulator
```

#### 5. Expected Output

You should see console output similar to this, demonstrating `Node 1` sending a message to `Node 2`, and `Node 2` replying to `Node 1`:

```bash
--- Starting Network Emulator Phase 1 ---
[NetworkMedium] NetworkMedium initialized.
[NetworkMedium] Node 1 registered with the medium.
[Node 1] Initialized.
[NetworkMedium] Node 2 registered with the medium.
[Node 2] Initialized.

--- Node 1 sending message to Node 2 ---
[Node 1] Attempting to send message 'Hello Node 2 from Node 1!' to Node 2
[NetworkMedium] Packet from Node 1 sent to Node 2 (Size: 26 bytes)

--- Node 2 checking for messages ---
[Node 2] Received message: 'Hello Node 2 from Node 1!'

--- Node 2 sending reply to Node 1 ---
[Node 2] Attempting to send message 'Hi Node 1! Got your message. Greetings from Node 2!' to Node 1
[NetworkMedium] Packet from Node 2 sent to Node 1 (Size: 52 bytes)

--- Node 1 checking for messages ---
[Node 1] Received message: 'Hi Node 1! Got your message. Greetings from Node 2!'

--- End of Network Emulator Phase 1 ---
```

## Future Phases (Outlook)

This project will evolve significantly in upcoming phases, addressing the limitations of Phase 1 and implementing more complex networking concepts:

- **Phase 2: Data Link Layer (MyEthernet):** Introduce proper framing, conceptual MAC addressing, and basic error detection.
    
- **Phase 3: Network Layer (MyIP):** Implement logical IP addressing, static routing tables, and the concept of an IP packet for inter-network communication.
    
- **Phase 4: Transport Layer (MyTCP/MyUDP):** Develop a simplified reliable data transfer protocol (like TCP), including connection management (handshake), sequence numbers, acknowledgments, retransmission timers, and flow control.
    
- **Phase 5: Advanced Features:** Introduce concepts like congestion control, dynamic routing protocols (e.g., Distance Vector), Address Resolution Protocol (ARP) simulation, and potentially simple application-layer protocols like DNS.
    

## Contributing

Contributions are welcome! If you have suggestions or improvements, feel free to open an issue or submit a pull request.