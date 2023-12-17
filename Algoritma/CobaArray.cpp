#include <iostream>

	const char ESC = '\x1b';
	const char CSI[3] = {ESC, '[', '\0'};

int main() {
	char A = 'A';
	char B[] = {A, 'B'};

	
	std::cout << CSI << "36m" << "AAA" << CSI << "0m";
}
