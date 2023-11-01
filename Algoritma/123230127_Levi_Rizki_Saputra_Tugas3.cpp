#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

using namespace std;

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

    void tetapkanWarna(Warna warna)
    {
#ifdef _WIN32
        WORD kodeWarna;
        switch (warna)
        {
        case Merah:
            kodeWarna = FOREGROUND_RED;
            break;
        case Hijau:
            kodeWarna = FOREGROUND_GREEN;
            break;
        case Kuning:
            kodeWarna = FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case Biru:
            kodeWarna = FOREGROUND_BLUE | FOREGROUND_GREEN;
            break;
        case LatarHijau:
            kodeWarna = BACKGROUND_GREEN;
            break;
        case Bawaan:
            kodeWarna = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        default:
            return;
            break;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), kodeWarna);
#else
        string kodeWarna;
        switch (warna)
        {
        case Merah:
            kodeWarna = "31";
            break;
        case Hijau:
            kodeWarna = "32";
            break;
        case Kuning:
            kodeWarna = "33";
            break;
        case Biru:
            kodeWarna = "36";
            break;
        case LatarHijau:
            kodeWarna = "42";
            break;
        case Bawaan:
            kodeWarna = "0";
            break;
        default:
            return;
            break;
        }
        cout << "\033[" << kodeWarna << "m";
#endif
    }

    unsigned int jumlahKolom()
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

#define PANJANG_PROMPT 25

    void tampilkanPrompt(string pertanyaan)
    {
        tetapkanWarna(Kuning);
        cout << left << setw(PANJANG_PROMPT) << pertanyaan
             << ": ";
        tetapkanWarna(Bawaan);
    }

    void tampilkanError(string error)
    {
        tetapkanWarna(Merah);
        cout << error << endl;
        tetapkanWarna(Bawaan);
    }

#define HEADING_PADDING 2

    void tampilkanHeading(string nama)
    {
        cout << setfill(' ');
        const int margin = jumlahKolom() / 2 - nama.length() / 2 - HEADING_PADDING;
        cout << setw(margin) << "";
        tetapkanWarna(LatarHijau);
        cout << setw(HEADING_PADDING) << "" << nama << setw(HEADING_PADDING) << "";
        tetapkanWarna(Bawaan);
        cout << endl
             << endl;
    }

    enum Navigasi
    {
        Enter,
        Atas,
        Bawah,
        TidakDikenal
    };

#ifndef _WIN32
    termios lastTermiosAttr;
#endif

    void aktifkanInputNavigasi()
    {
#ifndef _WIN32
        termios attr;
        tcgetattr(STDIN_FILENO, &attr);
        lastTermiosAttr = attr;
        attr.c_lflag &= ~ICANON & ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &attr);
#endif
    }

    void nonaktifkanInputNavigasi()
    {
#ifndef _WIN32
        termios attr;
        tcsetattr(STDIN_FILENO, TCSANOW, &lastTermiosAttr);
#endif
    }

    Navigasi inputNavigasi()
    {
#ifndef _WIN32
        char input;

        cin >> noskipws >> input >> skipws;
        if (input == '\x1b')
        {
            cin >> input;
            if (input == '[')
            {
                cin >> input;
                if (input == 'A')
                    return Atas;
                else if (input == 'B')
                    return Bawah;
                else
                    return TidakDikenal;
            }
            else
            {
                return TidakDikenal;
            }
        }
        else if (input == '\n')
        {
            return Enter;
        }
        else
        {
            return TidakDikenal;
        }
#else
        HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

        DWORD events = 0;
        DWORD eventRead = 0;

        do
        {
            GetNumberOfConsoleInputEvents(hStdIn, &events);
        } while (events == 0);
        INPUT_RECORD eventBuffer[events];
        ReadConsoleInput(hStdIn, eventBuffer, events, &eventRead);

        for (DWORD i = 0; i < eventRead; i++)
        {
            if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown)
            {
                switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode)
                {
                case VK_UP:
                    return Atas;
                case VK_DOWN:
                    return Bawah;
                case VK_RETURN:
                    return Enter;
                }
            }
        }

        return TidakDikenal;
