#pragma once
//random_generator.h
int rand_int_ab(int a, int b) {
	/*
	Ta funkcija generira naključno naravno število
	znotraj intervala a in b (vključno z a in b)
	*/
	int r = rand() % (b - a + 1) + a;
	return r;
}

double rand_double() {
	/*
	Ta funkcija generira realno naključno število r,
	za katero velja 0 <= r < 1.
	*/
	double r = (double)(rand()) / (double)(RAND_MAX);
	return r;
}

double rand_double_ab(double a, double b) {
	/*
	Ta funkcija generira realno naključno število r,
	za katero velja a <= r <= b.
	*/
	//RAND_MAX -> Koliko različnih števil lahko generirate (0,1, ... ,32767-1)

	double r = (double)(rand()) / (double)(RAND_MAX-1);
	//a = -1
	//b = 1
	r = a + (b - a) * r;
	return r;
}

void vaje_01() {
	//printf("Max rand = %d\n", RAND_MAX);
	int i;

	FILE* pisi;
	pisi = fopen("vaja_01.txt", "w+");
	//pisi=fopen("vaja_01.txt", "w+");
	int r1;
	double r2;
	double r3;
	for (i = 0; i < sample_size; i++) {
		r1 = rand_int_ab(1, 6);
		r2 = rand_double();
		r3 = rand_double_ab(-10, 10);
		fprintf(pisi, "%d\t%f\t%f\n", r1, r2, r3);
	}
	fclose(pisi);
}

/*Vaja 2*/

bool hit_or_miss(double x, double y,double polmer) {
	double d = sqrt(x * x + y * y);//Razdalja od izhodišča koordinatnega sistema do točke(x,y)
	if (d < polmer) return true;
	else return false;
}

void vaje_02(int N) {
	/*
	Uporabite generator naključnih števil in določite vrednost števila pi.
	Ustvarite graf, ki bo prikazoval, kako se natančnost izračunane vrednosti za 
	pi spreminja v odvisnosti od števila neneriranih naključnih števil.

	uporabiom rand_double();
	število pi je PI
	*/
	FILE* pisi;
	pisi = fopen("Vaja_02.txt", "w+"); //visual studio verzij
	//pisi = fopen("Vaja_02.txt", "w+"); //g++ verzija

	int st_metov, i;
	int Nh;
	double x, y;

	double ocena_stevila_pi;
	for (st_metov = 1; st_metov < N; st_metov++) {
		Nh = 0;
		for (i = 0; i < st_metov; i++) {
			/*Tukaj bom vrgel st_metov krat točk*/
			x = rand_double();
			y = rand_double();
			Nh += hit_or_miss(x, y, 1);
		}
		ocena_stevila_pi = 4.0 * (double)(Nh) / (double)(st_metov);
		fprintf(pisi,"%d\t%f\t%f\n", st_metov, ocena_stevila_pi,PI);
	}
	fclose(pisi);
}

/*Vaja 3*/

