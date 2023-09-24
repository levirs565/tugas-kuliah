#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

std::string format_currency(int angka)
{
    std::vector<int> daftarAngka;
    int hasil = angka;
    while (hasil >= 1000)
    {
        div_t bagi = div(hasil, 1000);
        daftarAngka.push_back(bagi.rem);
        hasil = bagi.quot;
    }
    daftarAngka.push_back(hasil);

    std::stringstream stream;

    for (std::vector<int>::reverse_iterator it = daftarAngka.rbegin();
         it != daftarAngka.rend();
         it++)
    {
        if (it != daftarAngka.rbegin())
        {
            stream << ".";
            stream << std::right << std::setw(3) << std::setfill('0');
        }
        stream << *it;
    }
    return stream.str();
}

int main()
{
    std::cout << format_currency(123) << std::endl;
    std::cout << format_currency(1002) << std::endl;
    std::cout << format_currency(1236245) << std::endl;
    std::cout << format_currency(1236245789) << std::endl;
    return 0;
}