#endif
    }

#ifdef _WIN32
    COORD lastCursorPos;
#endif

    void simpanPosisiCursor()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(hStdOut, &csbi);
        lastCursorPos = csbi.dwCursorPosition;
#else
        cout << "\x1b[s";
#endif
    }

    void kembalikanPosisiCursorDanHapus()
    {
#ifdef _WIN32
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hStdOut, &csbi);
        DWORD length = csbi.dwSize.X * csbi.dwSize.Y - lastCursorPos.X * lastCursorPos.Y;
        DWORD writtenLength = 0;
        FillConsoleOutputCharacter(hStdOut, ' ', length, lastCursorPos, &writtenLength);
        SetConsoleCursorPosition(hStdOut, lastCursorPos);
#else
        cout << "\x1b[u";
        cout << "\x1b[0J";
#endif
    }

    int pemilihInteraktif(vector<string> pilihan)
    {
        aktifkanInputNavigasi();
        Navigasi nav;
        bool redraw = false;
        string prompt = "(Tombol Panah/Atas Untuk Berpindah dan Enter untuk Memilih)";
        int currentIndex = 0;

        cout << prompt << endl;
        do
        {
            if (redraw)
            {
                kembalikanPosisiCursorDanHapus();
            }
            simpanPosisiCursor();

            for (int i = 0; i < pilihan.size(); i++)
            {
                if (i == currentIndex)
                {
                    UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Biru);
                    cout << "> ";
                }
                else
                    cout << "  ";
                cout << pilihan.at(i);
                if (i == currentIndex)
                {
                    cout << " (Terpilih)";
                    UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
                }
                cout << endl;
            }

            redraw = true;
            nav = inputNavigasi();

            if (nav == Atas)
                currentIndex--;
            else if (nav == Bawah)
                currentIndex++;
            currentIndex = currentIndex % pilihan.size();
        } while (nav != Enter);

        nonaktifkanInputNavigasi();
        return currentIndex;
    }

    void lineBorder()
    {
        UtilitasTerminal::tetapkanWarna(Kuning);
        cout << setw(jumlahKolom()) << setfill('-') << "" << endl;
        UtilitasTerminal::tetapkanWarna(Bawaan);
    }

    int columnBorder(bool leftMargin, bool rightMargin)
    {
        int result = 0;
        UtilitasTerminal::tetapkanWarna(Kuning);
        if (leftMargin)
        {
            cout << " ";
            result++;
        }
        cout << "|";
        result++;
        if (rightMargin)
        {
            cout << " ";
            result++;
        }
        UtilitasTerminal::tetapkanWarna(Bawaan);
        return result;
    }
}

const string FORMAT_TANGGAL = "dd/mm/yyyy";
class Tanggal
{
private:
    int mTanggal;
    int mBulan;
    int mTahun;

public:
    Tanggal(int tanggal, int bulan, int tahun) : mTanggal(tanggal), mBulan(bulan), mTahun(tahun)
    {
    }

    static Tanggal sekarang()
    {
        time_t detik = time(NULL);
        tm *dataWaktu = localtime(&detik);
        return Tanggal(dataWaktu->tm_mday, dataWaktu->tm_mon + 1, dataWaktu->tm_year + 1900);
    }

    static int jarakHari(Tanggal dari, Tanggal ke)
    {
        time_t jarakDetik = ke.keDetik() - dari.keDetik();
        return jarakDetik / 60 / 60 / 24;
    }

    time_t keDetik()
    {
        tm dataWaktu;
        dataWaktu.tm_year = mTahun - 1900;
        dataWaktu.tm_mon = mBulan - 1;
        dataWaktu.tm_mday = mTanggal;
        dataWaktu.tm_hour = 0;
        dataWaktu.tm_min = 0;
        dataWaktu.tm_sec = 0;
        dataWaktu.tm_isdst = -1;
        return mktime(&dataWaktu);
    }

