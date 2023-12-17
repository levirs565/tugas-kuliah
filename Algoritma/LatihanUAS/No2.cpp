#include <iostream>

void banyakPecahan(long uang, long pecahan[]);

int main()
{
    long uang;
    long pecahan[9] = {50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100};

    std::cout << "Jumlah Uang Kembali : Rp ";
    std::cin >> uang;
    banyakPecahan(uang, pecahan);
}

void banyakPecahan(long uang, long pecahan[])
{
    int lembar[9] = {};
    std::cout << "Pecahan uang kembali" << std::endl;

    long sisa = uang;
    for (size_t i = 0; i < sizeof(lembar) / sizeof(int); i++)
    {
        long nextSisa = sisa % pecahan[i];
        lembar[i] = (sisa - nextSisa) / pecahan[i];
        sisa = nextSisa;
    }

    for (size_t i = 0; i < sizeof(lembar) / sizeof(int); i++)
    {
        std::cout << lembar[i] << " lembar " << pecahan[i] << std::endl;
    }
}