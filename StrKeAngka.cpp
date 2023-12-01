#include <iostream>
#include <exception>
#include <cmath>

using namespace std;

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

    int tersisa = angka;
    for (int i = 0; i < digitCount; i++)
    {
        int divider = 1;
        const int powerFactor = digitCount - i;
        for (int j = 1; j < powerFactor; j++)
        {
            divider *= 10;
        }

        int currentDigit = (int)floor(tersisa / divider);
        str[i] = intKeChar(currentDigit);

        tersisa = tersisa - currentDigit * divider;
    }

    return string(str);
}

int strKeInt(string str)
{
    int result = 0;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        const int tenFactor = str.length() - i;
        const char currentChar = str.at(i);
        if (currentChar < '0' || currentChar > '9')
            throw invalid_argument("ditemukan karakter bukan angka");
        int currentNumber = currentChar - '0';
        for (int j = 1; j < tenFactor; j++)
            currentNumber *= 10;
        result += currentNumber;
    }
    return result;
}

int main()
{
    cout << "Masukkan angka\t: ";
    string str;
    getline(cin, str);

    int hasil = strKeInt(str);
    cout << "Angka hasil\t: " << hasil << endl;

    string ubahKemabli = intKeStr(hasil);
    cout << "String kembali\t: " << ubahKemabli << endl;
}