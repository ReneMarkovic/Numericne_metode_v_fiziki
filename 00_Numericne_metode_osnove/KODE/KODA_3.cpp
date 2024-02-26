// KODA_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Uporabili bomo logiène ukaze.
*/

int a = 0, b = 10;
int da = 1, db = 5;
/*
Povecujte stevilo a dokler ne postane vecje od stevila b.
Potem pricni povecevati stevilo b, dokler ni 5 krat vecje 
od stevila a. 

Zacetna vrednost stevila a = 0. Zacetna vrednost stevila b = 10.
Stevilo a povecuje po karatkih 1. Stevilo b povecuj po koraku 5.

Simulacijo izvajaj dokler stevilo b ni vecje od 200.
*/

int povecuj(int a, int b, int da, int db) {
	while (b < 200) {
		//Povecuj a:
		if (a < b || a == b) {
			a += da;
		}
		else {
			b += db;
		}
	}
	printf("a = %d\tb = %d\n",a,b);
	return 0;
}

int main(){
	printf("Pricetek\n");
	
	povecuj(a, b, da, db);

	printf("Konec\n");
	return 0;
}

