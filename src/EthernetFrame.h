#ifndef ETHERNET_FRAME_H
#define ETHERNET_FRAME_H

#include<string> // For std::string to hold message data
#include<vector> // For std::vector<char> to hold payload and serialized data
#include<array> // For std::array<unsigned char, 6> to represent MAC addresses
#include<cstdint> // For fixed-width interger types like uin16_t, uint64_t
#include<iomanip> // For std::hex, std::setfill, std::setw for fromatting MAC addresses in strings
#include<sstream> // For std::stringstream to build MAC address strings

// Define MAC_ADDRESS_LENGTH for clarity and easy modification
const size_t MAC_ADDRESS_LENGTH = 6; // Standard MAC address length in bytes

// Type alias for MAC address, representing 6 bytes.
using MacAddress = std::array<unsigned char, MAC_ADDRESS_LENGTH>;

// Namespace for MAC address utility functions.
// This helps organize helper funtions related to MAC addresses.
namespace MacUtil{
    // Converts a string MAC address (e.g., "00:11:22:AA:BB:CC").
    std::string macToString(const MacAddress &mac);

    // Converts a hex MAC address (e.g., {0x11, 0x22, 0x33, 0xAA, 0xBB, 0xCC})
    MacAddress stringToMac(const std::string macStr);

    // Simple checksum calculation for a given vector of bytes.
    // This will be used for error detection in the EthernetFrame.
    uint16_t calculateChecksum(const std::vector<char> &data);
}

// The EthernerFrame class represents a single data unit at the Data Link Layer.
// It includes common Ethernet II header fields and a basic checksum.
class EthernetFrame{
public:
    // Ethernet II header fields (simplified for simulation):
    MacAddress destMac;     // 6 bytes: Destination Media Access Control address.
    
    MacAddress srcMac;      // 6 bytes: Source Media Access Control address.
    
    uint16_t payloadLength; // 2 bytes: Lenght of the payload. In real Ethernet II, this is EtherType,
                            // but we'll use it for payload length for simplicity in the phase.
    
    std::vector<char> payload; // Variable lenght: The actual data carried by the frame.
                               // This will hold the application message (string).
    
    uint16_t checksum;         // 2 bytes: Simple sum checksum for basic error datection.

    // Constructor to build an EthernetFrame from destination/source MACs and a string payload.
    EthernetFrame(const MacAddress &dest, const MacAddress &src, const std::string &data_payload);

    EthernetFrame() = default; // Default constructor: Needed for deserialization to create an empty object.

    // Serialization method: Converts the EthernetFrame object into raw byte vector.
    // This byte vector can then be stored directly in a RawPacket for transmission.

    std::vector<char> serialize() const;

    // Deserialization static method: Reconstructs an EthernetFrame object
    // from a raw byte vector received from the NetworkMedium.
    static EthernetFrame deserialize(const std::vector<char> &raw_data);

    // Verification method: Checks the frame's checksum to detect errors.
    bool verifyChecksum() const;

    // Getter methods for accessing the private members of the frame.
    // 'const' indicates these methods do not modify the object's state.
    const MacAddress &getDestMac() const {return destMac;}

    const MacAddress &getSrcMac() const {return srcMac;}

    uint16_t getPayLoadLength() const {return payloadLength;}

    const std::vector<char> &getPayLoad () const {return payload;}

    std::string getPayLoadString() const; // Helper to convert payload vector to string.

    uint16_t getChecksum() const {return checksum;}

    // Utility method: Provides a string representation of the frame for debugging/logging.
    std::string toString() const;
};

#endif // ETHERNET_FRAME_H

