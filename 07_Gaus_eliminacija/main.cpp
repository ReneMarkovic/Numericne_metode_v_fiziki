#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Gauss_eliminacija.h"

using namespace std;

int main() {
	vector<vector <double>> A;
	vector <double> R;

	int preberi = 0;
	cout << "Ali želite prebrati datoteko 'matrix.dat' (0 - Ne, 1 - Da): ";
	cin >> preberi;

	if (preberi == 0) {
		vnesi_matriko_in_vektor(A);
		shrani_matriko_v_datoteko(A);
	}else {
		A = preberi_matriko_iz_datoteke("matrix.dat");
	}


	izpisi_matriko_na_terminal(A);
	R=Gauss_eliminacija(A);
	izpis_resitev(R);
	getchar();
	return 0;
}


