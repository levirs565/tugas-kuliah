#include <iostream>
#include <exception>
#include <cmath>

using namespace std;

string intKeStr(int angka);

int hexCharToInt(char hex)
{
    if (hex >= 'a' && hex <= 'f')
        return 10 + (hex - 'a');
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'A' && hex <= 'F')
        return 10 + (hex - 'A');
    throw invalid_argument(string("karakter bukanlah bilangan hex ") + hex);
}

int hexStrToInt(string hex)
{
    int angka = 0;
    int factor = 1;
    for (int i = hex.length() - 1; i >= 0; i--)
    {
        char currentChar = hex.at(i);
        int currentNumber = hexCharToInt(currentChar);
        angka += currentNumber * factor;
        factor *= 16;
    }
    return angka;
}

char intToHexChar(int angka)
{
    if (angka >= 0 && angka <= 9)
        return angka + '0';
    if (angka <= 15)
        return 'a' + (angka - 10);
    throw invalid_argument(string("karakter melambangkan hex lebih dari 1 digit: ") + intKeStr(angka));
}

string intToHexStr(int angka)
{
    int digitCount = (int)floor(log(angka) / log(16)) + 1;
    char str[digitCount + 1] = {};
    str[digitCount] = '\0';

    int currenDivider = 1;

    for (int power = 1; power < digitCount; power++)
    {
        currenDivider *= 16;
    }

    int tersisa = angka;
    for (int i = 0; i < digitCount; i++)
    {
        int currentDigit = (int)floor(tersisa / currenDivider);
        str[i] = intToHexChar(currentDigit);
        tersisa = tersisa - currenDivider * currentDigit;
        currenDivider /= 16;
    }

    return string(str);
}

char intKeChar(int angka)
{
    if (angka < 0 || angka > 9)
        throw invalid_argument("angka harus satu digit dan positif");
    return '0' + angka;
}

string intKeStr(int angka)
{
    int digitCount = (int)floor(log10(angka) + 1);
    char str[digitCount + 1] = {};
    str[digitCount] = '\0';

    int currentDivider = 1;

    for (int power = 1; power < digitCount; power++)
    {
        currentDivider *= 10;
    }

    int tersisa = angka;
    for (int i = 0; i < digitCount; i++)
    {
        int currentDigit = (int)floor(tersisa / currentDivider);
        str[i] = intKeChar(currentDigit);

        tersisa = tersisa - currentDigit * currentDivider;

        currentDivider /= 10;
    }

    return string(str);
}

int strKeInt(string str)
{
    int result = 0;
    int tenFactor = 1;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        const char currentChar = str.at(i);
        if (currentChar < '0' || currentChar > '9')
            throw invalid_argument(string("ditemukan karakter bukan angka yaitu ") + currentChar);
        int currentDigit = currentChar - '0';
        result += currentDigit * tenFactor;
        tenFactor *= 10;
    }
    return result;
}

int main()
{
    cout << "Pilihan mode: " << endl;
    cout << "1. Desimal (Basis 10) " << endl;
    cout << "2. Hex (Basis 10) " << endl;
    cout << "Pilih mode: ";

    int mode;
    cin >> mode;
    cin.ignore();

    cout << "Masukkan angka\t: ";
    string str;
    getline(cin, str);

    int hasil = mode == 1 ? strKeInt(str) : mode == 2 ? hexStrToInt(str)
                                                      : 0;
    cout << "Angka hasil\t: " << hasil << endl;

    string ubahKemabli = mode == 1 ? intKeStr(hasil) : mode == 2 ? intToHexStr(hasil)
                                                                 : 0;
    cout << "String kembali\t: " << ubahKemabli << endl;
}