    string toString()
    {
        stringstream stream;
        stream << right << setfill('0');
        stream << setw(2) << mTanggal << "/";
        stream << setw(2) << mBulan << "/";
        stream << setw(4) << mTahun;
        return stream.str();
    }

    int getTahun()
    {
        return this->mTahun;
    }
};

#define PESAN_VISIBILITAS_PUBLIK "Publik (Semua orang bisa mengakses)"
#define PESAN_VISIBILITAS_PRIVAT "Privat (Hanya anda yang bisa mengakses)"
#define PESAN_VISIBILITAS_TERPROTEKSI "Terproteksi (Hanya orang yang anda pilih yang bisa mengakses)"

enum Visibilitas
{
    VisibilitasPublik,
    VisibilitasPrivat,
    VisibilitasTerproteksi,
};

class Postingan
{
public:
    Postingan() : tanggalTerbit(Tanggal(0, 0, 0))
    {
    }

    string id;
    string judul;
    string tags;
    Visibilitas visibilitas;
    string orangYangDiizinkan;
    Tanggal tanggalTerbit;
    string konten;
};

#define PROMPT_ID "ID"
#define PROMPT_JUDUL "Judul"
#define PROMPT_TAG "Tag"
#define PROMPT_TANGGAL_TERBIT "Tanggal Terbit"
#define PROMPT_VISIBILITAS "Visibilitas"
#define PROMPT_KONTEN "Konten"
#define PROMPT_ORANG_DIIZINKAN "Orang yang Diizinkan"

class TambahPostingan
{
private:
    /*
        Menginput id
        id tidak boleh berisi spasi atau kosong
        Jika berisi spasi atau kosong, maka input diulangi
        Tidak menggunakan cin >> karena saat ada spasi karakter setelah spasi
        akan diberikan ke pertanyaan selanjutnya
    */

    void inputId()
    {
        while (true)
        {
            UtilitasTerminal::tampilkanPrompt(PROMPT_ID);
            getline(cin, postingan.id);

            if (postingan.id.find(' ') != string::npos)
            {
                UtilitasTerminal::tampilkanError("Tidak boleh ada spasi di dalam ID");
            }
            else if (postingan.id.length() == 0)
            {
                UtilitasTerminal::tampilkanError("ID tidak boleh kosong");
            }
            else
            {
                break;
            }
        }
    }

    /*
        Menginput Judul
        judul tidak boleh kosong
        Jika kosong, maka input diulangi
        Tidak menggunakan cin >> tidak mendukung spasi
    */

    void inputJudul()
    {
        while (true)
        {
            UtilitasTerminal::tampilkanPrompt(PROMPT_JUDUL);
            getline(cin, postingan.judul);
            if (postingan.judul.length() == 0)
            {

                UtilitasTerminal::tampilkanError("Judul tidak boleh kosong.");
            }
            else
            {
                break;
            }
        }
    }

    /*
        Menginput tanggal
        Tanggal harus berformat dd/mm/yyy
        Jika format salah atau bukan angka, maka input diulangi
        Tidak menggunakan scanf("%02d/%02d/%04d") karena jika format salah maka
        input diberikan ke pertanyaan selanjutnya
    */

    void inputTanggalTerbit()
    {
        int tanggal;
        int bulan;
        int tahun;
        string input;

        while (true)
        {
            UtilitasTerminal::tampilkanPrompt(PROMPT_TANGGAL_TERBIT);
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Biru);
            cout << "(" << FORMAT_TANGGAL << ") ";
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
            getline(cin, input);

            // dd/mm/yyyy
            //   /  /
            // 0123456789

            if (input.length() != FORMAT_TANGGAL.length() || input.at(2) != '/' || input.at(5) != '/')
            {
                UtilitasTerminal::tampilkanError("Format tanggal tidak valid. Gunakan format");
                continue;
            }

            stringstream stream(input);

            stream >> tanggal;
            if (stream.get() != '/')
            {
                UtilitasTerminal::tampilkanError("Tanggal tidak sepenuhnya angka");
                continue;
            }

            stream >> bulan;
            if (stream.get() != '/')
            {
                UtilitasTerminal::tampilkanError("Bulan tidak sepenuhnya angka");
                continue;
            }

            stream >> tahun;
            if (!stream.eof())
            {
                UtilitasTerminal::tampilkanError("Tahun tidak sepenuhnya angka");
                continue;
            }

            if (tanggal < 1 || tanggal > 31)
            {
                UtilitasTerminal::tampilkanError("Tanggal hanya boleh dari 1 sampai 31");
            }
            else if (bulan < 1 || bulan > 12)
            {
                UtilitasTerminal::tampilkanError("Bulan hanya boleh dari 1 sampai 12");
            }
            else if (tahun < 1)
            {
                UtilitasTerminal::tampilkanError("Tahun tidak boleh kurang dari 1");
            }
            else
            {
                break;
            }
        }

