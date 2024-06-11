#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

#include "StreamCipher/StreamCipher.h"
#include "Grain128/Grain128.h"
#include "ChaCha20/ChaCha20.h"
#include "RC4/RC4.h"
#include "CryptMT/CryptMT.h"

/**
 * Generates random byte.
 * @return 1 byte.
 */
uint8_t generateRandomByte() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    return static_cast<uint8_t>(dis(gen));
}

/**
 * Generates file with random bytes.
 * @param filename - path to file.
 * @param size of file you want to create.
 */
void generateRandomFile(const std::string &filename, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        uint8_t byte = generateRandomByte();
        file.write(reinterpret_cast<char *>(&byte), sizeof(byte));
    }

    file.close();
}

int main() {
    generateRandomFile("input.txt", pow(2, rand() % 16));

    std::vector<uint8_t> key(16, 0x01);
    std::vector<uint8_t> iv(12, 0x02);

    Grain128 grain128(key, iv);
    grain128.encryptDecryptFile("input.txt", "output_grain128.txt");

    std::vector<uint8_t> chachaKey(32, 0x01);
    std::vector<uint8_t> nonce(8, 0x02);
    ChaCha20 chacha20(chachaKey, nonce);
    chacha20.encryptDecryptFile("input.txt", "output_chacha20.txt");

    RC4 rc4(key);
    rc4.encryptDecryptFile("input.txt", "output_rc4.txt");

    std::vector<uint32_t> cryptmtKey = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    CryptMT cryptMT(cryptmtKey);
    cryptMT.encryptDecryptFile("input.txt", "output_cryptmt.txt");

    return 0;
}