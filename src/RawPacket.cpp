#include "RawPacket.h"
#include <iostream> // For debugging or string conversion later

// Calling the RawPacket() constructor of RawPacket class from RawPacket file.
RawPacket::RawPacket(const std::string &message) {
    // Copy the string characters into the vector<char>
    data.assign(message.begin(), message.end());
}

RawPacket::RawPacket(const std::vector<char> &raw_data) : data(raw_data){
    // Simple copy constructor
}

std::string RawPacket::toString() const {
    // Convert vector<char> back to string for easy display
    // Note: This assumes the data is text. Will change for binary data later.
    return std::string(data.begin(), data.end());
}

size_t RawPacket::size() const {
    return data.size();
}

/*
### **Line-by-Line Explanation of `RawPacket.cpp`**

Now, let's look at the implementation file `src/RawPacket.cpp`:


**Explanation:**

1.  `#include "RawPacket.h"`: This line includes the header file for `RawPacket`.

      * **Purpose:** To make the declarations (class definition, method prototypes) from `RawPacket.h` available to this `.cpp` file so that we can *implement* those methods.

2.  `#include <iostream>`: This line includes the standard C++ header for `std::cout`, etc.

      * **Purpose:** Although not directly used in *these specific* `RawPacket` method implementations, it's often useful to have for debugging `std::cout` statements. I put it here for completeness, though for `RawPacket` specifically, it could be omitted if no `cout` is used directly in its methods.

3.  `RawPacket::RawPacket(const std::string& message) { ... }`: This is the **definition** of the constructor that takes a `std::string`.

      * **Purpose:** Implements the behavior of this specific constructor, which is to initialize the `RawPacket`'s internal `data` vector from the input string. `RawPacket::` syntax indicates that this is a member of the `RawPacket` class.

4.  `// Copy the string characters into the vector<char>`: A **comment**.

      * **Purpose:** Explains the intent of the following line.

5.  `data.assign(message.begin(), message.end());`: This line uses the `assign` method of `std::vector`.

      * **Purpose:** It copies the characters from the beginning (`message.begin()`) to the end (`message.end()`) of the `message` string into the `data` vector. This effectively converts the `std::string` into a `std::vector<char>`.

6.  `}`: Closes the constructor's definition.

7.  `RawPacket::RawPacket(const std::vector<char>& raw_data) : data(raw_data) { ... }`: This is the **definition** of the constructor that takes a `std::vector<char>`.

      * **Purpose:** Implements the behavior for creating a `RawPacket` directly from another `std::vector<char>`.

8.  `// Simple copy constructor (member initializer list automatically copies data)`: A **comment**.

      * **Purpose:** Explains that the `data(raw_data)` part is a **member initializer list**. This is the preferred and most efficient way to initialize member variables in constructors. It directly calls the `std::vector`'s copy constructor, avoiding a temporary default construction followed by an assignment.

9.  `}`: Closes the constructor's definition. (The body is empty because the initializer list does all the work).

10. `std::string RawPacket::toString() const { ... }`: This is the **definition** of the `toString()` method.

      * **Purpose:** Implements the logic to convert the packet's raw data back into a readable string.

11. `// Convert vector<char> back to string for easy display ...`: A **comment**.

      * **Purpose:** Explains the conversion and notes a limitation (assuming text data).

12. `return std::string(data.begin(), data.end());`: This line creates a new `std::string` object.

      * **Purpose:** It uses a `std::string` constructor that takes two iterators (start and end). This copies the characters from the `data` vector into the new string, effectively converting the `std::vector<char>` back to a `std::string`.

13. `}`: Closes the method's definition.

14. `size_t RawPacket::size() const { ... }`: This is the **definition** of the `size()` method.

      * **Purpose:** Implements the logic to return the size of the packet's data.

15. `return data.size();`: This directly calls the `size()` method of the internal `std::vector<char> data`.

      * **Purpose:** To get the number of elements (bytes) currently stored in the `data` vector.

16. `}`: Closes the method's definition.

-----

*/