#include "ChaCha20.h"
#include <fstream>
#include <iostream>

ChaCha20::ChaCha20(const std::vector<uint8_t> &key, const std::vector<uint8_t> &nonce) {
    if (key.size() != 32 || nonce.size() != 8) {
        throw std::invalid_argument("Invalid key or nonce length.");
    }
    this->key = key;
    this->nonce = nonce;
    this->counter = 0;
}

void ChaCha20::encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    std::vector<uint8_t> block(64);
    while (inFile.read(reinterpret_cast<char *>(block.data()), 64)) {
        encryptBlock(block);
        outFile.write(reinterpret_cast<char *>(block.data()), 64);
    }

    size_t remaining = inFile.gcount();
    if (remaining > 0) {
        inFile.read(reinterpret_cast<char *>(block.data()), remaining);
        encryptBlock(block);
        outFile.write(reinterpret_cast<char *>(block.data()), remaining);
    }

    inFile.close();
    outFile.close();
}


void ChaCha20::encryptBlock(std::vector<uint8_t> &block) {
    std::vector<uint32_t> state(16);
    state[0] = 0x61707865;
    state[1] = (key[0]) | (key[1] << 8) | (key[2] << 16) | (key[3] << 24);
    state[2] = (key[4]) | (key[5] << 8) | (key[6] << 16) | (key[7] << 24);
    state[3] = (key[8]) | (key[9] << 8) | (key[10] << 16) | (key[11] << 24);
    state[4] = (key[12]) | (key[13] << 8) | (key[14] << 16) | (key[15] << 24);
    state[5] = (key[16]) | (key[17] << 8) | (key[18] << 16) | (key[19] << 24);
    state[6] = (key[20]) | (key[21] << 8) | (key[22] << 16) | (key[23] << 24);
    state[7] = (key[24]) | (key[25] << 8) | (key[26] << 16) | (key[27] << 24);
    state[8] = (key[28]) | (key[29] << 8) | (key[30] << 16) | (key[31] << 24);
    state[9] = 0;
    state[10] = (nonce[0]) | (nonce[1] << 8) | (nonce[2] << 16) | (nonce[3] << 24);
    state[11] = (nonce[4]) | (nonce[5] << 8) | (nonce[6] << 16) | (nonce[7] << 24);
    state[12] = counter;
    state[13] = 0;
    state[14] = 0;
    state[15] = 0x00000001;

    std::vector<uint32_t> originalState = state;

    for (int round = 0; round < 10; ++round) {
        quarterRound(state, 0, 4, 8, 12);
        quarterRound(state, 1, 5, 9, 13);
        quarterRound(state, 2, 6, 10, 14);
        quarterRound(state, 3, 7, 11, 15);
        quarterRound(state, 0, 5, 10, 15);
        quarterRound(state, 1, 6, 11, 12);
        quarterRound(state, 2, 7, 8, 13);
        quarterRound(state, 3, 4, 9, 14);
    }

    for (size_t i = 0; i < state.size(); ++i) {
        state[i] += originalState[i];
    }

    for (size_t i = 0; i < state.size(); ++i) {
        block[i * 4] ^= state[i] & 0xFF;
        block[i * 4 + 1] ^= (state[i] >> 8) & 0xFF;
        block[i * 4 + 2] ^= (state[i] >> 16) & 0xFF;
        block[i * 4 + 3] ^= (state[i] >> 24) & 0xFF;
    }

    counter++;
}

void ChaCha20::quarterRound(std::vector<uint32_t> &state, int a, int b, int c, int d) {
    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = rotateLeft(state[d], 16);

    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = rotateLeft(state[b], 12);

    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = rotateLeft(state[d], 8);

    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = rotateLeft(state[b], 7);
}

uint32_t ChaCha20::rotateLeft(uint32_t value, int count) {
    return (value << count) | (value >> (32 - count));
}