#include "Hash.h"

uint32_t Hash::ReceivingExistCodes(uint32_t x)
{
	x += 256;
	while (!(((x <= 57) && (x >= 48)) || ((x <= 90) && (x >= 56)) || ((x <= 122) && (x >= 97))))
	{
		if (x < 48)
			x += 24;
		else
			x -= 47;
	}
	return x;
}

std::string Hash::GenerateHash(std::string userString)
{

	uint32_t lenghtHash = HASH_LEN;
	uint32_t minLen = 2;
	uint32_t originalLenghtStr = userString.size();

	// getting the length of a string that is a multiple of two, closest to the given hash length
	while (minLen <= lenghtHash)
		minLen *= 2;

	// Get closest to original string length (2^n)
	while (minLen < originalLenghtStr)
		minLen *= 2;

	// Make the length of the hash string at least twice the length of the original string
	if ((minLen - originalLenghtStr) < minLen)
		minLen *= 2;

	// Get the number of characters to add to the string
	uint32_t addCount = minLen - originalLenghtStr;

	for (uint32_t i = 0; i < addCount; ++i)
		userString += this->ReceivingExistCodes(userString[i] + userString[i + 1]);

	for (uint32_t i = 0; this->hash.size() < (lenghtHash); ++i)
		this->hash += this->ReceivingExistCodes(userString[i] + userString[++i]);

	return this->hash;
}
