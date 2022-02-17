#include <iostream>
#include "RSA.h"

int main(int argc, char* argv[])
{
    std::cout << "---------RSA---------\nEnter your message: ";
    std::string message;
    getline(std::cin, message);
    RSA rsa;
    rsa.GenerateKey();
    rsa.LoadMyKey("public.key", "privat.key");
    rsa.Encrypt(message);
    rsa.Decipher("file.enc");
    return 0;
}