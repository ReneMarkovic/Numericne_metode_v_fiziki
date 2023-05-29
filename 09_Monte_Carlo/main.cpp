// Monte_Carlo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#pragma warning (disable:4996) //fopen


#define EIN 0.7
#define Nx (int)(4)
#define Ny (int)(4)
#define Nz (int)(4)
#define N Nx*Ny*Nz

#define STOP 1000*N

//#define T 0.001 // T okoli 1 je sobna temperatura
#define RHO 0.1

double a = pow(N / RHO,1.0/3.0); //Dolzina stranice kvadrata

double lega[N][3];
double Wv[N];
double T;
#include "MC.h"

double E_old;


int main(int argc, char* argv[]){
	if (argc > 1){
		sscanf(argv[1], "%lf", &T);
	}else{
		T=0.01;
	}
	FILE*pisi_2=fopen("info.dat","w+");
	fprintf(pisi_2,"%d\n",N);
	fprintf(pisi_2,"%f",T);
	fclose(pisi_2);
	
	zacetna_postavitev(lega);
	
	int in=0,ix,iy;
	FILE* pisi = fopen("00_zacetna_konfiguracija.dat", "w+");
	for (ix = 0; ix < Nx; ix++) {
		for (iy = 0; iy < Ny; iy++) {
			fprintf(pisi, "%f\t%f\t%f\n", lega[in][0], lega[in][1], lega[in][2]);
			in++;
		}
	}
	fclose(pisi);

	E_old = energija(lega,Wv);
	MC_simulator(lega, E_old,Wv);

	return 0;
}