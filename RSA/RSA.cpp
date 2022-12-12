#include "RSA.h"
#include <iostream>
#include <fstream>
#include <random>
#include <iterator>

#define START_LEN 100

RSA::RSA()
{
	open_exp = 0ull;
	secret_exp = 0ull;
	modulus = 0ull;
}

RSA::~RSA() {}

void RSA::GenerateKey()
{
	uint64_t p = 0ull, q = 0ull, euler = 0ull;
	std::random_device rd;
	std::mt19937 mersenne(rd()); // initialize the Mersenne Twister with a random starting number
	// generate p
	do
	{
		p = 5000ull + mersenne() % 20000;
		if (IsPrime(p))
			break;
	} while (true);
	// generate q
	do
	{
		q = 5000ull + mersenne() % 20000;
		if (q != p && IsPrime(q))
			break;
	} while (true);

	// p = 3557;
	// q = 2579;
	// Their product n = p*q is calculated, which is called the modulus.
	modulus = p * q;

	// The value of the Euler function from the number n is calculated: φ(n) = (p−1)⋅(q−1)
	euler = (p - 1) * (q - 1);

	// An integer e ( 1 < e < φ(n) ) is chosen, coprime with the value of the Euler function (t)
	// open exhibitor
	open_exp = CalculateE(euler); // e

	secret_exp = CalculateD(open_exp, euler); // d

	// writing to file
	std::ofstream public_file;
	public_file.open("public.key");
	if (public_file.is_open())
		public_file << open_exp << " " << modulus;
	public_file.close();

	// writing to file
	std::ofstream privat_file;
	privat_file.open("privat.key");
	if (privat_file.is_open())
		privat_file << secret_exp << " " << modulus;
	privat_file.close();

	std::cout << "\nRSA public key is (n = " << modulus << ", e = " << open_exp << ")" << std::endl;
	std::cout << "RSA private key is (n = " << modulus << ", d = " << secret_exp << ")" << std::endl;
	std::cout << "\"private.key\" and \"public.key\" files were created" << std::endl;
}

// miller-rabin test (start)

bool RSA::IsPrime(uint64_t n, uint16_t iter)
{
	if (n < 4)
		return n == 2 || n == 3;

	uint16_t s = 0;
	uint64_t d = n - 1;
	while ((d & 1) == 0)
	{
		d >>= 1;
		s++;
	}
	for (uint16_t i = 0; i < iter; i++)
	{
		uint32_t a = 2 + rand() % (n - 3);
		if (CheckComposite(n, a, d, s))
			return false;
	}
	return true;
}

bool RSA::CheckComposite(uint64_t n, uint64_t a, uint64_t d, int32_t s)
{
	uint64_t x = Binpower(a, d, n);
	if (x == 1 || x == n - 1)
		return false;
	for (int r = 1; r < s; r++)
	{
		x = (uint64_t)x * x % n;
		if (x == n - 1)
			return false;
	}
	return true;
}

// fast exponentiation
uint64_t RSA::Binpower(uint64_t base, uint64_t e, uint64_t mod)
{
	uint64_t result = 1;
	base %= mod;
	while (e)
	{
		if (e & 1)
			result = (uint64_t)result * base % mod;
		base = (uint64_t)base * base % mod;
		e >>= 1;
	}
	return result;
}

// miller-rabin test (end)

// open exponent calculation (start)
//  Select an integer e ( 1 < e < t ) coprime with the value of the Euler function (t)
uint64_t RSA::CalculateE(uint64_t t)
{
	uint64_t e;
	for (e = 2; e < t; e++)
		if (GreatestCommonDivisor(e, t) == 1)
			return e;
	return -1;
}

uint64_t RSA::GreatestCommonDivisor(uint64_t e, uint64_t t)
{
	uint64_t temp;

	while (e > 0)
	{
		temp = e;
		e = t % e;
		t = temp;
	}
	return t;
}
// open exponent calculation (end)

// Reverse modulo
// Calculate secret exponent (start)
int64_t RSA::CalculateD(int64_t a, int64_t m)
{
	int64_t x, y;
	gcdex(a, m, x, y);
	x = (x % m + m) % m;
	return x;
}

// Extended Euclid algorithm
int64_t RSA::gcdex(int64_t a, int64_t b, int64_t &x, int64_t &y)
{
	if (a == 0)
	{
		x = 0;
		y = 1;
		return b;
	}
	int64_t x1, y1;
	int64_t d = gcdex(b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}
// Calculate secret exponent (end)

void RSA::Encrypt(std::string message)
{

	std::vector<uint64_t> encrypt_file(message.length());

	for (size_t i = 0; i < message.length(); ++i)
		encrypt_file[i] = Binpower(message[i], secret_exp, modulus);

	std::ofstream encrypted;
	encrypted.open("file.enc");
	if (encrypted.is_open())
		std::copy(encrypt_file.begin(), encrypt_file.end(), std::ostream_iterator<uint64_t>(encrypted, " "));
	encrypted.close();

	std::cout << "Encrypted message saved in \"file.enc\"" << std::endl;
}

void RSA::Decipher(std::string path)
{

	std::ifstream encrypted(path);
	std::vector<uint64_t> encrypted_message(START_LEN);

	int32_t i = 0;
	if (encrypted.is_open())
		while (!encrypted.eof())
		{
			encrypted >> encrypted_message[i];
			i++;
			if (i >= encrypted_message.size())
				encrypted_message.resize(i + 1);
		}
	else
		std::cout << "Encrypted message file not opened" << std::endl;
	encrypted.close();
	encrypted_message.resize(i);

	std::vector<uint8_t> decrypted_message(encrypted_message.size());

	for (size_t i = 0; i < encrypted_message.size(); ++i)
		decrypted_message[i] = Binpower(encrypted_message[i], open_exp, modulus);
	std::cout << "Decrypted message:" << std::endl;
	std::copy(decrypted_message.begin(), decrypted_message.end(), std::ostream_iterator<uint8_t>(std::cout));
}

void RSA::LoadMyKey(std::string pub_key, std::string priv_key)
{

	std::ifstream pub(pub_key);

	if (pub.is_open())
		pub >> open_exp >> modulus;
	pub.close();

	std::ifstream priv(priv_key);

	if (priv.is_open())
		priv >> secret_exp >> modulus;
	priv.close();

	std::cout << "\nRSA public key is (n = " << modulus << ", e = " << open_exp << ")" << std::endl;

	std::cout << "RSA private key is (n = " << modulus << ", d = " << secret_exp << ")" << std::endl;
	std::cout << "Keys have been uploaded" << std::endl;
}
