#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
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

class Mahasiswa
{
public:
    Mahasiswa() : tanggalLahir(Tanggal(0, 0, 0)) {}
    int noPresensi;
    string nim;
    string nama;
    Tanggal tanggalLahir;
    string noHP;
    string alamat;
    string prodi;
    char kelas;
    float beratBadan;
    float tinggiBadan;
};

#define NO_PRESENSI_PROMPT "NO Presensi"
#define NIM_PROMPT "NIM"
#define NAMA_PROMPT "Nama"
#define NO_HP_PROMPT "NO HP"
#define ALAMAT_PROMPT "Alamat"
#define PRODI_PROMPT "Program Studi"
#define KELAS_PROMPT "Kelas"
#define BERAT_BADAN_PROMPT "Berat Badan"
#define TINGGI_BADAN_PROMPT "Tinggi Badan"
#define TANGGAL_LAHIR_PROMPT "Tanggal Lahir"
#define UMUR_PROMPT "Umur"

class MahasiswaInputForm
{
    typedef bool (MahasiswaInputForm::*Validator)(string);

private:
    string input_string(string prompt, vector<Validator> validators, string extra_info = "")
    {
        string hasil;

        bool valid;
        do
        {
            UtilitasTerminal::tampilkan_prompt(prompt);
            if (extra_info.length() > 0)
            {
                UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Biru);
                cout << "(" << extra_info << ") ";
                UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Bawaan);
            }
            getline(cin, hasil);

            valid = true;
            for (vector<Validator>::iterator i = validators.begin(); i != validators.end(); i++)
            {
                if (!((*this).*(*i))(hasil))
                {
                    valid = false;
                    break;
                }
            }
        } while (!valid);
        return hasil;
    }

    bool validate_tidak_kosong(string str)
    {
        if (str.length() == 0)
        {
            UtilitasTerminal::tampilkan_error("Tidak boleh kosong");
            return false;
        }
        return true;
    }

    bool validate_satu_karakter(string str)
    {

        if (str.length() != 1)
        {
            UtilitasTerminal::tampilkan_error("Hanya boleh 1 karakter");
            return false;
        }
        return true;
    }

    // NO HP dan NIM kemungkinan besar terlalu besar untuk disimpan di int
    bool validate_hanya_angka(string str)
    {
        for (string::iterator it = str.begin(); it != str.end(); it++)
        {
            if (*it < 48 || *it > 57)
            {
                UtilitasTerminal::tampilkan_error("Hanya boleh berisi bilangan bulat");
                return false;
            }
        }

        return true;
    }

    float input_desimal_terakhir;

    bool validate_bilangan_desimal(string str)
    {
        stringstream stream(str);
        stream >> input_desimal_terakhir;
        if (!stream.eof())
        {
            UtilitasTerminal::tampilkan_error("Hanya boleh berisi bilangan desimal");
            return false;
        }
        return true;
    }

    const string format_tanggal;

    int input_tanggal_terakhir;
    int input_bulan_terakhir;
    int input_tahun_terakhir;

    bool validate_tanggal(string str)
    {
        if (str.length() != format_tanggal.length() || str.at(2) != '/' || str.at(5) != '/')
        {
            UtilitasTerminal::tampilkan_error("Format tanggal tidak valid. Gunakan format " + format_tanggal);
            return false;
        }

        stringstream stream(str);

        stream >> input_tanggal_terakhir;
        if (stream.get() != '/')
        {
            UtilitasTerminal::tampilkan_error("Tanggal tidak sepenuhnya angka");
            return false;
        }

        stream >> input_bulan_terakhir;
        if (stream.get() != '/')
        {
            UtilitasTerminal::tampilkan_error("Bulan tidak sepenuhnya angka");
            return false;
        }

        stream >> input_tahun_terakhir;
        if (!stream.eof())
        {
            UtilitasTerminal::tampilkan_error("Tahun tidak sepenuhnya angka");
            return false;
        }

        if (input_tanggal_terakhir < 1 || input_tanggal_terakhir > 31)
        {
            UtilitasTerminal::tampilkan_error("Tanggal hanya boleh dari 1 sampai 31");
            return false;
        }

        if (input_bulan_terakhir < 1 || input_bulan_terakhir > 12)
        {
            UtilitasTerminal::tampilkan_error("Bulan hanya boleh dari 1 sampai 12");
            return false;
        }

        if (input_tahun_terakhir < 1)
        {
            UtilitasTerminal::tampilkan_error("Tahun tidak boleh kurang dari 1");
            return false;
        }

        return true;
    }

