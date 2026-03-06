// main.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define gloabl_eps 0.000001

#include "funkcije.h"


double a, b;

int main(){
	double x = 0.0;
	int i = 4;

	a = -10.0;
	b = 10.0;

	int OK;
	int metoda = 2;
	int N = 100000;
	double eps = 0.0001;

	FILE* out = fopen("rezultati.csv", "w+");
	if (out) fprintf(out, "FUNKCIJA,METODA,PARAMETER,VREDNOST\n");

	for(int j = 0; j < 3; j++){
		for (i = 1; i < 5; i++) {
			OK = preveri_init(a, b, i, j, N, eps, out);
			printf("%d\n", OK);
			printf("Vrednost funkcije f(%.2f) = %.2f\n", x, f(x, i));
		}
	}
	if (out) fclose(out);
    return 0;
}

