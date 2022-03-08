#include "DH.h"

std::string inttohex(uint64_t a)
{
	std::string tmp("");
	do
	{
		int r(a % 16);
		if (r > 9) { r += (int)'A' - 10; }
		else { r += (int)'0'; };
		tmp = (char)r + tmp;
		a /= 16;
	} while (a);
	return tmp;
}

int main(void) {
	DH a, b;

	//Alice creates p, g, a, A
	a.Initiate();

	//send to Bob
	b.A_B = a.A_B;
	b.g = a.g;
	b.p = a.p;

	//Bob creates b, B, key
	b.Receiving();

	//sent to Alice B
	a.A_B = b.A_B;

	//Alice got B and creates a key
	a.InitiateKey();

	std::cout << a.GetKey() << " " << b.GetKey() << std::endl;

	std::string key = inttohex(a.GetKey());

	std::cout << "hex " << key << std::endl;

    return 0;
}

