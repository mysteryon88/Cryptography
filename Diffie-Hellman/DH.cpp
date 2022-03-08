#include "DH.h"
#include <random>

DH::DH() {
	key = 0;
	A_B = 0;
	p = 0;
	g = 2;
	a_b = 0;
}
DH::~DH() {}

uint64_t DH::GCD(uint64_t a, uint64_t b) {
	if (b == 0)
		return a;
	return GCD(b, a % b);
}

bool DH::IsPrime(uint64_t x) {
	if (x == 2)
		return true;
	std::random_device rd;
	std::mt19937 mersenne(rd());
	for (uint8_t i = 0; i < 100;i++) {
		uint64_t a = (mersenne() % (x - 2)) + 2; //------------------------------------------------------
		if (GCD(a, x) != 1)
			return false;
		if (Binpower(a, x - 1, x) != 1)
			return false;
	}
	return true;
}

uint64_t DH::Mul(uint64_t x, uint64_t e, uint64_t m) {
	if (e == 1)
		return x;
	if (e % 2 == 0) {
		uint64_t t = Mul(x, e / 2, m);
		return (2 * t) % m;
	}
	return (Mul(x, e - 1, m) + x) % m;
}

uint64_t DH::Binpower(uint64_t x, uint64_t e, uint64_t m) {
	if (e == 0)
		return 1;
	if (e % 2 == 0) {
		uint64_t t = Binpower(x, e / 2, m);
		return Mul(t, t, m) % m;
	}
	return (Mul(Binpower(x, e - 1, m), x, m)) % m;
}

void DH::Initiate() {

	std::random_device rd;
	std::mt19937 mersenne(rd());

	uint64_t q = 0;

	do {
		//pub
		p = 1000000ull + mersenne() % 90000000ull; //-------------------------------------------

		if (IsPrime(p)) {
			//check for a simple reliable number
			q = (p - 1) / 2;
			if (IsPrime(q)) break;
		}

	} while (true); 
	std::cout << "q " << q << " p " << p << std::endl;

	uint64_t gsimple = Binpower(g, q, p);
	while (gsimple != 1)
	{
		gsimple = Binpower(g, q, p);
		g++;
		if (g % 2 == 1) g++;
	}
	std::cout << "g " << g << std::endl;
	
	//priv
	a_b = 1000000ull + mersenne() % 90000000ull; //-------------------------------------------


	//pub
	A_B = Binpower(g, a_b, p); //pub

	std::cout << "A " << A_B << std::endl;

	//send p, g, A to the interlocutor

}

void DH::Receiving(){
	std::random_device rd;
	std::mt19937 mersenne(rd());

	if (A_B == 1) return; //vulnerability replace A/B with 1

	//priv
	a_b = 1000000ull + mersenne() % 90000000ull; //-------------------------------------------

	key = Binpower(A_B, a_b, p); //priv

	A_B = Binpower(g, a_b, p);	//pub

	std::cout << "B " << A_B << std::endl;
}

void DH::InitiateKey() {
	//received from the interlocutor
	if (A_B == 1) return; //vulnerability replace A/B with 1
		key = Binpower(A_B, a_b, p);
}

uint64_t DH::GetKey() {
	return key;
}