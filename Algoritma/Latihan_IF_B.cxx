#include <iostream>

using namespace std;

struct User {
	string nama;
	string userName;
	string password;
};

User userList[100];
int jumlahUser;

void userRegister() {
	User user;
	cout << "Register" << endl;

	cout << "Nama\t\t: ";
	getline(cin, user.nama);
	
	cout << "Username\t: ";
	getline(cin, user.userName);
	
	cout << "Password\t: ";
	getline(cin, user.password);
	
	userList[jumlahUser] = user;
	jumlahUser++;
	
	cout << "User berhasil register" << endl;
}

bool userLogin() {
	cout << "Login" << endl;
	string username;
	string password;
	for (int i = 3; i > 0; i--) {
		if (i != 3)
			cout << i << " percobaan tersisa" << endl;
			
		cout << "Username\t: ";
		getline(cin, username);
		
		cout << "Password\t: ";
		getline(cin, password);
		
		for (int i = 0; i < jumlahUser; i++) {
			if (username == userList[i].userName 
				&& password == userList[i].password) {
				
				cout << "Anda berhasil login" << endl;
				return true;
			}
		}
		
		cout << "Password salah" << endl;
	}
	
	cout << "Anda gagal login" << endl;
	return false;
}

int main(int argc, char **argv)
{
	userList[0].nama = "Levi Rizki Saputra";
	userList[0].userName = "levi";
	userList[0].password = "lili";
	jumlahUser = 1;
	
	cout << "Menu User" << endl;
	cout << "1. Login" << endl;
	cout << "2. Register" << endl;
	
	cout << "Pilihan: ";
	int pilihanMenuUser;
	cin >> pilihanMenuUser;
	cin.ignore();
	
	bool berhasilLogin = false;
	if (pilihanMenuUser == 1) {
		berhasilLogin = userLogin();
	} else if (pilihanMenuUser == 2) {
		userRegister();
		berhasilLogin = userLogin();
	}
	
	if (!berhasilLogin)
		return 0;
		
	
	
	return 0;
}

