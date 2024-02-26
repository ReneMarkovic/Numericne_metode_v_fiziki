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

void primer_3(void){
	int i;
	for (i = 0; i < 100; i = i + 1) {
		printf("i = %d\n", i);
	}

	while (i <= 100) {
		printf("i (while) = %d\n", i);
		i = i + 1;
	}
}

int primer_4(int a) {
	if (a == 0 || a == 1) {
		printf("Število %d po definiciji ni praštevilo.\n", a);
		return 0;
	}
	if (a == 2 || a == 3) {
		printf("Stevilo %d je praštevilo", a);
		return 1;
	}
	if (a % 2 == 0) {
		printf("Število %d po definiciji ni praštevilo.\n", a);
		return 0;
	}
	if (a % 3 == 0) {
		printf("Število %d po definiciji ni praštevilo.\n", a);
		return 0;
	}
	int i ;
	int check = 0;
	for (i = 4; i < a; i++) {
		if (a % i == 0) {
			printf("Število %d po definiciji ni praštevilo.\n", a);
			return 0;
		}
	}
	printf("Stevilo %d je praštevilo", a);
	return 1;
}

void primer_5(double R) {
	double S, V;
	S = 4 * PI * R * R;
	V = S * R / 3.0;
	printf("\n Krogla, ima polmer %f m.\n", R);
	printf("S = %f m^2\n", S);
	printf("V = %f m^3\n", V);
}