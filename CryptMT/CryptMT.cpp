#include "CryptMT.h"
#include <fstream>
#include <iostream>

CryptMT::CryptMT(const std::vector<uint32_t>& key) : state(625)
{
    initState(key);
}

uint32_t CryptMT::generateRandomByte()
{
    if (index >= 624)
    {
        generateState();
    }

    uint32_t y = state[index++];

    y ^= (y >> 11);
    y ^= ((y << 7) & 0x9D2C5680);
    y ^= ((y << 15) & 0xEFC60000);
    y ^= (y >> 18);

    return y & 0xFF;
}

void CryptMT::initState(const std::vector<uint32_t>& key)
{
    state[0] = key[0];

    for (int i = 1; i < 624; ++i)
    {
        state[i] = 0xFFFFFFFF & (0x6C078965 * (state[i - 1] ^ (state[i - 1] >> 30)) + i);
    }
}

void CryptMT::generateState()
{
    for (int i = 0; i < 624; ++i)
    {
        uint32_t y = (state[i] & 0x80000000) + (state[(i + 1) % 624] & 0x7FFFFFFF);
        state[i] = state[(i + 397) % 624] ^ (y >> 1);

        if (y % 2 != 0)
        {
            state[i] ^= 0x9908B0DF;
        }
    }
}

void CryptMT::encryptDecryptFile(const std::string& inputFile, const std::string& outputFile)
{
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open())
    {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    char buffer;
    while (inFile.get(buffer))
    {
        buffer ^= static_cast<char>(generateRandomByte());
        outFile.put(buffer);
    }

    inFile.close();
    outFile.close();
}
