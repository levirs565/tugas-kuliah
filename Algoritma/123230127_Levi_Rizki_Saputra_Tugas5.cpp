#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
  int jumlahLadang;

  cout << "Jumlah ladang\t: ";

  cin >> jumlahLadang;
  cin.ignore();

  cout << "Panjang lebar ladang" << endl;

  int panjangLebar[jumlahLadang][2];

  for (int i = 0; i < jumlahLadang; i++)
  {
    string raw;
    getline(cin, raw);

    stringstream stream(raw);

    int a;
    int b;

    stream >> a;
    stream >> b;

    panjangLebar[i][0] = a;
    panjangLebar[i][1] = b;
  }

  cout << "Output" << endl;
  int result = 0;
  for (int i = 0; i < jumlahLadang; i++)
  {
    int min;
    if (panjangLebar[i][0] < panjangLebar[i][1])
      min = panjangLebar[i][0];
    else
      min = panjangLebar[i][1];

    int minSquared = min * min;
    cout << minSquared << endl;
    result += minSquared;
  }

  cout << result << endl;
}