        postingan.tanggalTerbit = Tanggal(tanggal, bulan, tahun);
    }

    /*
        Menginput banyak baris
        Baris kosong digunakan untuk mengakhiri input
    */

    void inputKonten()
    {

        UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Kuning);
        cout << "Tulis Konten ";
        UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
        cout << "(Mendukung Banyak Baris. Akhiri dengan Baris Kosong)" << endl;

        postingan.konten = "";
        string baris;

        do
        {
            getline(cin, baris);
            if (postingan.konten.length() > 0)
                postingan.konten.append("\n");
            postingan.konten.append(baris);
        } while (baris.length() != 0);
    }

    void inputVisibilitas()
    {
        vector<string> daftarPilihan;
        daftarPilihan.push_back(PESAN_VISIBILITAS_PUBLIK);
        daftarPilihan.push_back(PESAN_VISIBILITAS_PRIVAT);
        daftarPilihan.push_back(PESAN_VISIBILITAS_TERPROTEKSI);

        UtilitasTerminal::tampilkanPrompt(PROMPT_VISIBILITAS);
        cout << endl;

        int pilihan = UtilitasTerminal::pemilihInteraktif(daftarPilihan);

        switch (pilihan)
        {
        case 0:
            postingan.visibilitas = VisibilitasPublik;
            break;
        case 1:
            postingan.visibilitas = VisibilitasPrivat;
            break;
        case 2:
            postingan.visibilitas = VisibilitasTerproteksi;
            break;
        }
    }

public:
    Postingan postingan;
    void run()
    {
        UtilitasTerminal::tampilkanHeading("Tambah Postingan");

        inputId();

        inputJudul();

        UtilitasTerminal::tampilkanPrompt(PROMPT_TAG);
        getline(cin, postingan.tags);

        inputTanggalTerbit();

        inputVisibilitas();

        if (postingan.visibilitas == VisibilitasTerproteksi)
        {
            UtilitasTerminal::tampilkanPrompt(PROMPT_ORANG_DIIZINKAN);
            getline(cin, postingan.orangYangDiizinkan);
        }

        inputKonten();

        cout << endl;

        Tanggal tanggalSekarang = Tanggal::sekarang();
        cout << "Sekarang tanggal " << tanggalSekarang.toString() << endl;

        int jarak = Tanggal::jarakHari(tanggalSekarang, postingan.tanggalTerbit);

        if (jarak == 0)
        {
            cout << "Hari ini adalah hari penerbitan postingan anda" << endl;
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Hijau);
            cout << "Selamat postingan anda telah diterbitkan";
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
        }
        else if (jarak < 0)
        {
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Kuning);
            cout << "Tanggal penerbitan adalah " << (-jarak) << " hari yang lalu" << endl;
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Hijau);
            cout << "Postingan langsung diterbitkan" << endl;
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
        }
        else
        {
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Hijau);
            cout << "Postingan akan diterbitkan " << jarak << " hari lagi" << endl;
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
        }
        cout << endl;
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }
};

