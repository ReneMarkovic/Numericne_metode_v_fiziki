// KODA_1.cpp : Defines the entry point for the console application.
//


//To je knji�njica, ki so visual studio sam uvozi.
//�e bi hoteli program zagnati na kak�nem drugem operacijskem sistemu,
//ta ne bi deloval, ker je ta knji�ica prisotna samo znotraj Visual
//Studio projekta.

/*
Kometar v ve�ih vrsticah
*/
#include "stdafx.h"

//1. Uvozimo knji�ice
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//2. Globalne spremenljivke
// Vsaka spremenljivka mora imeti dolo�en tip oz. vrsto.
double x0=1.0; //Realna �tevila
int x1=1;//Cela �tevila
char x2[] = "Pozdrav";//Sosledje simbolov
bool x3=false; //Binarna spremenljivka, ki ima vrednosti 0 - False in 1 - True


int main(){
	printf("x0 = %3.2f\n",x0);//printf('oblikujemo izpis',spremenljivke)
	x0 = x0 + 2 * 10;
	printf("x0 = %3.2f\n", x0);
	x0 += 2 * 10;
	printf("x0 = %3.2f\n", x0);
	x0 = sqrt(x0);
	printf("x0 = %3.2f\n", x0);

	printf("x1 = %d\n", x1);
	printf("x1 = %04d\n", x1);

	printf("x2 = %s\n", x2);

    return 0;
}

