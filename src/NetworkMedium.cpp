#include "NetworkMedium.h"
#include <iostream>

// Definition of the static constant member
const RawPacket NetworkMedium::EMPTY_PACKET;

// Puts a packet onto the medium
void NetworkMedium::sendPacket(const RawPacket& packet) {
    packetQueue.push(packet);
}

// Takes a packet off the medium and returns it
RawPacket NetworkMedium::receivePacket() {
    if (!packetQueue.empty()) {
        RawPacket packet = packetQueue.front();
        packetQueue.pop();
        return packet;
    }
    // Return the empty packet if no packet is available
    return EMPTY_PACKET;
}

// Checks if there are any packets waiting on the medium
bool NetworkMedium::hasPackets() const {
    return !packetQueue.empty();
}