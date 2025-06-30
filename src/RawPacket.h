#ifndef RAW_PACKET_H
#define RAW_PACKET_H

#include <vector>
#include <string> // to store string data directly

// Represents the raw data being transmitted.
// In this phase, it's just a simple container for bytes.
class RawPacket{
    public:
    std::vector<char> data; // Stores the raw byte data

    RawPacket() = default; // Default constructor
    RawPacket(const std::string &message); // Constructor from a string
    RawPacket(const std::vector<char> &raw_data); // Constructor from vector<char>

    std::string toString() const; // Convert data to string (for display)
    size_t size() const; // Get size of the data

    // A way to get the raw data directly for lower layers
    const std::vector<char> &getRawData() const { return data; }
};

#endif // RAW_PACKET_H

/*
Let's dive deep into the `RawPacket` class.

-----

### **Purpose of the `RawPacket` Class**

In Phase 1, the `RawPacket` class serves a very fundamental purpose:

**It acts as a simple, generic container for raw, uninterpreted byte data that is intended to be sent across our simulated network medium.**

Think of it as a **plain, unaddressed envelope**. It's just a way to hold the content you want to send. It doesn't know who it's from, who it's for, or what type of data it contains (text, image, etc.). Its only job is to carry bytes.

**Why is this important in a network emulator?**

1.  **Abstraction:** It abstracts away the specific content. Whether you're sending a chat message, a file chunk, or a future network header, it all boils down to bytes at the lower layers. `RawPacket` represents these raw bytes.
2.  **Foundation for Encapsulation:** In later phases, when we add headers (Data Link, Network, Transport), these headers will also be just sequences of bytes. A `RawPacket` can then hold the *entire* encapsulated data, headers included. This sets us up for the crucial concept of **encapsulation** in networking.
3.  **Simplicity for Phase 1:** By starting with a `RawPacket`, we keep the initial communication as simple as possible. We don't complicate things with parsing, addressing, or error checking right away.

-----

### **Line-by-Line Explanation of `RawPacket.h`**

Let's break down the header file `src/RawPacket.h`:


**Explanation:**

1.  `#ifndef RAW_PACKET_H`: This is the start of an **include guard**. It's a preprocessor directive that checks if `RAW_PACKET_H` (a macro name) has *not* been defined yet.

2.  `#define RAW_PACKET_H`: If `RAW_PACKET_H` was not defined (meaning this file hasn't been included before), this line defines it.
      * **Purpose of Include Guards:** Prevents the contents of this header file from being included multiple times in the same compilation unit. Multiple inclusions can lead to redefinition errors for classes, functions, and variables, causing compilation failures.

3.  `#include <vector>`: This line includes the standard C++ header for `std::vector`.
      * **Purpose:** `std::vector` is a dynamic array template that we'll use to store the raw byte data of our packet. It's flexible (can grow/shrink) and efficient.

4.  `#include <string>`: This line includes the standard C++ header for `std::string`.
      * **Purpose:** Although we primarily use `std::vector<char>` for raw data, we include `<string>` because we want to allow `RawPacket` to be constructed directly from a `std::string` (for convenience in Phase 1, as we're sending text messages). We also use it in `toString()` for display.

5.  `// Represents the raw data being transmitted. ...`: These are **comments**.
      * **Purpose:** To explain the overall purpose of the `RawPacket` class to anyone reading the code (including your future self\!). Good comments are vital for maintainability and understanding.

6.  `class RawPacket { ... };`: This declares the `RawPacket` **class**.
      * **Purpose:** Classes are the fundamental building blocks in Object-Oriented Programming (OOP). They act as blueprints for creating objects. This class defines the structure and behavior of our "raw packet" objects.

7.  `public:`: This is an **access specifier**.
      * **Purpose:** Members declared under `public:` can be accessed from *anywhere* outside the class. We make our members public because, in this very basic phase, we want easy access to the `data` and simple methods. In more advanced phases, you might make `data` private and provide controlled access.

8.  `std::vector<char> data;`: This declares a **public member variable** named `data` of type `std::vector<char>`.
      * **Purpose:** This is the core storage for the raw bytes of the packet. `std::vector<char>` is chosen because it can hold any sequence of bytes (characters are just bytes in C++), and its size can change dynamically, which is perfect for variable-length messages or packets with different headers later.

9.  `RawPacket() = default;`: This declares a **default constructor**.
      * **Purpose:** `  = default; ` explicitly tells the compiler to generate the default constructor. This constructor takes no arguments and initializes the `data` member (an empty `std::vector<char>` in this case). This allows us to create `RawPacket` objects without providing initial data, which is useful for situations like `RawPacket receivedPacket = medium.receivePacket(nodeId);` when `receivePacket` might return an empty packet.

10. `RawPacket(const std::string& message);`: This declares a **constructor** that takes a `const std::string&` as an argument.
      * **Purpose:** This allows us to easily create a `RawPacket` object directly from a `std::string`. For example, `RawPacket p("Hello");` is now possible. The `const` indicates the string won't be modified, and `&` means it's passed by reference (efficient, avoids copying large strings).

11. `RawPacket(const std::vector<char>& raw_data);`: This declares another **constructor** that takes a `const std::vector<char>&` as an argument.
      * **Purpose:** This provides a way to construct a `RawPacket` directly from existing raw byte data, which will become more useful when lower layers pass up byte arrays that represent their payloads.

12. `std::string toString() const;`: This declares a **public member function (method)** named `toString()`.
      * **Purpose:** This utility function converts the raw byte data stored in `data` back into a `std::string` for easy human-readable output (especially important for debugging in Phase 1). `const` at the end means this method does not modify the object's state.

13. `size_t size() const;`: This declares another **public member function** named `size()`.
      * **Purpose:** Returns the number of bytes (characters) currently stored in the `data` vector. `size_t` is the appropriate unsigned integer type for sizes and counts. `const` indicates it doesn't modify the object.

14. `// You might want a way to get the raw data directly for lower layers`: This is a **comment**.
      * **Purpose:** A forward-looking hint. In later phases, when different layers manipulate the raw bytes, they might need direct access to the `std::vector<char>` itself, not just a string representation.

15. `const std::vector<char>& getRawData() const { return data; }`: This declares a **public member function** named `getRawData()`.
      * **Purpose:** Provides read-only access to the internal `data` vector by returning a `const` reference. This is more efficient than returning a copy, especially for large packets, and the `const` ensures the caller cannot modify the internal data directly.

16. `#endif // RAW_PACKET_H`: This is the end of the **include guard**.

-----

*/