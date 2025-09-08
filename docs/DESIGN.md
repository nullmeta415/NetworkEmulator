# Network Emulator Design Documentation

This document outlines the high-level design and core components of our network emulator project. It serves as a comprehensive reference for understanding the system's architecture and the purpose of each component.

---

## 1. Project Structure

The project is organized into a clean and professional directory structure to ensure clarity and maintainability.

* `./`: The root directory of the project.
* `src/`: A subdirectory that contains all C++ source code (`.cpp`) and header (`.h`) files.
* `docs/`: A folder dedicated to detailed design documentation files, such as this one.
* `README.md`: A top-level file that provides a general overview of the project and instructions for building and using it.
* `.gitignore`: A file that instructs the Git version control system to ignore specified files and directories (e.g., compiled binaries, object files) to keep the repository clean.

---

## 2. Physical Layer Simulation

### 2.1 What is Network Emulation?

**Definition:** Network emulation is the process of creating a simulated environment that mimics the behavior and characteristics of a real computer network. It is a user-space simulation, meaning our network components (nodes, medium, packets) are C++ objects and functions that run within the operating system, without direct interaction with real network hardware.

**Purpose:** Building an emulator helps us:
* **Understand Complex Systems:** We can learn how different network layers (like the Data Link Layer) interact and how data flows without needing expensive or complex physical setups.
* **Test & Debug:** We can test new network protocols and algorithms in a controlled, repeatable environment. This allows us to intentionally introduce errors or delays to observe system behavior.
* **Learn by Doing:** The hands-on process of implementing these concepts solidifies theoretical knowledge of both networking and C++ programming.

### 2.2 The Physical Layer (Layer 1) and Our `NetworkMedium`

**Conceptual Role:** The physical layer is the lowest layer of a network model. It deals with the raw physical transmission of bits (0s and 1s) over a communication medium (e.g., electrical signals on a wire). Its only concern is how to send and receive these raw bits, not what they mean.

**Our Simulation (`NetworkMedium`):** In our emulator, the `NetworkMedium` class simulates this physical layer. It acts as a shared channel or bus that all our simulated `Node` objects can connect to. Its key responsibility is to move raw data from a sender to all connected receivers without interpreting the data. It is like a perfectly reliable virtual wire.

---

## 3. The `RawPacket` Data Unit

**What it is:** A `RawPacket` is the most basic unit of data our `NetworkMedium` can transport. It is a simple container, represented as a `std::vector<char>`, that holds a sequence of raw bytes. This `std::vector<char>` is a perfect tool for this purpose because it can hold any sequence of bytes (a `char` is a single byte in C++) and its size can change dynamically, accommodating messages of different lengths.

**Purpose:** The `RawPacket` serves as the fundamental "envelope" for data. It represents the raw bitstream that would travel over a real physical medium before any higher-layer protocols (like the Data Link Layer) add structure to it. The sole purpose is to hold the raw bytes.

```cpp
class RawPacket {
public:
    std::vector<char> data;
};
```
---

## 4. The Network Endpoints & Project Scope

**The `Node` Class:** The `Node` class represents a single device or computer connected to our simulated network. A `Node` acts as the interface between the application layer and the underlying network layers, originating and consuming data.

**Project Scope:** This project will focus on building a simplified, multi-node network emulator that simulates the basic functionality of the **Data Link Layer**. We will build the components that allow nodes to send and receive data frames, handle addressing, and detect basic errors.

---

## 5. The `NetworkMedium` Class

### 5.1 What is the NetworkMedium?

**Conceptual Role:** The `NetworkMedium` class simulates a shared physical communication channel, such as an Ethernet cable or a Wi-Fi signal. In a real network, all devices on a local network segment share the same medium.

**Our Simulation:** In our project, all `Node` objects will have access to a single, shared `NetworkMedium` object. When a `Node` "sends" a packet, it places it onto the `NetworkMedium`, and when a `Node` wants to "receive," it retrieves a packet from the medium. This decouples the sender from the receiver, just like a real network.

### 5.2 Implementation Choices

To simulate a channel where packets are sent and received in order, the best C++ tool for the job is `std::queue`.

* **`std::queue<T>`:** This is a C++ Standard Library container adapter. It provides a FIFO (First-In, First-Out) data structure.
    * **"First-In, First-Out":** The first element added to the queue is always the first one to be removed.
    * **Why we use it:** This behavior perfectly models a communication channel. Packets sent by a Node are placed at the back of the queue (the "send" operation), and packets are retrieved from the front of the queue (the "receive" operation). This ensures that the order of delivery is maintained.

* **`std::vector<char>` vs. `std::queue<RawPacket>`:**
    * A `std::vector` is great for holding a contiguous sequence of elements, like the bytes inside a single `RawPacket`.
    * A `std::queue` is a better tool for managing a collection of `RawPacket`s in the order they were sent.

### 5.3 The Public Interface of NetworkMedium

A well-designed class has a clear public interface. For the `NetworkMedium`, we will have three simple functions:

* `void sendPacket(const RawPacket& packet)`: This function will be called by a `Node` to send a packet. It will take a `RawPacket` as input and place it into our queue.
* `RawPacket receivePacket()`: This function will be called by a `Node` to retrieve a packet. It will take the first packet from the queue and return it.
* `bool hasPackets() const`: This simple function will let a `Node` check if there are any packets waiting in the queue before attempting to receive one.