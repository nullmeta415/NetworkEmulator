// Include the main header for EthernetFrame and MacUtil declarations.
// Always include your own header first to ensure it's self-contained and
// that all necessary declarations are present.
#include "EthernetFrame.h"

// Include necessary standard library headers that are used directly in this .cpp file.
// Even if some might be transitively included by EthernetFrame.h, explicitly including them
// here improves readability and ensures the file compiles even if dependencies change.
#include <sstream>  // For std::stringstream for string parsing and building.
#include <iomanip>  // For std::hex, std::uppercase, std::setfill, std::setw manipulators.
#include <iostream> // For std::cerr (standard error stream) for error messages.
#include <string>   // For std::string.
#include <array>    // For std::array (used in MacAddress).
#include <cstdint>  // For fixed-width integer types like uint16_t, uint8_t.

// =============================================================================
// Namespace: MacUtil
// Purpose: Contains utility functions specifically for MAC address manipulation
//          (conversion to/from string, and checksum calculation for data blocks).
// =============================================================================
namespace MacUtil {

    // -------------------------------------------------------------------------
    // Function: macToString
    // Purpose: Converts a MacAddress (std::array<unsigned char, 6>) into a
    //          human-readable, colon-separated hexadecimal string.
    //          Example: {0x00, 0x11, 0x22, 0xAA, 0xBB, 0xCC} -> "00:11:22:AA:BB:CC"
    // Parameters:
    //   - mac: A constant reference to the MacAddress array to be converted.
    //          'const&' ensures efficiency (no copy) and prevents modification.
    // Returns: A std::string representing the formatted MAC address.
    // -------------------------------------------------------------------------
    std::string macToString(const MacAddress& mac) {
        std::stringstream ss; // Create a stringstream object to build the string incrementally.

        // Set stream manipulators for hexadecimal, uppercase, and zero-filled output.
        // These manipulators are "sticky" for the stringstream, affecting subsequent outputs.
        ss << std::hex      // Output numbers in hexadecimal format.
           << std::uppercase // Use uppercase letters (A-F) for hex digits.
           << std::setfill('0'); // Fill empty width spaces with '0' (e.g., 'A' becomes '0A').

        // Loop through each of the 6 bytes in the MacAddress array.
        for (size_t i = 0; i < mac.size(); ++i) {
            // Set width to 2 for the current byte. std::setw(N) only applies to the NEXT output.
            // static_cast<int>(mac[i]) is crucial: it ensures unsigned char is treated as a number
            // for printing, not as an ASCII character (which might happen for printable ASCII values).
            ss << std::setw(2) << static_cast<int>(mac[i]);

            // Add a colon separator AFTER each byte, EXCEPT for the very last byte.
            // This prevents a trailing colon at the end of the MAC address string.
            if (i < mac.size() - 1) {
                ss << ':';
            }
        }

        return ss.str(); // Get the final built string from the stringstream.
    }

    // -------------------------------------------------------------------------
    // Function: stringToMac
    // Purpose: Converts a colon-separated hexadecimal MAC address string
    //          (e.g., "00:11:22:AA:BB:CC") into a MacAddress
    //          (std::array<unsigned char, 6>).
    //          Includes robust error handling for malformed input strings.
    // Parameters:
    //   - macStr: The input std::string representing the MAC address.
    // Returns: A MacAddress array. Returns an all-zeros MacAddress ({0x00...})
    //          on any parsing or validation error, and prints an error to std::cerr.
    // -------------------------------------------------------------------------
    MacAddress stringToMac(const std::string macStr) {
        MacAddress resultMac = {}; // Initialize all elements of the result MacAddress to 0x00 (all zeros).
                                   // This serves as the default return value on error.
        std::stringstream ss(macStr); // Load the input string into a stringstream for parsing.
        ss >> std::hex; // Set the stringstream to interpret numbers as hexadecimal during extraction. This is sticky.

        // Loop 6 times, once for each byte of the MAC address.
        for (int i = 0; i < MAC_ADDRESS_LENGTH; ++i) {
            unsigned int byteVal; // Temporary variable to read the hex value into.
                                  // Use unsigned int to safely read values from 0x00 to 0xFF,
                                  // as direct extraction into unsigned char can be problematic.

            // 1. Attempt to read the hex byte from the stringstream.
            // If reading fails (e.g., non-hex characters encountered, or string ends prematurely),
            // the stream's failbit will be set, and !(ss >> byteVal) will evaluate to true.
            if (!(ss >> byteVal)) {
                std::cerr << "Error: Failed to read hex byte at position " << i << ". Input: '" << macStr << "'" << std::endl;
                return {}; // Return a default-constructed (all-zeros) MacAddress on error.
            }

            // 2. Validate the byte value: ensure it's within the valid range for a byte (0-255).
            if (byteVal > 255) {
                std::cerr << "Error: Hex byte value out of range (0-255) at position " << i
                          << ". Value: 0x" << std::hex << byteVal << ". Input: '" << macStr << "'" << std::endl;
                return {}; // Return all-zeros MacAddress on error.
            }

            // Store the validated byte value (cast to unsigned char) into the result array.
            resultMac[i] = static_cast<unsigned char>(byteVal);

            // 3. If this is NOT the last byte (i.e., we expect a colon separator after it).
            if (i < MAC_ADDRESS_LENGTH - 1) {
                char colonChar; // Temporary variable to read the separator character.
                // Attempt to read the colon character.
                // If reading fails OR the character read is not a colon, it's an error.
                if (!(ss >> colonChar) || colonChar != ':') {
                    std::cerr << "Error: Missing or invalid colon separator at position " << i
                              << ". Input: '" << macStr << "'" << std::endl;
                    return {}; // Return all-zeros MacAddress on error.
                }
            }
        }

        // 4. After the loop, perform a final check for any unexpected leftover data in the stringstream.
        // If ss.good() is true, it means the stream is still in a valid state (no previous errors, not at EOF).
        // If we can then successfully read ANY character, it implies there's extra data in the input string.
        if (ss.good()) {
            char remainingChar;
            if (ss >> remainingChar) { // Attempt to read one more character.
                std::cerr << "Error: Extra characters found after MAC address. Input: '" << macStr << "'" << std::endl;
                return {}; // Return all-zeros MacAddress on error.
            }
        }
        // Note: If ss.good() was already false (e.g., due to a previous parsing failure or reaching EOF
        // exactly at the end of the MAC address), this final check block is correctly skipped.

        return resultMac; // If all parsing and validation checks pass, return the successfully parsed MacAddress.
    }

