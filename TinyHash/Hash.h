#pragma once
#include <string>
#include <cstdint>

#define HASH_LEN 32

class Hash
{
private:
	std::string hash;
	uint32_t ReceivingExistCodes(uint32_t x);

public:
	std::string GenerateHash(std::string userString);
};
