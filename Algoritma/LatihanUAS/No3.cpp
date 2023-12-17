#include <iostream>

void konversi(int bilangan, int *jam, int *menit, int *detik);

int main()
{
    int bilangan, jam, menit, detik;

    std::cout << "bilangan = ";
    std::cin >> bilangan;

    konversi(bilangan, &jam, &menit, &detik);

    std::cout << bilangan << " detik = ";
    std::cout << jam << " jam ";
    std::cout << menit << " menit ";
    std::cout << detik << " detik";
    std::cout << std::endl;
}

int customDivFloor(int p, int q)
{
    int remaining = p % q;
    return (p - remaining) / q;
}

void konversi(int bilangan, int *jam, int *menit, int *detik)
{
    // CARA 1
    // *detik = bilangan % 60;
    // *menit = ((bilangan - *detik) / 60) % 60;
    // *jam = (bilangan - *menit * 60 - *detik) / 3600;

    // CARA 2

    // *detik = bilangan % 60;
    // *menit = customDivFloor(bilangan, 60) % 60;
    // *jam = customDivFloor(bilangan, 3600);

    // CARA 3

    *detik = bilangan % 60;
    *menit = bilangan / 60 % 60;
    *jam = bilangan / 3600;
}