/*
Let's define the structure for our **`EthernetFrame`** class. This class will represent the Data Link Layer's Protocol Data Unit (PDU), encapsulating our application data and adding necessary fields for local network communication.

We'll include utility functions for MAC address handling and a simple checksum directly in this header for convenience in Phase 2.

-------------------------------------------

### **Line-by-Line Explanation of `src/EthernetFrame.h`**

1.  `#ifndef ETHERNET_FRAME_H`: This is the **start of an include guard**. It checks if the `ETHERNET_FRAME_H` macro has *not* been defined.

      * **Why:** Prevents the contents of this header file from being included multiple times in the same compilation unit, which would lead to redefinition errors.

2.  `#define ETHERNET_FRAME_H`: If the macro was not defined, this line **defines it**.

      * **Why:** Marks that this header has now been processed, so subsequent attempts to include it will skip its content.

3.  `#include <string>`: Includes the C++ Standard Library header for `std::string`.

      * **Why:** We'll be working with `std::string` for the application-level message payload.

4.  `#include <vector>`: Includes the C++ Standard Library header for `std::vector`.

      * **Why:** `std::vector<char>` is used to store the variable-length payload and for the raw byte representation during serialization/deserialization.

5.  `#include <array>`: Includes the C++ Standard Library header for `std::array`.

      * **Why:** `std::array<unsigned char, 6>` is chosen for a fixed-size, stack-allocated array to represent MAC addresses more realistically than a simple integer type.

6.  `#include <cstdint>`: Includes the C++ Standard Library header for fixed-width integer types.

      * **Why:** Provides types like `uint16_t` (unsigned 16-bit integer) for payload length and checksum, and potentially `uint64_t` for future MAC address manipulation, ensuring consistent byte sizes across different systems.

7.  `#include <iomanip>`: Includes the C++ Standard Library header for stream manipulators.

      * **Why:** `std::hex`, `std::setfill`, and `std::setw` are useful for formatting MAC addresses into a standard colon-separated string representation for display.

8.  `#include <sstream>`: Includes the C++ Standard Library header for `std::stringstream`.

      * **Why:** Used to build strings programmatically, particularly helpful for converting MAC address byte arrays to formatted strings and vice-versa.

9.  `const size_t MAC_ADDRESS_LENGTH = 6;`: Defines a constant for the standard length of a MAC address.

      * **Why:** Improves readability, maintainability, and makes the code less prone to "magic numbers."

10. `using MacAddress = std::array<unsigned char, MAC_ADDRESS_LENGTH>;`: Creates a **type alias** `MacAddress`.

      * **Why:** Makes the code more readable and semantically clear. Instead of `std::array<unsigned char, 6>` everywhere, we can just use `MacAddress` when referring to MAC addresses.

11. `namespace MacUtil { ... }`: Defines a **namespace** called `MacUtil`.

      * **Why:** Organizes utility functions related to MAC addresses, preventing name collisions with other parts of the code and making it clear these functions belong together.

12. `MacAddress stringToMac(const std::string& macStr);`: Declares a function to convert a string (e.g., "AA:BB:CC:DD:EE:FF") to a `MacAddress` array.

      * **Why:** Provides a convenient way to initialize `MacAddress` objects from human-readable strings, which is common in networking.

13. `std::string macToString(const MacAddress& mac);`: Declares a function to convert a `MacAddress` array back to a formatted string.

      * **Why:** Essential for displaying MAC addresses in logs and for debugging.

14. `uint16_t calculateChecksum(const std::vector<char>& data);`: Declares a function to calculate a simple checksum over a vector of characters.

      * **Why:** This will be our basic error detection mechanism for frames.

15. `}`: Closes the `MacUtil` namespace.

16. `class EthernetFrame { ... };`: Declares the `EthernetFrame` class.

      * **Why:** This is the core data unit for our Data Link Layer.

17. `public:`: Specifies that the following members are publicly accessible.

      * **Why:** These are the fields and methods that other parts of the program (like `DataLinkLayer`) will need to interact with.

18. `MacAddress destMac;`: Declares a `MacAddress` object to store the **destination MAC address**.

      * **Why:** A fundamental part of an Ethernet header, specifying who the frame is intended for on the local network.

19. `MacAddress srcMac;`: Declares a `MacAddress` object to store the **source MAC address**.

      * **Why:** Identifies the sender of the frame.

20. `uint16_t payloadLength;`: Declares a 16-bit unsigned integer to store the **length of the payload**.

      * **Why:** In real Ethernet II, this field is usually "EtherType" (indicating the higher-layer protocol). For simplicity in our simulation, we'll use it to explicitly store the length of our variable-size payload, which helps during deserialization.

21. `std::vector<char> payload;`: Declares a `std::vector<char>` to hold the **actual data** carried by the frame.

      * **Why:** This is where the application-level message (from the `Node`) will be placed after being encapsulated by the `DataLinkLayer`. It's `vector<char>` to represent raw bytes.

22. `uint16_t checksum;`: Declares a 16-bit unsigned integer to store the **checksum**.

      * **Why:** This field will hold a value calculated from the frame's data, used by the receiver to detect if the frame was corrupted during transmission.

23. `EthernetFrame(const MacAddress& dest, const MacAddress& src, const std::string& data_payload);`: Declares the primary **constructor** for `EthernetFrame`.

      * **Why:** Allows easy creation of a frame by providing the necessary destination, source, and data payload.

24. `EthernetFrame() = default;`: Declares a **default constructor**.

      * **Why:** This is crucial because when we deserialize raw bytes into an `EthernetFrame` object, we first need an empty `EthernetFrame` object to reconstruct. `default` asks the compiler to generate a simple, no-argument constructor.

25. `std::vector<char> serialize() const;`: Declares the **`serialize` method**.

      * **Why:** This method is vital. It converts the structured `EthernetFrame` object (with its distinct fields) into a flat `std::vector<char>` (a sequence of bytes) suitable for putting into a `RawPacket` and sending over the `NetworkMedium`. `const` means it doesn't change the frame.

26. `static EthernetFrame deserialize(const std::vector<char>& raw_data);`: Declares the **`deserialize` static method**.

      * **Why:** This method is equally vital for reception. It takes the raw `std::vector<char>` data (which came from a `RawPacket`) and reconstructs it into a meaningful `EthernetFrame` object. It's `static` because you call it on the class itself (e.g., `EthernetFrame::deserialize(...)`) to *create* an `EthernetFrame` instance, rather than needing an existing instance to call it on.

27. `bool verifyChecksum() const;`: Declares the **`verifyChecksum` method**.

      * **Why:** The receiver will call this method on a deserialized frame to check for data integrity.

28. `const MacAddress& getDestMac() const { return destMac; }`: A **getter** method for `destMac`.

      * **Why:** Provides controlled read-only access to the private `destMac` member. `const&` makes it efficient.

29. `const MacAddress& getSrcMac() const { return srcMac; }`: Getter for `srcMac`.

30. `uint16_t getPayloadLength() const { return payloadLength; }`: Getter for `payloadLength`.

31. `const std::vector<char>& getPayload() const { return payload; }`: Getter for `payload`.

32. `std::string getPayloadString() const;`: Declares a helper getter to convert the `payload` (which is `std::vector<char>`) into a `std::string`.

      * **Why:** The `Node` and application level still primarily work with `std::string` messages.

33. `uint16_t getChecksum() const { return checksum; }`: Getter for `checksum`.

34. `std::string toString() const;`: Declares a `toString` utility method.

      * **Why:** Essential for debugging and logging, providing a human-readable representation of the entire frame's contents.

35. `};`: Closes the `EthernetFrame` class declaration.

36. `#endif // ETHERNET_FRAME_H`: Ends the include guard.

-----

This `EthernetFrame.h` file sets up the blueprint for our Data Link Layer's data unit.
*/