#pragma once
#include <iostream>
#include <cstdint>

class DH
{
private:
	/*
	uint64_t A_B, p, g;
	uint64_t a_b, key;
	*/
	//Greatest Common Divisor (Euclid's algorithm)
	uint64_t GCD(uint64_t a, uint64_t b);
	//Ferma
	bool IsPrime(uint64_t x);
	uint64_t Mul(uint64_t a, uint64_t b, uint64_t m);
	uint64_t Binpower(uint64_t a, uint64_t b, uint64_t m);

public:
	void Initiate();
	void InitiateKey();
	void Receiving();
	uint64_t A_B, p, g;
	uint64_t a_b, key;
	DH();
	~DH();
};

