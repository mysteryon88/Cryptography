#include <iostream>
#include <string>
#include <Windows.h>

constexpr auto N = 6;

const char POLYBIUS[N][N] = {
    { '�', '�', '�', '�', '�', '�'},
    { '�', '�', '�', '�', '�', '�'},
    { '�', '�', '�', '�', '�', '�'},
    { '�', '�', '�', '�', '�', '�'},
    { '�', '�', '�', '�', '�', '�'},
    { '�', '�', '�', ' ', ',', '.'}
};

char encrypt(const char c);
char decrypt(const char c);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string c, _c;
    int i;
    std::cout << "������� ������:\n";
    std::getline(std::cin, c);
    std::cout << "\n----------�������������----------" << std::endl;

    for (i = 0; i < c.size(); ++i)
        _c += encrypt(c[i]);
    std::cout << _c << std::endl;

    std::cout << "\n----------��������������---------" << std::endl;

    for (i = 0; i < _c.size(); ++i)
        std::cout << decrypt(_c[i]);
    std::cout << std::endl;

    return 0;
}

char encrypt(const char c) {
    int i, j;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            if (POLYBIUS[i][j] == c)
                return POLYBIUS[(i + 1) % N][j];
}

char decrypt(const char c) {
    int i, j;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            if (POLYBIUS[i][j] == c)
                return POLYBIUS[(i + (N - 1)) % N][j];
}