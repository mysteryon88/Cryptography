#include "DH.h"

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

	std::cout << a.key << " " << b.key << std::endl;
    return 0;
}

