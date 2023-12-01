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

    int currentDivider = 1;

    for (int power = 1; power < digitCount; power++)
    {
        currentDivider *= 10;
    }

    int tersisa = angka;
    for (int i = 0; i < digitCount; i++)
    {
        const int powerFactor = digitCount - i;
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
    cout << "Masukkan angka\t: ";
    string str;
    getline(cin, str);

    int hasil = strKeInt(str);
    cout << "Angka hasil\t: " << hasil << endl;

    string ubahKemabli = intKeStr(hasil);
    cout << "String kembali\t: " << ubahKemabli << endl;
}