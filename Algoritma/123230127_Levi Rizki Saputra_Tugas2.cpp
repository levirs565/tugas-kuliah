/*
    Aplikasi untuk menginput data postingan
    kemudian mencetak data postingan
    Hasil output berwarna-warni
    Dibuat oleh Levi Rizki Saputra
*/

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <ctime>

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
    const int margin = jumlahKolom() / 2 - nama.length() / 2 - HEADING_PADDING;
    cout << setw(margin) << "";
    tetapkanWarna(LatarHijau);
    cout << setw(HEADING_PADDING) << "" << nama << setw(HEADING_PADDING) << "";
    tetapkanWarna(Bawaan);
    cout << endl
         << endl;
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

class PenerbitPostingan
{
private:
  Postingan &mPostingan;

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
      getline(cin, mPostingan.id);

      if (mPostingan.id.find(' ') != string::npos)
      {
        UtilitasTerminal::tampilkanError("Tidak boleh ada spasi di dalam ID");
      }
      else if (mPostingan.id.length() == 0)
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
      getline(cin, mPostingan.judul);
      if (mPostingan.judul.length() == 0)
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

    mPostingan.tanggalTerbit = Tanggal(tanggal, bulan, tahun);
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

    mPostingan.konten = "";
    string baris;

    do
    {
      getline(cin, baris);
      if (mPostingan.konten.length() > 0)
        mPostingan.konten.append("\n");
      mPostingan.konten.append(baris);
    } while (baris.length() != 0);
  }

  void inputVisibilitas()
  {
    string daftarPilihan[3] = {
        PESAN_VISIBILITAS_PUBLIK,
        PESAN_VISIBILITAS_PRIVAT,
        PESAN_VISIBILITAS_TERPROTEKSI};
    cout << "Pilihan Visibilitas (Pilih berdasarkan angka)" << endl;
    for (int i = 0; i < sizeof(daftarPilihan) / sizeof(string); i++)
    {
      UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Biru);
      cout << i + 1;
      UtilitasTerminal::tetapkanWarna(UtilitasTerminal::Bawaan);
      cout << ". " << daftarPilihan[i] << endl;
    }

    int pilihan;
    while (true)
    {
      UtilitasTerminal::tampilkanPrompt(PROMPT_VISIBILITAS);

      cin >> pilihan;

      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      if (cin.gcount() > 1)
      {
        UtilitasTerminal::tampilkanError("Tidak boleh mengandung selain angka");
      }
      else
      {
        bool valid = true;
        switch (pilihan)
        {
        case 1:
          mPostingan.visibilitas = VisibilitasPublik;
          break;
        case 2:
          mPostingan.visibilitas = VisibilitasPrivat;
          break;
        case 3:
          mPostingan.visibilitas = VisibilitasTerproteksi;
          break;
        default:
          valid = false;
          UtilitasTerminal::tampilkanError("Pilihan harus di antara 1 sampai 3");
          break;
        }
        if (valid)
          break;
      }
    }
  }

public:
  PenerbitPostingan(Postingan &postingan) : mPostingan(postingan)
  {
  }

  void run()
  {
    UtilitasTerminal::tampilkanHeading("Form Penerbitan Postingan");

    inputId();

    inputJudul();

    UtilitasTerminal::tampilkanPrompt(PROMPT_TAG);
    getline(cin, mPostingan.tags);

    inputTanggalTerbit();

    inputVisibilitas();

    if (mPostingan.visibilitas == VisibilitasTerproteksi)
    {
      UtilitasTerminal::tampilkanPrompt(PROMPT_ORANG_DIIZINKAN);
      getline(cin, mPostingan.orangYangDiizinkan);
    }

    inputKonten();

    cout << endl;

    Tanggal tanggalSekarang = Tanggal::sekarang();
    cout << "Sekarang tanggal " << tanggalSekarang.toString() << endl;

    int jarak = Tanggal::jarakHari(tanggalSekarang, mPostingan.tanggalTerbit);

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

class PratinjauPostingan
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
  PratinjauPostingan(Postingan &postingan) : mPostingan(postingan)
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

  Postingan postingan;

  PenerbitPostingan penerbit(postingan);
  penerbit.run();

  PratinjauPostingan pratinjau(postingan);
  pratinjau.run();

  return 0;
}
