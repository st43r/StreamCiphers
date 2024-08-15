#ifndef CRYPTMT_H
#define CRYPTMT_H

#include "StreamCipher.h"
#include <vector>
#include <cstdint>

class CryptMT : public StreamCipher {
public:
    CryptMT(const std::vector<uint32_t> &key);

    void encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) override;

private:
    std::vector<uint32_t> state;
    uint32_t index;

    void initialize(const std::vector<uint32_t> &key);
    uint8_t generateKeystreamByte();
};

#endif // CRYPTMT_H
