#include <iostream>
#include "RC4.h"

int main(int argc, char *argv[])
{
	char key[] = "PY39=MQ</FOV@8H;1>3:%D3FPP?(4P&0";

	char *random_key = new char(KEY_LEN);
	RandomKey(random_key);

	RC4 test(key); //	RC4 test(random_key);

	char mes[] = "Hello, World!";
	uint32_t mes_len = strlen(mes);
	test.PrintASCII(mes, mes_len);

	uint16_t k_size = BUFSIZ;
	char *K = new char(k_size);
	test.KeyGen(K, k_size);

	test.EnDeCrypt(mes, K, k_size, mes, mes_len);
	test.PrintASCII(mes, mes_len);

	test.EnDeCrypt(mes, K, k_size, mes, mes_len);
	test.PrintASCII(mes, mes_len);

	return 0;
}