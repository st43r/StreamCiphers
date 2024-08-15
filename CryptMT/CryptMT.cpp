#include "CryptMT.h"
#include <fstream>
#include <iostream>
#include <cstring> // For std::memcpy

static const uint32_t MATRIX_A = 0x9908B0DF;
static const uint32_t UPPER_MASK = 0x80000000;
static const uint32_t LOWER_MASK = 0x7FFFFFFF;
static const int N = 624;
static const int M = 397;
static const int W = 32;
static const int R = 31;
static const int U = 11;
static const int S = 7;
static const int B = 0x9D2C5680;
static const int T = 0xEFC60000;
static const int C = 0xBF800000;
static const int L = 18;

CryptMT::CryptMT(const std::vector<uint32_t> &key) : state(N), index(N + 1) {
    initialize(key);
}

void CryptMT::initialize(const std::vector<uint32_t> &key) {
    std::vector<uint32_t> initKey = key;
    state[0] = 0x012BD6F1; // Arbitrary seed value
    for (size_t i = 1; i < N; ++i) {
        state[i] = 1812433253UL * (state[i - 1] ^ (state[i - 1] >> (W - 2))) + i;
    }
    for (size_t i = 0; i < N; ++i) {
        state[i] ^= initKey[i % initKey.size()];
    }
    index = N;
}

uint8_t CryptMT::generateKeystreamByte() {
    if (index >= N) {
        // Generate new state
        for (size_t i = 0; i < N - M; ++i) {
            uint32_t y = (state[i] & UPPER_MASK) | (state[i + 1] & LOWER_MASK);
            state[i] = state[i + M] ^ (y >> 1) ^ (y & 1 ? MATRIX_A : 0);
        }
        for (size_t i = N - M; i < N - 1; ++i) {
            uint32_t y = (state[i] & UPPER_MASK) | (state[i + 1] & LOWER_MASK);
            state[i] = state[i + (M - N)] ^ (y >> 1) ^ (y & 1 ? MATRIX_A : 0);
        }
        uint32_t y = (state[N - 1] & UPPER_MASK) | (state[0] & LOWER_MASK);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ (y & 1 ? MATRIX_A : 0);
        index = 0;
    }
    uint32_t y = state[index++];
    y ^= (y >> U) & B;
    y ^= (y << S) & T;
    y ^= (y << L) & C;
    y ^= (y >> R);
    return static_cast<uint8_t>(y);
}

void CryptMT::encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    char byte;
    while (inFile.get(byte)) {
        byte ^= generateKeystreamByte();
        outFile.put(byte);
    }

    inFile.close();
    outFile.close();
}
