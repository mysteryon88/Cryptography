#include "spn.h"

SP::SP(uint8_t _rounds) : rounds(_rounds)
{
	srand(time(NULL));
	generate_s_blocks();
	generate_p_blocks();
}

void SP::generate_s_blocks()
{
	uint8_t count_rounds = 0;

	while (count_rounds < rounds)
	{
		std::vector<uint8_t> tmp(UINT8_MAX + 1);

		for (auto i = 0; i < tmp.size(); ++i)
			tmp[i] = i;

		uint8_t rnd = rand() % UINT8_MAX;

		shuffle(tmp, rnd);

		s_blocks.insert(std::make_pair(count_rounds, tmp));

		std::vector<uint8_t> tmp_order(UINT8_MAX + 1);

		for (auto i = 0; i < tmp_order.size(); ++i)
			tmp_order[tmp[i]] = i;

		s_blocks_decrypt.insert(std::make_pair(count_rounds, tmp_order));

		count_rounds++;
	}
}

void SP::generate_p_blocks()
{
	uint8_t count_rounds = 0;

	while (count_rounds < rounds)
	{
		std::vector<uint8_t> tmp(BLOCKSIZE * BYTE / 2); // 16 * 8 = 128

		for (auto i = 0; i < tmp.size(); ++i)
			tmp[i] = i + tmp.size();

		uint8_t rnd = rand() % UINT8_MAX;
		shuffle(tmp, rnd);

		p_blocks.insert(std::make_pair(count_rounds, tmp));

		std::vector<uint8_t> tmp_order(BLOCKSIZE * BYTE); // 16 * 8 = 128

		for (auto i = 0; i < tmp.size(); ++i)
			tmp_order[tmp[i]] = i;

		p_blocks_decrypt.insert(std::make_pair(count_rounds, tmp_order));

		++count_rounds;
	}
}

void SP::encrypt(std::vector<uint8_t> &pixels)
{

	uint8_t count_rounds = 0;

	while (count_rounds < rounds)
	{
		std::vector<uint8_t> block(BLOCKSIZE);

		for (size_t i = 0; i < pixels.size(); i += BLOCKSIZE)
		{
			for (size_t j = 0; j < BLOCKSIZE; ++j)
				block[j] = pixels[i + j];

			s_round(block, count_rounds, true);

			p_round(block, count_rounds, true);

			for (size_t j = 0; j < BLOCKSIZE; ++j)
				pixels[i + j] = block[j];
		}

		count_rounds++;
	}
}

void SP::decrypt(std::vector<uint8_t> &pixels)
{

	int8_t count_rounds = rounds - 1;

	while (count_rounds >= 0)
	{
		std::vector<uint8_t> block(BLOCKSIZE);

		for (size_t i = 0; i < pixels.size(); i += BLOCKSIZE)
		{
			for (size_t j = 0; j < BLOCKSIZE; ++j)
				block[j] = pixels[i + j];

			p_round(block, count_rounds, false);

			s_round(block, count_rounds, false);

			for (size_t j = 0; j < BLOCKSIZE; ++j)
				pixels[i + j] = block[j];
		}

		count_rounds--;
	}
}

void SP::s_round(std::vector<uint8_t> &block, uint8_t round, bool encrypt)
{
	if (encrypt)
	{
		for (size_t i = 0; i < BLOCKSIZE; ++i)
			block[i] = s_blocks[round][block[i]];
	}
	else
	{
		for (size_t i = 0; i < BLOCKSIZE; ++i)
			block[i] = s_blocks_decrypt[round][block[i]];
	}
}

void SP::p_round(std::vector<uint8_t> &block, uint8_t round, bool encrypt) // 16
{
	std::vector<bool> bits(block.size() * BYTE); // 16 * 8
	std::vector<bool> byte(BYTE);				 // 8
	if (encrypt)
	{
		for (size_t i = 0; i < block.size(); ++i)
		{
			u8_to_bits(block[i], byte);
			for (size_t j = 0; j < BYTE; ++j)
				bits[i * BYTE + j] = byte[j];
		}

		for (size_t i = 0; i < bits.size() / 2; ++i)
		{
			bool tmp = bits[size_t(p_blocks[round][i])];
			bits[size_t(p_blocks[round][i])] = bits[i];
			bits[i] = tmp;
		}

		for (size_t i = 0; i < block.size(); ++i)
		{
			for (size_t j = 0; j < BYTE; ++j)
				byte[j] = bits[i * BYTE + j];
			block[i] = bits_to_u8(byte);
		}
	}
	else
	{
		for (size_t i = 0; i < block.size(); ++i)
		{
			u8_to_bits(block[i], byte);
			for (size_t j = 0; j < BYTE; ++j)
				bits[i * BYTE + j] = byte[j];
		}

		for (size_t i = bits.size() / 2; i < bits.size(); ++i)
		{
			bool tmp = bits[size_t(p_blocks_decrypt[round][i])];
			bits[size_t(p_blocks_decrypt[round][i])] = bits[i];
			bits[i] = tmp;
		}

		for (size_t i = 0; i < block.size(); ++i)
		{
			for (size_t j = 0; j < BYTE; ++j)
				byte[j] = bits[i * BYTE + j];
			block[i] = bits_to_u8(byte);
		}
	}
}

template <typename T>
void SP::shuffle(std::vector<T> &arr, uint8_t &rnd)
{
	for (uint32_t i = arr.size() - 1; i >= 1; i--)
	{
		uint32_t j = rnd % (i + 1);

		T tmp = arr[j];
		arr[j] = arr[i];
		arr[i] = tmp;
		rnd++;
	}
}

void SP::u8_to_bits(uint8_t num, std::vector<bool> &byte)
{
	for (int8_t i = 7; i >= 0; --i)
	{
		byte[i] = num & 1;
		num >>= 1;
	}
}

uint8_t SP::bits_to_u8(std::vector<bool> &bits)
{
	uint8_t result = 0;
	for (auto i : bits)
		result = (result << 1) | i;

	return result;
}