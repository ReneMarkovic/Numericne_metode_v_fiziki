#pragma once
void primer_1(void) {
	int i = 101.9;// Definiramo spremenljivko i, ki je tipa int (celo število)
	double j = 101.3; //Definiramo spremenljivko j, ki je tipa double (realno število)
	char n[] = "Fizika"; //DEfiniramo spremenljivko n, ki je polje simbolov
	bool s = true; //Definiramo spremenljivko s, ki je tipa bool (Da/Ne binarno)
	/*
	Vrednost s je lahko 1 || true
	Vrednost s je lahko 0 || false
	*/
	printf("Spremenljivka i = %d\n", i); // \n -> nova vrstica
	printf("Spremenljivka j = %.2f\n", j);
	printf("Spremenljivka n = %s\n", n);
	printf("Spremenljivka s = %d\n", s);
}

using namespace std;

void primer_2(int stevilo) {
	printf("Èe imamo %d spremenljivk doloèenega tipa, te zasedejo:\n", stevilo);
	printf("Velikost spremenljivke char = %d B\n", sizeof(char) * stevilo);
	printf("Velikost spremenljivke double = %d B\n", sizeof(double) * stevilo);
	printf("Velikost spremenljivke int = %d B\n", sizeof(int) * stevilo);
	printf("Velikost spremenljivke bool = %d B\n", sizeof(bool) * stevilo);
}