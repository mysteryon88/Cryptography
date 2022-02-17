#include "RSA.h"

#include <iterator>

RSA::RSA() {
	privat_key.exp = 0ull;
	privat_key.modulus = 0ull;
	public_key.exp = 0ull;
	public_key.modulus = 0ull;
}

RSA::~RSA() {}

void RSA::GenerateKey() {
	uint64_t p = 0ull, q = 0ull;
	uint64_t modulus = 0ull, euler = 0ull, open_exp = 0ull, secret_exp = 0ull;
	std::random_device rd;
	std::mt19937 mersenne(rd()); // initialize the Mersenne Twister with a random starting number
	//generate p
	do {
		p = 500ull + mersenne() % 2000;
		if (IsPrime(p)) break;
	} while (true);
	//generate q
	do {
		q = 500ull + mersenne() % 2000;
		if (q != p && IsPrime(q)) break;
	} while (true);

	//p = 3557;
	//q = 2579;
	//Their product n = p*q is calculated, which is called the modulus.
	modulus = p * q;
	public_key.modulus = modulus;
	privat_key.modulus = modulus;

	//The value of the Euler function from the number n is calculated: φ(n) = (p−1)⋅(q−1)
	euler = (p - 1) * (q - 1);

	//An integer e ( 1 < e < φ(n) ) is chosen, coprime with the value of the Euler function (t)
	//open exhibitor
	public_key.exp = CalculateE(euler);			        //e

	privat_key.exp = CalculateD(public_key.exp, euler); //d

	//writing to file
	std::ofstream public_file;
	public_file.open("public.key");
	if (public_file.is_open()) 
		public_file << public_key.exp << " " << public_key.modulus;
	public_file.close();

	//writing to file
	std::ofstream privat_file;
	privat_file.open("privat.key");
	if (privat_file.is_open())
		privat_file << privat_key.exp << " " << privat_key.modulus;
	privat_file.close();

	//Пара {e, n} публикуется в качестве открытого ключа RSA
	std::cout << "\nRSA public key is (n = " << public_key.modulus << ", e = " << public_key.exp << ")" << std::endl;

	//Пара {d, n} играет роль закрытого ключа RSA и держится в секрете
	std::cout << "RSA private key is (n = " << privat_key.modulus << ", d = " << privat_key.exp << ")" << std::endl;

	Encrypt("23");
}

//miller-rabin test (start)

bool RSA::IsPrime(uint64_t n, unsigned short iter) {
	if (n < 4) return n == 2 || n == 3;

	unsigned short s = 0;
	uint64_t d = n - 1;
	while ((d & 1) == 0) {
		d >>= 1;
		s++;
	}
	for (unsigned short i = 0; i < iter; i++) {
		int a = 2 + rand() % (n - 3);
		if (CheckComposite(n, a, d, s))
			return false;
	}
	return true;
}

bool RSA::CheckComposite(uint64_t n, uint64_t a, uint64_t d, int s) {
	uint64_t x = Binpower(a, d, n);
	if (x == 1 || x == n - 1)
		return false;
	for (int r = 1; r < s; r++) {
		x = (uint64_t)x * x % n;
		if (x == n - 1)
			return false;
	}
	return true;
}

//fast exponentiation
uint64_t RSA::Binpower(uint64_t base, uint64_t e, uint64_t mod) {
	uint64_t result = 1;
	base %= mod;
	while (e) {
		if (e & 1)
			result = (uint64_t)result * base % mod;
		base = (uint64_t)base * base % mod;
		e >>= 1;
	}
	return result;
}

//miller-rabin test (end)

//open exponent calculation (start)
// Select an integer e ( 1 < e < t ) coprime with the value of the Euler function (t)
uint64_t RSA::CalculateE(uint64_t t) {
	uint64_t e;
	for (e = 2; e < t; e++)
		if (GreatestCommonDivisor(e, t) == 1) 
			return e;

	return -1;
}

uint64_t RSA::GreatestCommonDivisor(uint64_t e, uint64_t t) {
	uint64_t temp;

	while (e > 0){
		temp = e;
		e = t % e;
		t = temp;
	}
	return t;
}
//open exponent calculation (end)

//Reverse modulo
//Calculate secret exponent (start)
int64_t RSA::CalculateD(int64_t a, int64_t m) {
	int64_t x, y;
	gcdex(a, m, x, y);
	x = (x % m + m) % m;
	return x;
}

// Extended Euclid algorithm
int64_t RSA::gcdex(int64_t a, int64_t b, int64_t& x, int64_t& y) {
	if (a == 0) {
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
//Calculate secret exponent (end)

void RSA::Encrypt(std::string path)
{	
	std::string data_hash = "HEllo WOrld!";
	/*
	std::ifstream hash(path); // open file with hash
	if (hash.is_open()) {
		//while (getline(hash, data_hash)); 
		getline(hash, data_hash);
	}
	hash.close();
	*/
	
	std::vector<uint64_t> encrypt_hash(data_hash.length());
	
	for(size_t i = 0; i < data_hash.length(); ++i)
		encrypt_hash[i] = Binpower((uint16_t)data_hash[i], public_key.exp, public_key.modulus);

	std::ofstream encrypted;
	encrypted.open("signed file.enc");
	if (encrypted.is_open())
		std::copy(encrypt_hash.begin(), encrypt_hash.end(), std::ostream_iterator<uint64_t>(encrypted, " "));
	encrypted.close();

	//-----

	//std::copy(encrypt_hash.begin(), encrypt_hash.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
	//std::cout << "\n";

	Decipher("signed file.enc");
}

void RSA::Decipher(std::string path) {
	//path = signed file.enc
	std::ifstream encrypted(path); // open file with encrypted hash
	std::vector<uint64_t> encrypted_hash(100); //-----------------------------------не забыть про размер хэша
	if (encrypted.is_open()) {
		size_t i = 0;
		while (!encrypted.eof()) {
			encrypted >> encrypted_hash[i];
			i++;
		}
	}
	encrypted.close();

	//hash decryption
	std::vector<uint8_t> decrypted_hash(encrypted_hash.size());

	for (size_t i = 0; i < encrypted_hash.size(); ++i)
		decrypted_hash[i] = Binpower(encrypted_hash[i], privat_key.exp, privat_key.modulus);

	//write hash to file for comparison
	std::ofstream decrypted;
	decrypted.open("decrypted.hash");
	if (decrypted.is_open())
		std::copy(decrypted_hash.begin(), decrypted_hash.end(), std::ostream_iterator<uint8_t>(decrypted));
	decrypted.close();

	std::copy(decrypted_hash.begin(), decrypted_hash.end(), std::ostream_iterator<uint8_t>(std::cout));
}