# Mega256

A lightweight, high-performance C++20 library implementing a custom **256-bit signed integer type** (`Mega256`) from scratch. This library bypasses the storage limitations of standard fixed-width primitives (`int64_t`, `long long`) by combining an internal static array of blocks with an explicit sign representation into a single cryptographic-sized integer capable of storing positive and negative values up to approx. 78 decimal digits.

## Features

- **Signed Representation**: Supports both positive and negative integers out-of-the-box.
- **Full Operator Overloading**: Supports arithmetic operators (`+`, `-`, `*`, `/`, `%`) and compound assignments (`+=`, `-=`, `*=`, `/=`, `%=`).
- **Increment/Decrement**: Implements prefix and postfix `++` and `--`.
- **Comparison Operators**: Full support for standard boolean checks (`==`, `!=`, `<`, `>`, `<=`, `>=`).
- **Flexible Constructors**: Initialize variables using native `int64_t` / `uint64_t` numbers or via huge decimal numeric strings (with optional `-` or `+` signs) wrapped in standard `std::string` objects.
- **Double Base Formatting**: Output and debug results seamlessly in either raw signed Hexadecimal (`-0x...`) or localized base-10 strings (`toDecString()`).
- **Zero Dynamic Allocations**: Utilizing stack-allocated `std::array` layout to maximize throughput, avoid memory fragmentation, and maintain low cache-miss ratios during hardware execution.

## Getting Started

### Project Structure
```text
Mega256/
├── include/
│   └── Mega256.h
├── src/
│   └── Mega256.cpp
├── main.cpp
└── CMakeLists.txt
```

### Installation with CMake
Integrate Mega256 into your project tree by mapping your `CMakeLists.txt` file as follows:

```cmake
cmake_minimum_required(VERSION 3.20)
project(YourProject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(YourProject main.cpp src/Mega256.cpp)
```

## Usage Example

```cpp
#include <iostream>
#include "Mega256.h"

int main() {
    // Initialization from a huge signed decimal string
    Mega256 hugeNegative("-18446744073709551616"); // -2^64
    
    // Arithmetic operations with mixed signs
    Mega256 incremented = hugeNegative + Mega256(1);
    
    std::cout << "Decimal:     " << incremented.toDecString() << std::endl;
    std::cout << "Hexadecimal: "; incremented.print();
    
    // Subtraction triggering sign changes
    Mega256 a = 5000;
    Mega256 b = 10000;
    Mega256 negativeResult = a - b;
    
    std::cout << "Result:      " << negativeResult.toDecString() << std::endl; // Output: -5000

    return 0;
}
```

## Specifications & Performance
- **Capacity**: Holds numbers ranging from negative `MAX_VALUE` up to positive `MAX_VALUE` boundaries.
- **Architecture**: Leverages architectural GCC/Clang built-in `__uint128_t` hardware extensions internally to execute operations via optimal 64-bit native register pipeline sequences.