    // -------------------------------------------------------------------------
    // Function: calculateChecksum
    // Purpose: Calculates a simple sum checksum for a given vector of bytes.
    //          This is a basic error detection mechanism for our frames.
    // Parameters:
    //   - data: A constant reference to the vector of bytes for which to calculate the checksum.
    //           'const&' ensures efficiency and prevents modification.
    // Returns: A 16-bit unsigned integer (uint16_t) representing the calculated sum.
    // -------------------------------------------------------------------------
    uint16_t calculateChecksum(const std::vector<char>& data) {
        uint16_t sum = 0; // Initialize the sum to 0. uint16_t is chosen to prevent immediate overflow
                          // for typical small payloads, and to match the checksum field size.

        // Iterate through each byte in the input data vector.
        for (char byte : data) {
            // Crucial: Cast each 'char' to 'unsigned char' before adding it to the sum.
            // This prevents sign extension issues, where a 'char' with a value > 127
            // (e.g., 0xFF) might be interpreted as a negative number if 'char' is signed
            // on the specific system. 'unsigned char' guarantees values from 0-255.
            sum += static_cast<unsigned char>(byte);
        }
        return sum; // Return the final calculated sum.
    }

} // namespace MacUtil


/*
// MacAddress -> std::array<unsigned char, 6>

namespace MacUtil{
    std::string macToString(const MacAddress &mac){ 
        // mac = {0x00, 0x11, 0x22, 0xAA, 0xBB, 0xCC}
        std::stringstream ss ;
        for(int i=0; i<mac.size(); ++i){
            ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(mac[i]);
            if(i<mac.size()-1) ss << ':';
        }

        return ss.str();
    }

    MacAddress stringToMac(const std::string macStr){
        MacAddress resultMac = {};
        std::stringstream ss(macStr);
        ss >> std::hex;

        for(int i=0; i<6; i++){
            unsigned int byteVal;
            
            // Attempt to read the hex byte
            if(!(ss >> byteVal)){
                std::cerr << "Failed to read hex byte"<<std::endl; 
                return resultMac = {};
            }

            // Validate the byte value
            if(byteVal > 255){
                std::cerr << "Failed to read hex byte"<<std::endl; 
                return resultMac = {};
            }
            
            resultMac[i] = static_cast<unsigned char>(byteVal);

            // If this is NOT the last byte, attempt to read the colon seperator
            if(i<5){
                char colonChar;
                if(!(ss >> colonChar) || colonChar != ':'){
                    std::cerr << "Missing or invalid colon seperator"<<std::endl; 
                    return resultMac = {};
                }
            }
        }
        
        // After the loop, check if there's any unexpected leftover data in the string.
        // If the stream is not at its end-of-file, it means there's extra data.

        if(ss.good()){
            char remainingChar;
            if(ss >> remainingChar){
                std::cerr << "Extra characters found after MAC address"<<std::endl;
                return resultMac = {};
            }
        }

        return resultMac;
    }

    uint16_t calculateChecksum(const std::vector<char> &data){
        uint16_t sum = 0;
        for(char byte: data){
            sum += static_cast<unsigned char>(byte);
        }
        return sum;
    }
}
*/