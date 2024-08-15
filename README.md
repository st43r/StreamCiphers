# StreamCiphers

## Overview

This project implements four different stream ciphers in C++:

- **ChaCha20**: A modern, secure stream cipher designed for high-speed performance.
- **Grain128**: A lightweight stream cipher suitable for constrained environments.
- **CryptMT**: A cryptographically secure pseudo-random number generator based on the Mersenne Twister algorithm.
- **RC4**: A widely known but now deprecated stream cipher.

The project is built using CMake and is designed to provide an educational overview and comparison of these cryptographic algorithms.

## Requirements

- CMake 3.10 or higher
- A C++11 compatible compiler

## Building the Project

To build the project, follow these steps:

1. **Clone the Repository**

   ```bash
   git clone https://github.com/st43r/StreamCiphers.git
   cd StreamCiphers
   ```

2. **Create a Build Directory**

   ```bash
   mkdir build
   cd build
   ```

3. **Run CMake**

   ```bash
   cmake ..
   ```

4. **Build the Project**

   ```bash
   make
   ```

   This will produce an executable named `StreamCiphers`.

## Usage

To use the `StreamCiphers` executable, you can run it from the command line. The exact usage will depend on the implementation details of `main.cpp`, which is designed to demonstrate or test the functionality of the stream ciphers.

Example:

```bash
./StreamCiphers
```

## File Structure

- `CMakeLists.txt`: CMake configuration file for building the project.
- `main.cpp`: The main file that drives the execution of the stream cipher implementations and tests ciphers on the greeting text file.
- `Grain128/`: Contains source and header files for the Grain128 cipher.
- `ChaCha20/`: Contains source and header files for the ChaCha20 cipher.
- `RC4/`: Contains source and header files for the RC4 cipher.
- `CryptMT/`: Contains source and header files for the CryptMT cipher.
- `StreamCipher/`: Contains the interface for the stream ciphers.