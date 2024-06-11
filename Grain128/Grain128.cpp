#include "Grain128.h"
#include <fstream>
#include <iostream>

Grain128::Grain128(const std::vector<uint8_t> &key, const std::vector<uint8_t> &iv) : key(key), iv(iv), counter(0) {
    for (auto &i: s) {
        state.push_back(0);
        for (unsigned char &j: i) {
            j = 0;
        }
    }

    for (uint32_t i = 0; i < key.size(); ++i) {
        s[i / 4][i % 4] = key[i];
    }

    for (uint32_t i = 0; i < iv.size(); ++i) {
        s[(i + 8) / 4][(i + 8) % 4] = iv[i];
    }

    for (auto &i: s) {
        i[8] = 1;
    }

    for (uint32_t i = 0; i < 16; ++i) {
        generateKeystream();
    }
}

void Grain128::encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    char byte;
    while (inFile.get(byte)) {
        byte ^= output[counter % 16];
        ++counter;

        outFile.put(byte);
    }

    inFile.close();
    outFile.close();
}

void Grain128::generateKeystream() {
    for (unsigned char &r: output) {
        r = 0;

        for (uint32_t m = 0; m < 8; ++m) {
            uint8_t feedback = state[15] & 1;

            for (uint32_t n = 0; n < 15; ++n) {
                state[n] = state[n + 1];
            }

            state[15] = (r + feedback) ^ (state[15] >> 1) ^ s[15][counter & 0xf] ^ s[14][(counter >> 4) & 0xf]
                        ^ s[13][(counter >> 8) & 0xf] ^ s[12][(counter >> 12) & 0xf] ^ s[8][(counter >> 16) & 0xf]
                        ^ s[5][(counter >> 20) & 0xf] ^ s[2][(counter >> 24) & 0xf] ^ s[0][(counter >> 28) & 0xf] ^
                        iv[m];
        }

        r = state[15];
        ++counter;
    }
}