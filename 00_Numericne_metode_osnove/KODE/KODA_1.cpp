// KODA_1.cpp : Defines the entry point for the console application.
//


//To je knjižnjica, ki so visual studio sam uvozi.
//Èe bi hoteli program zagnati na kakšnem drugem operacijskem sistemu,
//ta ne bi deloval, ker je ta knjižica prisotna samo znotraj Visual
//Studio projekta.

/*
Kometar v veèih vrsticah
*/
#include "stdafx.h"

//1. Uvozimo knjižice
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//2. Globalne spremenljivke
// Vsaka spremenljivka mora imeti doloèen tip oz. vrsto.
double x0=1.0; //Realna števila
int x1=1;//Cela števila
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