public:
    MahasiswaInputForm() : format_tanggal("dd/mm/yyyy")
    {
    }

    void showHeading()
    {
        UtilitasTerminal::tampilkan_heading("Input Mahasiswa");
    }

    int inputNoPresensi()
    {
        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        validators.push_back(&MahasiswaInputForm::validate_hanya_angka);
        int output;
        stringstream stream(input_string(NO_PRESENSI_PROMPT, validators));
        stream >> output;
        return output;
    }

    string inputNIM()
    {
        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        validators.push_back(&MahasiswaInputForm::validate_hanya_angka);
        return input_string(NIM_PROMPT, validators);
    }

    string inputNama()
    {
        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        return input_string(NAMA_PROMPT, validators);
    }

    string inputNoHP()
    {
        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        validators.push_back(&MahasiswaInputForm::validate_hanya_angka);
        return input_string(NO_HP_PROMPT, validators);
    }

    string inputAlamat()
    {
        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        return input_string(ALAMAT_PROMPT, validators);
    }

    string inputProdi()
    {
        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        return input_string(PRODI_PROMPT, validators);
    }

    char inputKelas()
    {
        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        validators.push_back(&MahasiswaInputForm::validate_satu_karakter);
        return input_string(KELAS_PROMPT, validators).at(0);
    }

    float inputBeratBadan()
    {

        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        validators.push_back(&MahasiswaInputForm::validate_bilangan_desimal);
        input_string(BERAT_BADAN_PROMPT, validators);
        return input_desimal_terakhir;
    }

    float inputTinggiBadan()
    {

        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        validators.push_back(&MahasiswaInputForm::validate_bilangan_desimal);
        input_string(TINGGI_BADAN_PROMPT, validators);
        return input_desimal_terakhir;
    }

    Tanggal inputTanggalLahir()
    {

        vector<Validator> validators;
        validators.push_back(&MahasiswaInputForm::validate_tidak_kosong);
        validators.push_back(&MahasiswaInputForm::validate_tanggal);
        input_string(TANGGAL_LAHIR_PROMPT, validators, format_tanggal);
        return Tanggal(input_tanggal_terakhir, input_bulan_terakhir, input_tahun_terakhir);
    }
};

class MahasiswaViewForm
{
public:
    void showHeading()
    {
        UtilitasTerminal::tampilkan_heading("Identitas Mahasiswa");
    }

    void showMahasiswa(Mahasiswa mhs)
    {
        UtilitasTerminal::tampilkan_prompt(NO_PRESENSI_PROMPT);
        cout << mhs.noPresensi << endl;

        UtilitasTerminal::tampilkan_prompt(NIM_PROMPT);
        cout << mhs.nim << endl;

        UtilitasTerminal::tampilkan_prompt(NAMA_PROMPT);
        cout << mhs.nama << endl;

        UtilitasTerminal::tampilkan_prompt(TANGGAL_LAHIR_PROMPT);
        cout << mhs.tanggalLahir.toString() << endl;

        UtilitasTerminal::tampilkan_prompt(UMUR_PROMPT);
        cout << 2023 - mhs.tanggalLahir.getTahun() << endl;

        UtilitasTerminal::tampilkan_prompt(NO_HP_PROMPT);
        cout << mhs.noHP << endl;

        UtilitasTerminal::tampilkan_prompt(ALAMAT_PROMPT);
        cout << mhs.alamat << endl;

        UtilitasTerminal::tampilkan_prompt(PRODI_PROMPT);
        cout << mhs.prodi << endl;

        UtilitasTerminal::tampilkan_prompt(KELAS_PROMPT);
        cout << mhs.kelas << endl;

        UtilitasTerminal::tampilkan_prompt(BERAT_BADAN_PROMPT);
        cout << mhs.beratBadan << endl;

        UtilitasTerminal::tampilkan_prompt(TINGGI_BADAN_PROMPT);
        cout << mhs.tinggiBadan << endl;
    }
};

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

    Mahasiswa mhs;

    MahasiswaInputForm inputForm;
    inputForm.showHeading();
    mhs.noPresensi = inputForm.inputNoPresensi();
    mhs.nim = inputForm.inputNIM();
    mhs.nama = inputForm.inputNama();
    mhs.tanggalLahir = inputForm.inputTanggalLahir();
    mhs.noHP = inputForm.inputNoHP();
    mhs.alamat = inputForm.inputAlamat();
    mhs.prodi = inputForm.inputProdi();
    mhs.kelas = inputForm.inputKelas();
    mhs.beratBadan = inputForm.inputBeratBadan();
    mhs.tinggiBadan = inputForm.inputTinggiBadan();

    cout << endl;
    cout << "Menyimpan data" << endl;
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Hijau);
    cout << "Data telah disimpan" << endl;
    UtilitasTerminal::tetapkan_warna(UtilitasTerminal::Bawaan);
    cout << endl;

    MahasiswaViewForm viewForm;
    viewForm.showHeading();
    viewForm.showMahasiswa(mhs);
    return 0;
}