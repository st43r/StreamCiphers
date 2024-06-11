#ifndef GRAIN128_H
#define GRAIN128_H

#include "StreamCipher.h"

class Grain128 : public StreamCipher {
public:
    Grain128(const std::vector<uint8_t> &key, const std::vector<uint8_t> &iv);

    void encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) override;

private:
    std::vector<uint8_t> key;
    std::vector<uint8_t> iv;
    std::vector<uint8_t> state;

    uint8_t s[16][16];
    uint8_t output[16];
    uint8_t i, j, k, l;
    uint32_t counter;

    void generateKeystream();
};

#endif // GRAIN128_H