vector<double> vaja_03(int N){
	/*V tej vaji boste s pomočjo simulacije obravnavali znani problem Monty Hall.
	V igri nastopajo tri vrata, za katerimi se skriva ena nagrada (npr. avtomobil),
	za ostalima dvema pa ne (npr. koza). Igralec najprej naključno izbere ena vrata.
	Voditelj nato odpre ena izmed preostalih dveh vrat, za katerimi zagotovo ni nagrade.
	Igralec ima nato možnost, da vztraja pri prvotni izbiri ali pa zamenja vrata.
	Vaša naloga je, da s pomočjo generatorja naključnih števil izvedete simulacijo
	igre Monty Hall in statistično preverite uspešnost obeh strategij:
	- strategije, pri kateri igralec ne zamenja začetne izbire,
	- strategije, pri kateri igralec vedno zamenja izbiro po tem, ko voditelj odpre ena vrata.
	Simulacijo izvedite za večje število ponovitev in beležite število zmag v obeh primerih. Na podlagi zbranih rezultatov izračunajte relativno verjetnost zmage za posamezno strategijo ter rezultate grafično in numerično analizirajte.
	*/
	int i;
	int zmage_ostani = 0;   // strategija: ne zamenjaj vrat
	int zmage_zamenjaj = 0; // strategija: zamenjaj vrata

	FILE* pisi;
	pisi = fopen("Vaja_03.txt", "w+");

	for (i = 0; i < N; i++) {
		int nagrada = rand_int_ab(0, 2); // Nagradno vrata (0, 1 ali 2)
		int izbira = rand_int_ab(0, 2); // Igralec izbere vrata

		// Voditelj odpre ena vrata (ne izbira, ne nagrada)
		int voditelj;
		do {
			voditelj = rand_int_ab(0, 2);
		} while (voditelj == izbira || voditelj == nagrada);

		// Strategija 1: ostani pri prvotni izbiri
		if (izbira == nagrada) zmage_ostani++;

		// Strategija 2: zamenjaj na preostala vrata
		int zamenjava;
		do {
			zamenjava = rand_int_ab(0, 2);
		} while (zamenjava == izbira || zamenjava == voditelj);

		if (zamenjava == nagrada) zmage_zamenjaj++;

		// Zapisi v datoteko: st_iger, p_ostani, p_zamenjaj
		double p_ostani = (double)zmage_ostani / (double)i;
		double p_zamenjaj = (double)zmage_zamenjaj / (double)i;
		fprintf(pisi, "%d\t%f\t%f\n", i, p_ostani, p_zamenjaj);
	}
	fclose(pisi);

	double p_ostani_final = (double)zmage_ostani / (double)N;
	double p_zamenjaj_final = (double)zmage_zamenjaj / (double)N;

	printf("Monty Hall simulacija (%d iger):\n", N);
	printf("  Strategija OSTANI:   P = %.4f (teorija: 1/3)\n", p_ostani_final);
	printf("  Strategija ZAMENJAJ: P = %.4f (teorija: 2/3)\n", p_zamenjaj_final);

	vector<double> rezultat = {p_ostani_final, p_zamenjaj_final};
	return rezultat;
}

/*Vaja 4*/

vector<int> loto_generator() {
	vector<int> stevila(7); //Listek od uporabnika
	vector<int> zrebanje(7);
	vector<int> moznosti(39);
	int i;
	for (i = 0; i < (int)(moznosti.size()); i++) {
		moznosti[i] = (int)(i+1);
	}
	//Generiraj
	int s,k,l;
	for (i = 0; i < 7; i++) {
		s = rand_int_ab(0, 38-i);
		k = moznosti[s];
		stevila[i] = k;

		l = moznosti[38 - i];
		moznosti[s] = l;
		moznosti[38 - i] = k;
		
	}
	//izpisi(stevila);
	sortiraj(stevila);
	return stevila;
}

vector<int> sortiraj(vector<int>& x) {
	int n = x.size();
	int i, j, i_min, l;
	int min_value = 100000;

	for (i = 0; i < n; i++) {
		min_value = 100000;

		for (j = i; j < n; j++) {
			if (x[j] < min_value) {
				min_value = x[j];
				i_min = j;
			}
		}

		l = x[i];
		x[i] = min_value;
		x[i_min] = l;
	}
	return x;
}

void izpisi_listek(vector<int> x) {
	int n = x.size(),i;
	printf("Kominacija: ");
	for (i = 0; i < n; i++) {
		printf("%d ", x[i]);
	}
	printf("\n");
}

void izpisi_rezultat(vector<int> x) {
	int n = x.size();
	int i;
	for (i = 0; i < n; i++) {
		printf("%d enakih stevil se pojavi %d-krat.\n", i, x[i]);
	}
}

void igra_loto() {
	//vector<int> stevila(7); //Listek od uporabnika
	vector<int> zrebanje;
	vector<int> stevila;

	stevila=loto_generator();

	izpisi_listek(stevila);
	int igra, nc,i,j;

	vector<int> rezultati(8);
	//[0,1,2,3,4,5,6,7]

	int max_krogov = 2 * 51 * 100;
	printf("Simulacija se izvede za %d krogov igre\n\n", max_krogov);
	for (igra = 0; igra < max_krogov; igra++) {
		zrebanje = loto_generator();
		//Koliko stevil se ujema
		nc = 0;
		for (i = 0; i < stevila.size(); i++) {
			for (j = 0; j < stevila.size(); j++) {
				if (zrebanje[j] == stevila[i]) {
					nc += 1;
				}
			}
		}
		rezultati[nc] += 1;
	}
	izpisi_rezultat(rezultati);
}