#ifndef NETWORK_MEDIUM_H    // This will ensure no repeat definition of this header file.
#define NETWORK_MEDIUM_H    // If the header file is not defined then only define it for use.

// Including the necessary header files
#include <vector>
#include <queue>

/*
RawPacket is a simple container for raw bytes.
It is the basic unit of data that the NetworkMedium can transport.
*/
class RawPacket{
public:
    std::vector<char> data; //Sequence of data that will travel on the network medium.
};

/*
The NetworkMedium class simulates a shared physical communication channel where all nodes can send and receive RawPackets.
*/

// A constant to represent an empty or invalid packet.
// Used by receivePacket() to signal that no packet was available.
static const RawPacket EMPTY_PACKET;

class NetworkMedium{
private:
    std::queue<RawPacket> packetQueue;  // Packets are added to the back and removed from the front.

public:
    // Puts a packet onto the medium (adds to the queue).
    void sendPacket(const RawPacket& packet);

    // Takes a packet off the medium (removes from the queue) and returns it.
    RawPacket receivePacket();

    // Checks if there are any packets waiting on the medium.
    bool hasPackets() const;
};


#endif  // End NETWORK_MEDIUM_H