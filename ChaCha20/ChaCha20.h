#ifndef CHACHA20_H
#define CHACHA20_H

#include "StreamCipher.h"

class ChaCha20 : public StreamCipher {
public:
    ChaCha20(const std::vector<uint8_t> &key, const std::vector<uint8_t> &nonce);

    void encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) override;

private:
    std::vector<uint8_t> key;
    std::vector<uint8_t> nonce;
    uint64_t counter;

    void encryptBlock(std::vector<uint8_t> &block);

    static void quarterRound(std::vector<uint32_t> &state, int a, int b, int c, int d);

    static uint32_t rotateLeft(uint32_t value, int count);
};

#endif // CHACHA20_H
