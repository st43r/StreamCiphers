#ifndef RC4_H
#define RC4_H

#include "StreamCipher.h"

class RC4 : public StreamCipher {
public:
    explicit RC4(const std::vector<uint8_t> &key);

    void encryptDecryptFile(const std::string &inputFile, const std::string &outputFile) override;

private:
    std::vector<uint8_t> state;
    int i, j;

    uint8_t generateKeystreamByte();
};


#endif // RC4_H
