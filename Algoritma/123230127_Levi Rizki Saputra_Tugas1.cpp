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

#ifdef _WIN32
void windows_tetapkan_warna(WORD warna)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), warna);
}
#else
void ansi_tetapkan_warna(string warna)
{
  cout << "\033[" << warna << "m";
}
#endif

void warna_merah()
{
#ifdef _WIN32
  windows_tetapkan_warna(FOREGROUND_RED);
#else
  ansi_tetapkan_warna("31");
#endif
}

void warna_biru()
{
#ifdef _WIN32
  windows_tetapkan_warna(FOREGROUND_BLUE | FOREGROUND_GREEN);
#else
  ansi_tetapkan_warna("36");
#endif
}

void warna_kuning()
{
#ifdef _WIN32
  windows_tetapkan_warna(FOREGROUND_RED | FOREGROUND_GREEN);
#else
  ansi_tetapkan_warna("33");
#endif
}

void warna_latar_hijau()
{
#ifdef _WIN32
  windows_tetapkan_warna(BACKGROUND_GREEN);
#else
  ansi_tetapkan_warna("42");
#endif
}

void warna_hijau()
{
#ifdef _WIN32
  windows_tetapkan_warna(FOREGROUND_GREEN);
#else
  ansi_tetapkan_warna("32");
#endif
}

void warna_bawaan()
{
#ifdef _WIN32
  windows_tetapkan_warna(7);
#else
  ansi_tetapkan_warna("0");
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

#define PANJANG_PROMPT 15

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

void tampilkan_prompt(string pertanyaan)
{

  warna_kuning();
  cout << left << setw(PANJANG_PROMPT) << pertanyaan
       << ": ";
  warna_bawaan();
}

#define HEADING_PADDING 2

void tampilkan_heading(string nama)
{
  const int margin = panjang_terminal() / 2 - nama.length() / 2 - HEADING_PADDING;
  cout << setw(margin) << "";
  warna_latar_hijau();
  cout << setw(HEADING_PADDING) << "" << nama << setw(HEADING_PADDING) << "";
  warna_bawaan();
  cout << endl
       << endl;
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
    tampilkan_prompt(prompt);
    getline(cin, id);

    if (id.find(' ') != string::npos)
    {
      warna_merah();
      cout << "Tidak boleh ada spasi di dalam ID" << endl;
      warna_bawaan();
      continue;
    }

    if (id.length() == 0)
    {
      warna_merah();
      cout << "ID tidak boleh kosong" << endl;
      warna_bawaan();
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
    tampilkan_prompt(prompt);
    getline(cin, judul);
    if (judul.length() == 0)
    {
      warna_merah();
      cout << "Judul tidak boleh kosong." << endl;
      warna_bawaan();
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
    tampilkan_prompt(prompt);
    warna_biru();
    cout << "(" << format_tanggal << ") ";
    warna_bawaan();
    getline(cin, input);

    // dd/mm/yyyy
    //   /  /
    // 0123456789

    if (input.length() != format_tanggal.length() || input.at(2) != '/' || input.at(5) != '/')
    {
      warna_merah();
      cout << "Format tanggal tidak valid. Gunakan format " << format_tanggal << endl;
      warna_bawaan();
      continue;
    }

    if (!string_ke_angka(input.substr(0, 2), tanggal))
    {
      warna_merah();
      cout << "Tanggal tidak sepenuhnya angka" << endl;
      warna_bawaan();
      continue;
    }
    if (!string_ke_angka(input.substr(3, 2), bulan))
    {
      warna_merah();
      cout << "Bulan tidak sepenuhnya angka" << endl;
      warna_bawaan();
      continue;
    }
    if (!string_ke_angka(input.substr(6, 4), tahun))
    {
      warna_merah();
      cout << "Tahun tidak sepenuhnya angka" << endl;
      warna_bawaan();
      continue;
    }

    if (*tanggal < 1 || *tanggal > 31)
    {
      warna_merah();
      cout << "Tanggal hanya boleh dari 1 sampai 31" << endl;
      warna_bawaan();
      continue;
    }

    if (*bulan < 1 || *bulan > 12)
    {
      warna_merah();
      cout << "Bulan hanya boleh dari 1 sampai 12" << endl;
      warna_bawaan();
      continue;
    }

    if (*tahun < 1)
    {
      warna_merah();
      cout << "Tahun tidak boleh kurang dari 1" << endl;
      warna_bawaan();
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
  warna_hijau();
  cout << "Dibuat oleh ";
  warna_biru();
  cout << "Levi Rizki Saputra ";
  warna_kuning();
  cout << "(" << 123230127 << ")";
  warna_bawaan();
  cout << endl
       << endl;

  tampilkan_heading("Form Penerbitan Postingan");

  string id = input_id(PROMPT_ID);

  string judul = input_judul(PROMPT_JUDUL);

  tampilkan_prompt(PROMPT_TAG);
  string tags;
  getline(cin, tags);

  int tanggal;
  int bulan;
  int tahun;
  input_tanggal(PROMPT_TANGGAL_TERBIT, &tanggal, &bulan, &tahun);

  warna_kuning();
  cout << "Tulis Konten ";
  warna_bawaan();
  cout << "(Mendukung Banyak Baris. Akhiri dengan Baris Korong)" << endl;

  string konten = input_banyak_baris();

  cout << endl;
  warna_hijau();
  cout << "Selamat postingan anda telah diterbitkan";
  warna_bawaan();
  cout << endl
       << endl;

  tampilkan_heading("Pratinjau Postingan");

  tampilkan_prompt(PROMPT_ID);
  cout << id << endl;

  tampilkan_prompt(PROMPT_JUDUL);
  cout << judul << endl;

  tampilkan_prompt(PROMPT_JUDUL);
  cout << tags << endl;

  tampilkan_prompt(PROMPT_TANGGAL_TERBIT);
  cout << right << setfill('0')
       << setw(2) << tanggal << "/"
       << setw(2) << bulan << "/"
       << setw(4) << tahun << endl
       << setfill(' ') << left;

  tampilkan_prompt("Konten");
  cout << endl
       << konten;

  return 0;
}
