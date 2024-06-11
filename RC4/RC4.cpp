#include "RC4.h"
#include <fstream>
#include <iostream>

RC4::RC4(const std::vector<uint8_t> &key) : state(256), i(0), j(0) {
    if (key.empty() || key.size() > 256) {
        throw std::invalid_argument("Invalid key length.");
    }

    for (int i = 0; i < 256; ++i) {
        state[i] = i;
    }

    int j = 0;
    for (int i = 0; i < 256; ++i) {
        j = (j + state[i] + key[i % key.size()]) % 256;
        std::swap(state[i], state[j]);
    }
}

void RC4::encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    uint8_t byte;
    while (inFile.read(reinterpret_cast<char *>(&byte), 1)) {
        byte ^= generateKeystreamByte();
        outFile.write(reinterpret_cast<char *>(&byte), 1);
    }

    inFile.close();
    outFile.close();
}

uint8_t RC4::generateKeystreamByte() {
    i = (i + 1) % 256;
    j = (j + state[i]) % 256;
    std::swap(state[i], state[j]);
    return state[(state[i] + state[j]) % 256];
}
