#include "StreamCipher/StreamCipher.h"
#include "Grain128/Grain128.h"
#include "ChaCha20/ChaCha20.h"
#include "RC4/RC4.h"
#include "CryptMT/CryptMT.h"

#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <cstdlib> // for getenv

#ifdef _WIN32
#include <windows.h>
#include <lmcons.h> // for UNLEN
#else
#include <pwd.h>	// for struct passwd
#include <unistd.h> // for getuid()
#endif

/**
 * Function to get the system username.
 */
std::string getCurrentUsername()
{
	std::string username;

#ifdef _WIN32
	char name[UNLEN + 1];
	DWORD size = UNLEN + 1;
	if (GetUserNameA(name, &size))
	{
		username = name;
	}
	else
	{
		username = "Unknown";
	}
#else
	struct passwd *pw = getpwuid(getuid());
	if (pw)
	{
		username = pw->pw_name;
	}
	else
	{
		username = "Unknown";
	}
#endif

	return username;
}

/**
 * Function to generate greeting file.
 */
void generateGreetingFile(const std::string &filePath)
{
	std::string userName = getCurrentUsername();
	std::string message = "Hello, " + userName + " from Sergey Protasov @st43r!";

	std::ofstream outFile(filePath);
	if (!outFile)
	{
		std::cerr << "Error opening file: " << filePath << std::endl;
		return;
	}

	outFile << message;
	outFile.close();
}

int main()
{
	generateGreetingFile("input.txt");

	const std::string inputPath = "input.txt";

	std::vector<uint8_t> key(16, 0x01);
	std::vector<uint8_t> iv(12, 0x02);

	Grain128 grain128Encrypt(key, iv);
	std::string outputPath = "encrypted_grain128.bin";
	grain128Encrypt.encryptDecryptFile(inputPath, outputPath);

	Grain128 grain128Decrypt(key, iv);
	grain128Decrypt.encryptDecryptFile(outputPath, "decrypted_grain128.txt");

	std::vector<uint8_t> chachaKey(32, 0x01);
	std::vector<uint8_t> nonce(8, 0x02);
	ChaCha20 chacha20Encrypt(chachaKey, nonce);
	outputPath = "encrypted_chacha20.txt";
	chacha20Encrypt.encryptDecryptFile(inputPath, outputPath);

	ChaCha20 chacha20Decrypt(chachaKey, nonce);
	chacha20Decrypt.encryptDecryptFile(outputPath, "decrypted_chacha20.txt");

	RC4 rc4Encrypt(key);
	outputPath = "encrypted_cryptmt.txt";
	rc4Encrypt.encryptDecryptFile(inputPath, outputPath);

	RC4 rc4Decrypt(key);
	rc4Decrypt.encryptDecryptFile(outputPath, "decrypted_cryptmt.txt");

	std::vector<uint32_t> cryptmtKey = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	CryptMT cryptMTEncrypt(cryptmtKey);
	outputPath = "encrypted_rc4.txt";
	cryptMTEncrypt.encryptDecryptFile(inputPath, outputPath);

	CryptMT cryptMTDecrypt(cryptmtKey);
	cryptMTDecrypt.encryptDecryptFile(outputPath, "decrypted_rc4.txt");

	return 0;
}
