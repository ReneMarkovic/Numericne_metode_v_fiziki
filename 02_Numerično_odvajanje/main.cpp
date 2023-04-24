// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Header.h"

#define start 1E-6
#define stop 1
#define steps 20 

#define PI 4*atan(1)

#define x_start 0
#define x_stop 2.0*PI
#define N 100

double a = 0.5;// V tej točki računamo vrednost odvoda.
double dh = faktor(start, stop, steps);

FILE* data;

int main(){
	printf("Pi = %f\n", PI);
	//Naloga 1
	fopen_s(&data, "naloga_1.txt", "w+");
	fclose(data);

	double analiticna_vrednost_odvoda = dfdx(a);
	double h = start;
	while (h <= stop) {
		fopen_s(&data, "naloga_1.txt", "a");
		
		fprintf(data, "%f\t", h);
		fprintf(data, "%f\t", analiticna_vrednost_odvoda);
		fprintf(data, "%f\t%f\t", df_en1(a, h), df_en2(a, h));
		fprintf(data, "%f\t", fabs(df_en1(a, h),analiticna_vrednost_odvoda));
		fprintf(data, "%f\n", fabs(df_en2(a, h), analiticna_vrednost_odvoda));
		
		fclose(data);
		h *= dh;
	}


	//Naloga 2
	double x;
	double dx = (x_stop - x_start) / (double)(N);
	h = 0.01;

	fopen_s(&data, "naloga_2.txt", "w+");
	fclose(data);

	for (x = x_start; x <= x_stop; x += dx) {
		fopen_s(&data, "naloga_2.txt", "a");

		analiticna_vrednost_odvoda = dfdx(x);

		fprintf(data, "%f\t", x);
		fprintf(data, "%f\t", analiticna_vrednost_odvoda);
		fprintf(data, "%f\t%f\t", df_en1(x, h), df_en2(x, h));
		fprintf(data, "%f\t", fabs(df_en1(x, h), analiticna_vrednost_odvoda));
		fprintf(data, "%f\n", fabs(df_en2(x, h), analiticna_vrednost_odvoda));

		fclose(data);
	}
	return 0;
}