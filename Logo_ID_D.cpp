#include <iostream>
#include <iomanip>

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
        kode_warna = FOREGROUND_RED;
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
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), kode_warna);
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

unsigned int panjang_terminal()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

using namespace std;

int main()
{
    string logo_if[] = {
        " /$$$$$$ /$$$$$$$$",
        "|_  $$_/| $$_____/",
        "  | $$  | $$      ",
        "  | $$  | $$$$$",
        "  | $$  | $$__/",
        "  | $$  | $$   ",
        " /$$$$$$| $$   ",
        "|______/|__/   ",
    };
    string logo_strip_1[] = {
        "     ",
        "     ",
        "     ",
        " /$$$$$$",
        "|______/",
        "        ",
        "        ",
        "        ",
    };

    string logo_d[] = {
        " /$$$$$$$ ",
        "| $$__  $$",
        "| $$  \\ $$",
        "| $$  | $$",
        "| $$  | $$",
        "| $$  | $$",
        "| $$$$$$$/",
        "|_______/ ",
    };

    string logo_strip_2[] = {
        "       ",
        "       ",
        "       ",
        " /$$$$$$",
        "|______/",
        "       ",
        "       ",
        "       ",
    };

    string logo_23[] = {
        "  /$$$$$$   /$$$$$$ ",
        " /$$__  $$ /$$__  $$",
        "|__/  \\ $$|__/  \\ $$",
        " /$$$$$$/   /$$$$$/",
        "/$$____/   |___  $$",
        "| $$       /$$  \\ $$",
        "| $$$$$$$$|  $$$$$$/",
        "|________/ \\______/ ",
    };

    const int jumlah_kolom = logo_if[0].length() + logo_strip_1[0].length() + logo_d[0].length() + logo_strip_2[0].length() + logo_23[0].length();
    const int kolom_terminal = panjang_terminal();
    int margin;
    if (jumlah_kolom > kolom_terminal)
    {
        tetapkan_warna(Merah);
        cout << "Logo mungkin tidak ditampilkan dengan benar" << endl;
        tetapkan_warna(Bawaan);
    } else {
        margin = kolom_terminal / 2 - jumlah_kolom / 2;
    }

    const int jumlah_baris = sizeof(logo_if) / sizeof(string);
    for (int i = 0; i < jumlah_baris; i++)
    {
        cout << setw(margin) << "";
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