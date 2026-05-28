// 09_MC.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define SEME 42
#define n_bins 71

#define RHO 0.3
#define Nx 40
#define Ny 40
#define N Nx*Ny
#define a sqrt(1.0/RHO)
#define L Nx*a
#define D 2
#define STOP 100*N 
#define EIN 0.7*STOP

double T = 10.0;

double lega[N][D];
double Wv[N];


#include "MC.h"


/*

Kaj še moramo narediti

	(1) Ročno nastaljanje temeprature.
	(2) Kontroni zapis na koncu simulacije v terminalu.
	(3) Regulacija pristranskosti (bias ~ 0.5).
	(4) Zapis <E>/N v odvisnosti od iteracije, Acceptance ratio in delta.
	(5) Zapisosovanje leg posameznih delcev v teku simulaije.
	(6) Radialno porazdelitvena funkcija (g(r)) in sproten zapis vrednosti. 
	(7) Vizualizacija
		- Distribucja energija
		- Začetna in končna lega
		- g(r)
		- 3D ?


	


*/



int main(int argc, char* argv[]){ //Dodana sta argc in argv
	//Prebiranje vnosa
	if(argc>1){
		sscanf(argv[1], "%lf", &T);
	}else{
		T = 0.001;
	}

	srand(SEME);
	double E_old = 0.0;
	double MC_bias = 0.0; //Dodana je spremenljivka MC_bias

	inicializacija_leg(lega);
	E_old = energija_sistema(lega, Wv);
	printf("E_old = %f\n",E_old);
	MC_simulator(lega, &E_old,  Wv, &MC_bias); // Dana sta kazalnika za &E_old in &MC_bias


	//Posodobljen izpis
	printf("Povzetek simulacije: \n");
	printf(" - N = %d\n",N);
	printf(" - Rho = %.3f\n",RHO);
	printf(" - T = %.3f\n",T);
	printf(" - <E>/N = %.3f\n", E_old/(double)(N));
	printf(" - Acceptance ratio = %.3f %% \n",MC_bias*100);


	getchar();
	return 0;
}
