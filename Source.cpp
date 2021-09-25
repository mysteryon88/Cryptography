#include <iostream>
#include <string>
#include <Windows.h>

//ÍÀ ÄÍÅ ÊÀÆÄÎÃÎ ÑÅĞÄÖÀ ÅÑÒÜ ÎÑÀÄÎÊ

constexpr auto N = 6;

const char POLYBIUS[N][N] = {
    { 'À', 'Á', 'Â', 'Ã', 'Ä', 'Å'},
    { '¨', 'Æ', 'Ç', 'È', 'É', 'Ê'},
    { 'Ë', 'Ì', 'Í', 'Î', 'Ï', 'Ğ'},
    { 'Ñ', 'Ò', 'Ó', 'Ô', 'Õ', 'Ö'},
    { '×', 'Ø', 'Ù', 'Ú', 'Û', 'Ü'},
    { 'İ', 'Ş', 'ß', ' ', ',', '.'}
};

char encrypt(const char c);
char decrypt(const char c);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string c, _c;
    int i;
    std::cout << "Ââåäèòå ñòğîêó:\n";
    std::getline(std::cin, c);
    std::cout << "\n----------ÇÀÊÎÄÈĞÎÂÀÍÍÎ----------" << std::endl;

    for (i = 0; i < c.size(); ++i)
        _c += encrypt(c[i]);
    std::cout << _c << std::endl;

    std::cout << "\n----------ĞÀÑÊÎÄÈĞÎÂÀÍÍÎ---------" << std::endl;

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