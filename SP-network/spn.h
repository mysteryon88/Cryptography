#pragma once

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

#define BLOCKSIZE 8 // 2 4 8 16 bytes
#define BYTE 8

class SP
{
private:
	uint8_t rounds;

	std::unordered_map<uint8_t, std::vector<uint8_t>> s_blocks;
	std::unordered_map<uint8_t, std::vector<uint8_t>> s_blocks_decrypt;

	std::unordered_map<uint8_t, std::vector<uint8_t>> p_blocks;
	std::unordered_map<uint8_t, std::vector<uint8_t>> p_blocks_decrypt;

	void generate_s_blocks();
	void generate_p_blocks();

	void s_round(std::vector<uint8_t> &block, uint8_t round, bool encrypt);
	void p_round(std::vector<uint8_t> &block, uint8_t round, bool encrypt);

	void u8_to_bits(uint8_t num, std::vector<bool> &byte);
	uint8_t bits_to_u8(std::vector<bool> &bits);

	template <typename T>
	void shuffle(std::vector<T> &arr, uint8_t &rnd);

public:
	SP(uint8_t _rounds);

	void encrypt(std::vector<uint8_t> &pixels);
	void decrypt(std::vector<uint8_t> &pixels);
};
