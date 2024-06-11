#ifndef CRYPTMT_H
#define CRYPTMT_H

#include "StreamCipher.h"

class CryptMT : public StreamCipher {
public:
    explicit CryptMT(const std::vector<uint32_t> &key);

    void encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) override;

private:
    std::vector<uint32_t> state;
    int index = 624;

    uint32_t generateRandomByte();

    void initState(const std::vector<uint32_t> &key);

    void generateState();
};

#endif // CRYPTMT_H
