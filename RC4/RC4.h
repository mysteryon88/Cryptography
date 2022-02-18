#pragma once
#include <cstdint>
#include <string>

#define KEY_LEN 32
#define S_LEN	256

class RC4
{
private:
	uint8_t Sbox[256];
	void Swap(uint8_t* x, uint8_t* y);

public:
	RC4(char* key);
	~RC4();
	
	void KeyGen(char* output, uint16_t k_size);
	void EnDeCrypt(char* output, char* key, uint16_t k_size, char* input, uint32_t mes_len);
	void PrintASCII(char* input, uint32_t size);
};

void RandomKey(char* key);