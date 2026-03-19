#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Gauss_eliminacija.h"

/*
V tej datoteki definimarmo funkcije, katerih prototipi obstajajo v "Gauss_eliminacija.h"
*/

using namespace std;

void vnesi_matriko_in_vektor(vector<vector<double>>& A) {
	int i, j, N;
	cout << "Vnesite velikost matrike A: ";
	cin >> N;

	//N +1 je zato da imamo istočasno A in b
	A.resize(N, vector<double>(N + 1));

	cout << "Vnesite elemente matrike A\n";
	for (i = 0; i < N; i++) {//Vrstica
		for (j = 0; j < N; j++) {//Stolpec
			cout << "A[" << i << "][" << j << "]";
			cin >> A[i][j];
		}
	}
	
	cout << "Vnesite elemente vektorja b\n";
	for (i = 0; i < N; i++) {//Vrstica
		cout << "b[" << i << "]";
		cin >> A[i][N];
	}
}

void izpisi_matriko_na_terminal(vector<vector<double>> A) {
	int i, j;
	int n = A.size(); //-> število vrstic
	//Ispizi A
	for (i = 0; i < n; i++) {
		for (j = 0; j < n+1; j++) {
			cout << A[i][j] << "\t";
			if (j == n-1) {
				cout <<"\t|\t";
			}
		}
		cout << "\n";
	}
}

vector<double> Gauss_eliminacija(vector<vector<double>> A) {
	int stolpec, vrstica, k;
	int n = A.size();
	double fac;

	//LU dekompozicija;

	for (stolpec = 0; stolpec < n; stolpec++) {

		int max_row = stolpec;
		for (vrstica = stolpec + 1; vrstica < n; vrstica++) {
			if (abs(A[vrstica][stolpec]) > abs(A[max_row][stolpec])) {
				max_row = vrstica;
			}
		}

		if (max_row != stolpec) {
			swap(A[max_row], A[stolpec]);
		}


		for (vrstica = stolpec + 1; vrstica < n; vrstica++) {
			fac = A[vrstica][stolpec] / A[stolpec][stolpec];
			cout << "Iteracija: fac = " << fac << " " << stolpec << vrstica << "\n";
			cout << "stevec = " << A[stolpec][vrstica] << "\n";
			cout << "imenovalec = " << A[stolpec][stolpec] << "\n";
			for (k = 0; k < n+1; k++) {
				A[vrstica][k] = A[vrstica][k] - A[stolpec][k] * fac;
			}

			
			izpisi_matriko_na_terminal(A);
		}
	} //Od te točke dalje je matrika A postala matrika U

	//Rešimo sistem enačb
	vector<double> x(n);
	double b;
	for(vrstica=n-1; vrstica>=0; vrstica--){
		b = A[vrstica][n];
		x[vrstica] =  b/A[vrstica][vrstica];
		for (k = vrstica - 1; k >= 0; k--) {
			A[k][n] -= A[k][vrstica] * x[vrstica];
		}
	}
	return x;
}

void izpis_resitev(vector <double> R) {
	int n = R.size();
	int i;
	for (i = 0; i < n; i++) {
		cout << "x(" << i + 1 << ") = " << R[i] << "\n";
	}
}


//Shrani matriko A v datoteko z imanom filename
void shrani_matriko_v_datoteko(const vector<vector<double>>& A) {
	string filename;
	cout << "Vnesite ima datoteke, v katero bo matrika shranjena: ";
	cin >> filename;

	ofstream file(filename);

	if (file.is_open()) {
		int n = A.size();
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n + 1; j++) {
				file << A[i][j] << (j == n ? '\n' : ' ');
			}
		}
		file.close();
	}
	else {
		cerr << "Unable to open file " << filename << endl;
	}
}

//Preberi matriko, ki je shranjena v datoteki z imenom filename
vector<vector<double>> preberi_matriko_iz_datoteke(const string& filename) {
	vector<vector<double>> A;
	ifstream file(filename);

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			vector<double> row;
			istringstream ss(line);
			double value;

			while (ss >> value) {
				row.push_back(value);
			}

			A.push_back(row);
		}
		file.close();
	}
	else {
		cerr << "Ne morem odpreti datoteke " << filename << endl;
	}
	return A;
}