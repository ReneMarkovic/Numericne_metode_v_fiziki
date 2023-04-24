#ifndef GAUSS_ELIMINACIJA_H
#define GAUSS_ELIMINACIJA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

//Uporabnik vnese matriko (A) in vektor (b)
void vnesi_matriko_in_vektor(vector<vector<double>>& A);

//Izpisi matriko v terminal
void izpisi_matriko_na_terminal(vector<vector<double>> A);

//Izvedba Gauss eliminacije
vector<double> Gauss_eliminacija(vector<vector<double>> A);

//Izpiši rešitev
void izpis_resitev(vector <double> R);

void shrani_matriko_v_datoteko(const vector<vector<double>>& A);

vector<vector<double>> preberi_matriko_iz_datoteke(const string& filename);

#endif // !GAUSS_ELIMINACIJA_H

