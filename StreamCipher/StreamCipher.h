#ifndef STREAMCIPHER_H
#define STREAMCIPHER_H

#include <string>
#include <vector>
#include <cstdint>

class StreamCipher {
public:
    /**
     * Encrypts or Decrypts file using stream cipher.
     */
    virtual void encryptDecryptFile(const std::string& inputFile, const std::string& outputFile) = 0;
    virtual ~StreamCipher() = default;
};

#endif // STREAMCIPHER_H