bool login()
{
    UtilitasTerminal::tampilkanHeading("Login");

    string username;
    string password;
    bool success = false;

    for (int i = 1; i <= 3; i++)
    {
        UtilitasTerminal::simpanPosisiCursor();
        if (i != 1)
        {
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Merah);
            cout << "Password salah" << endl;
            UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
            cout << "Percobaan ke " << i << endl;
            cout << endl;
        }
        UtilitasTerminal::tampilkanPrompt("Username");
        getline(cin, username);
        UtilitasTerminal::tampilkanPrompt("Password");
        getline(cin, password);

        UtilitasTerminal::kembalikanPosisiCursorDanHapus();
        if (username == "admin" && password == "admin")
        {
            success = true;
            break;
        }
    }

    if (success)
    {
        UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Hijau);
        cout << "Login berhasil" << endl;
    }
    else
    {
        UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Merah);
        cout << "Login gagal" << endl;
    }
    UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
    cout << endl;

    return success;
}

vector<Postingan> buatDaftarPostingan()
{
    vector<Postingan> hasil;

    Postingan postingan1;
    postingan1.id = "web-js";
    postingan1.judul = "Web Interaktif dengan JS";
    postingan1.visibilitas = VisibilitasPublik;
    postingan1.konten = "JS adalah bahasa pemrograman";
    postingan1.tags = "js,web";
    postingan1.tanggalTerbit = Tanggal(21, 05, 2020);
    hasil.push_back(postingan1);

    Postingan postingan2;
    postingan2.id = "css-responsive";
    postingan2.judul = "Web Responsive dengan CSS";
    postingan2.visibilitas = VisibilitasPublik;
    postingan2.konten = "Web Media Query adalah kunci!";
    postingan2.tags = "css,web";
    postingan2.tanggalTerbit = Tanggal(22, 05, 2020);
    hasil.push_back(postingan2);

    Postingan postingan3;
    postingan3.id = "cpp-iomanip";
    postingan3.judul = "Manipulasi IO di C++";
    postingan3.visibilitas = VisibilitasPublik;
    postingan3.konten = "Lobrary IO Manip adalah kunci!";
    postingan3.tags = "cpp,io";
    postingan3.tanggalTerbit = Tanggal(23, 05, 2020);
    hasil.push_back(postingan3);

    return hasil;
}

void tampilkanDaftarPostingan(vector<Postingan> daftar)
{
    UtilitasTerminal::tampilkanHeading("Daftar Postingan");

    vector<vector<string> > grid;

    vector<string> headerRow;
    headerRow.push_back("ID");
    headerRow.push_back("Judul");

    grid.push_back(headerRow);

    for (vector<Postingan>::iterator it = daftar.begin(); it != daftar.end(); it++)
    {
        vector<string> currentRow;
        currentRow.push_back(it->id);
        currentRow.push_back(it->judul);
        grid.push_back(currentRow);
    }

    int panjangKolomId = 0;

    for (vector<vector<string> >::iterator it = grid.begin(); it != grid.end(); it++)
    {
        int panjangId = it->at(0).length();
        if (panjangId > panjangKolomId)
            panjangKolomId = panjangId;
    }

    const int kolomMaks = UtilitasTerminal::jumlahKolom();
    UtilitasTerminal::lineBorder();
    for (vector<vector<string> >::iterator it = grid.begin(); it != grid.end(); it++)
    {
        bool isHeader = it == grid.begin();
        int writtenCount = 0;
        writtenCount += UtilitasTerminal::columnBorder(false, true);
        UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Biru);
        cout << setfill(' ');
        cout << setw(panjangKolomId) << it->at(0);
        writtenCount += panjangKolomId;
        writtenCount += UtilitasTerminal::columnBorder(true, true);
        UtilitasTerminal::tetapkanWarna(isHeader ? UtilitasTerminal::Biru : UtilitasTerminal::Bawaan);
        cout << setfill(' ');
        cout << setw(kolomMaks - writtenCount - 1) << it->at(1);
        UtilitasTerminal::columnBorder(false, false);
        UtilitasTerminal::lineBorder();
    }

    UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
}

class LihatPostingan
{
private:
    Postingan &mPostingan;

