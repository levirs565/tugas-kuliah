#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

using namespace std;

/*
    Fungsi pewarnaan cout
    Sistem non-Windows mendukung ANSI escape
    Windows tidak menundkung ANSI escape
    Dibutuhkan fungsi yang berbeda untuk mewarnai stdout
*/

enum Warna
{
    Merah,
    Biru,
    Kuning,
    Hijau,
    Ungu,
    Bawaan,
    LatarHijau
};

void tetapkan_warna(Warna warna)
{
#ifdef _WIN32
    WORD kode_warna;
    switch (warna)
    {
    case Merah:
        kode_warna = FOREGRROUND_RED;
        break;
    case Hijau:
        kode_warna = FOREGROUND_GREEN;
        break;
    case Kuning:
        kode_warna = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case Ungu:
        kode_warna = FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    case Biru:
        kode_warna = FOREGROUND_BLUE | FOREGROUND_GREEN;
        break;
    case LatarHijau:
        kode_warna = BACKGROUND_GREEN;
        break;
    case Bawaan:
        kode_warna = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    default:
        return;
        break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), warna);
#else
    string kode_warna;
    switch (warna)
    {
    case Merah:
        kode_warna = "31";
        break;
    case Hijau:
        kode_warna = "32";
        break;
    case Kuning:
        kode_warna = "33";
        break;
    case Ungu:
        kode_warna = "35";
        break;
    case Biru:
        kode_warna = "36";
        break;
    case LatarHijau:
        kode_warna = "42";
        break;
    case Bawaan:
        kode_warna = "0";
        break;
    default:
        return;
        break;
    }
    cout << "\033[" << kode_warna << "m";
#endif
}

using namespace std;

int main()
{
    string logo_if[] = {
        "██╗███████╗",
        "██║██╔════╝",
        "██║█████╗",
        "██║██╔══╝",
        "██║██║   ",
        "╚═╝╚═╝   ",
    };
    string logo_strip_1[] = {
        "    ",
        "    ",
        "█████╗",
        "╚════╝",
        "      ",
        "      ",
    };

    string logo_d[] = {
        "██████╗ ",
        "██╔══██╗",
        "██║  ██║",
        "██║  ██║",
        "██████╔╝",
        "╚═════╝ ",
    };

    string logo_strip_2[] = {
        "      ",
        "      ",
        "█████╗",
        "╚════╝",
        "      ",
        "      ",
    };

    string logo_23[] = {
        "██████╗ ██████╗ ",
        "╚════██╗╚════██╗",
        " █████╔╝ █████╔╝",
        "██╔═══╝  ╚═══██╗",
        "███████╗██████╔╝",
        "╚══════╝╚═════╝ ",
    };

    const int jumlah_baris = sizeof(logo_if) / sizeof(string);
    for (int i = 0; i < jumlah_baris; i++)
    {
        tetapkan_warna(Hijau);
        cout << logo_if[i];
        tetapkan_warna(Ungu);
        cout << logo_strip_1[i];
        tetapkan_warna(Biru);
        cout << logo_d[i];
        tetapkan_warna(Ungu);
        cout << logo_strip_2[i];
        tetapkan_warna(Kuning);
        cout << logo_23[i];
        tetapkan_warna(Bawaan);
        cout << endl;
    }

    return 0;
}