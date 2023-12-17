#include <iostream>
#include <exception>

void konversi(int mm, int *hasil);

int main()
{
    int bilangan;
    std::cout << "bilangan (dalam mm) = ";
    std::cin >> bilangan;

    int hasil[4] = {};
    std::string satuan[4] = {"km", "m", "cm", "mm"};

    konversi(bilangan, hasil);

    std::cout << "hasil = ";
    for (int i = 0; i < 4; i++)
    {
        std::cout << hasil[i] << " " << satuan[i];
        if (i < 3)
        {
            std::cout << ", ";
        }
        else
        {
            std::cout << "." << std::endl;
        }
    }
}

void konversi(int mm, int *hasil)
{
    int faktor[3] = {1000000, 1000, 10};

    int sisa = mm;
    for (int i = 0; i < 3; i++)
    {
        int nextSisa = sisa % faktor[i];
        hasil[i] = (sisa - nextSisa) / faktor[i];
        sisa = nextSisa;
    }

    hasil[3] = sisa;
}