    static unsigned int hitungJumlahKata(string str)
    {
        unsigned int jumlah = 0;
        bool didalamString = false;
        for (string::iterator it = str.begin(); it != str.end(); it++)
        {
            if (*it == ' ' || *it == '\r' || *it == '\n')
            {
                didalamString = false;
            }
            else if (!didalamString)
            {
                didalamString = true;
                jumlah++;
            }
        }
        return jumlah;
    }

public:
    LihatPostingan(Postingan &postingan) : mPostingan(postingan)
    {
    }

    void run()
    {

        UtilitasTerminal::tampilkanHeading("Pratinjau Postingan");

        UtilitasTerminal::tampilkanPrompt(PROMPT_ID);
        cout << mPostingan.id << endl;
        UtilitasTerminal::tampilkanPrompt(PROMPT_JUDUL);
        cout << mPostingan.judul << endl;

        UtilitasTerminal::tampilkanPrompt(PROMPT_TAG);
        cout << mPostingan.tags << endl;

        UtilitasTerminal::tampilkanPrompt(PROMPT_TANGGAL_TERBIT);
        cout << mPostingan.tanggalTerbit.toString() << endl;

        UtilitasTerminal::tampilkanPrompt("Visibilitas");
        switch (mPostingan.visibilitas)
        {
        case VisibilitasPublik:
            cout << PESAN_VISIBILITAS_PUBLIK;
            break;
        case VisibilitasPrivat:
            cout << PESAN_VISIBILITAS_PRIVAT;
            break;
        case VisibilitasTerproteksi:
            cout << PESAN_VISIBILITAS_TERPROTEKSI;
            break;
        default:
            break;
        }
        cout << endl;

        if (mPostingan.visibilitas == VisibilitasTerproteksi)
        {
            UtilitasTerminal::tampilkanPrompt(PROMPT_ORANG_DIIZINKAN);
            cout << mPostingan.orangYangDiizinkan << endl;
        }

        UtilitasTerminal::tampilkanPrompt("Jumlah Kata");
        int jumlahKata = hitungJumlahKata(mPostingan.konten);
        cout << jumlahKata << " (";
        if (jumlahKata < 50)
            cout << "Sedikit";
        else if (jumlahKata < 100)
            cout << "Sedang";
        else
            cout << "Banyak";
        cout << ")" << endl;

        UtilitasTerminal::tampilkanPrompt(PROMPT_KONTEN);
        cout << endl
             << mPostingan.konten;
        cout << endl;
    }
};

int main()
{

    UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Hijau);
    cout << "Dibuat oleh ";
    UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Biru);
    cout << "Levi Rizki Saputra ";
    UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Kuning);
    cout << "(" << 123230127 << ")";
    UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
    cout << endl
         << endl;

    if (!login())
        return 0;

    vector<Postingan> daftarPostingan = buatDaftarPostingan();

    vector<string> daftarPilihan;
    daftarPilihan.push_back("Lihat Daftar Postingan");
    daftarPilihan.push_back("Lihat Detail Postingan");
    daftarPilihan.push_back("Tambah Postingan");
    daftarPilihan.push_back("Logout");

    int pilihan;
    do
    {
        UtilitasTerminal::tampilkanHeading("Aksi");
        pilihan = UtilitasTerminal::pemilihInteraktif(daftarPilihan);

        if (pilihan == 0)
        {
            tampilkanDaftarPostingan(daftarPostingan);
        }
        else if (pilihan == 1)
        {
            UtilitasTerminal::tampilkanHeading("Pilih Postingan");
            vector<string> pilihanPostingan;

            for (vector<Postingan>::iterator it = daftarPostingan.begin(); it != daftarPostingan.end(); it++)
            {
                pilihanPostingan.push_back(it->judul);
            }

            Postingan postinganTerpilih = daftarPostingan.at(UtilitasTerminal::pemilihInteraktif(pilihanPostingan));
            LihatPostingan form(postinganTerpilih);
            form.run();
        }
        else if (pilihan == 2)
        {
            TambahPostingan form;
            form.run();
            daftarPostingan.push_back(form.postingan);
        }
    } while (pilihan != 3);

    return 0;
}