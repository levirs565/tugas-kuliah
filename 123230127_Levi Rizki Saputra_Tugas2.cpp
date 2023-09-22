/*
    Aplikasi untuk menginput data postingan
    kemudian mencetak data postingan
    Hasil output berwarna-warni
    Dibuat oleh Levi Rizki Saputra
*/

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

namespace UtilitasTerminal
{

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

    unsigned int jumlah_kolom()
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

#define PANJANG_PROMPT 15

    void tampilkan_prompt(string pertanyaan)
    {

        tetapkan_warna(Kuning);
        cout << left << setw(PANJANG_PROMPT) << pertanyaan
             << ": ";
        tetapkan_warna(Bawaan);
    }

    void tampilkan_error(string error)
    {
        tetapkan_warna(Merah);
        cout << error << endl;
        tetapkan_warna(Bawaan);
    }

#define HEADING_PADDING 2

    void tampilkan_heading(string nama)
    {
        const int margin = jumlah_kolom() / 2 - nama.length() / 2 - HEADING_PADDING;
        cout << setw(margin) << "";
        tetapkan_warna(LatarHijau);
        cout << setw(HEADING_PADDING) << "" << nama << setw(HEADING_PADDING) << "";
        tetapkan_warna(Bawaan);
        cout << endl
             << endl;
    }
}

/*
    Mengubah string ke angka
    Jika berhasil, mengembalikan true
*/

bool string_ke_angka(string teks, int *output)
{
    char *endptr;
    *output = (int)strtoll(teks.c_str(), &endptr, 10);
    return *endptr == '\0';
}

/*
    Menginput id
    id tidak boleh berisi spasi atau kosong
    Jika berisi spasi atau kosong, maka input diulangi
    Tidak menggunakan cin >> karena saat ada spasi karakter setelah spasi
    akan diberikan ke pertanyaan selanjutnya
*/

string input_id(string prompt)
{
    string id;
    while (true)
    {
        UtilitasTerminal::tampilkan_prompt(prompt);
        getline(cin, id);

        if (id.find(' ') != string::npos)
        {
            UtilitasTerminal::tampilkan_error("Tidak boleh ada spasi di dalam ID");
            continue;
        }

        if (id.length() == 0)
        {
            UtilitasTerminal::tampilkan_error("ID tidak boleh kosong");
            continue;
        }

        break;
    }
    return id;
}

/*
    Menginput Judul
    judul tidak boleh kosong
    Jika kosong, maka input diulangi
    Tidak menggunakan cin >> tidak mendukung spasi
*/

string input_judul(string prompt)
{
    string judul;

    while (true)
    {
        UtilitasTerminal::tampilkan_prompt(prompt);
        getline(cin, judul);
        if (judul.length() == 0)
        {

            UtilitasTerminal::tampilkan_error("Judul tidak boleh kosong.");
            continue;
        }
        break;
    }
    return judul;
}

const string format_tanggal = "dd/mm/yyyy";

/*
    Menginput tanggal
    Tanggal harus berformat dd/mm/yyy
    Jika format salah atau bukan angka, maka input diulangi
    Tidak menggunakan scanf("%02d/%02d/%04d") karena jika format salah maka
    input diberikan ke pertanyaan selanjutnya
*/

void input_tanggal(string prompt, int *tanggal, int *bulan, int *tahun)
{
    string input;
    while (true)
    {
        UtilitasTerminal::tampilkan_prompt(prompt);
        UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Biru);
        cout << "(" << format_tanggal << ") ";
        UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Bawaan);
        getline(cin, input);

        // dd/mm/yyyy
        //   /  /
        // 0123456789

        if (input.length() != format_tanggal.length() || input.at(2) != '/' || input.at(5) != '/')
        {
            UtilitasTerminal::tampilkan_error("Format tanggal tidak valid. Gunakan format");
            continue;
        }

        if (!string_ke_angka(input.substr(0, 2), tanggal))
        {
            UtilitasTerminal::tampilkan_error("Tanggal tidak sepenuhnya angka");
            continue;
        }
        if (!string_ke_angka(input.substr(3, 2), bulan))
        {
            UtilitasTerminal::tampilkan_error("Bulan tidak sepenuhnya angka");
            continue;
        }
        if (!string_ke_angka(input.substr(6, 4), tahun))
        {
            UtilitasTerminal::tampilkan_error("Tahun tidak sepenuhnya angka");
            continue;
        }

        if (*tanggal < 1 || *tanggal > 31)
        {
            UtilitasTerminal::tampilkan_error("Tanggal hanya boleh dari 1 sampai 31");
            continue;
        }

        if (*bulan < 1 || *bulan > 12)
        {
            UtilitasTerminal::tampilkan_error("Bulan hanya boleh dari 1 sampai 12");
            continue;
        }

        if (*tahun < 1)
        {
            UtilitasTerminal::tampilkan_error("Tahun tidak boleh kurang dari 1");
            continue;
        }

        break;
    }
}

/*
    Menginput banyak baris
    Baris kosong digunakan untuk mengakhiri input
*/

string input_banyak_baris()
{
    string konten;
    string baris;

    do
    {
        getline(cin, baris);
        if (konten.length() > 0)
            konten.append("\n");
        konten.append(baris);
    } while (baris.length() != 0);

    return konten;
}

#define PROMPT_ID "ID"
#define PROMPT_JUDUL "Judul"
#define PROMPT_TAG "Tag"
#define PROMPT_TANGGAL_TERBIT "Tanggal Terbit"

int main()
{
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Hijau);
    cout << "Dibuat oleh ";
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Biru);
    cout << "Levi Rizki Saputra ";
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Kuning);
    cout << "(" << 123230127 << ")";
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Bawaan);
    cout << endl
         << endl;

    UtilitasTerminal::tampilkan_heading("Form Penerbitan Postingan");

    string id = input_id(PROMPT_ID);

    string judul = input_judul(PROMPT_JUDUL);

    UtilitasTerminal::tampilkan_prompt(PROMPT_TAG);
    string tags;
    getline(cin, tags);

    int tanggal;
    int bulan;
    int tahun;
    input_tanggal(PROMPT_TANGGAL_TERBIT, &tanggal, &bulan, &tahun);

    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Kuning);
    cout << "Tulis Konten ";
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Bawaan);
    cout << "(Mendukung Banyak Baris. Akhiri dengan Baris Korong)" << endl;

    string konten = input_banyak_baris();

    cout << endl;
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Hijau);
    cout << "Selamat postingan anda telah diterbitkan";
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Bawaan);
    cout << endl
         << endl;

    UtilitasTerminal::tampilkan_heading("Pratinjau Postingan");

    UtilitasTerminal::tampilkan_prompt(PROMPT_ID);
    cout << id << endl;
    UtilitasTerminal::tampilkan_prompt(PROMPT_JUDUL);
    cout << judul << endl;

    UtilitasTerminal::tampilkan_prompt(PROMPT_JUDUL);
    cout << tags << endl;

    UtilitasTerminal::tampilkan_prompt(PROMPT_TANGGAL_TERBIT);
    cout << right << setfill('0')
         << setw(2) << tanggal << "/"
         << setw(2) << bulan << "/"
         << setw(4) << tahun << endl
         << setfill(' ') << left;

    UtilitasTerminal::tampilkan_prompt("Konten");
    cout << endl
         << konten;

    return 0;
}
