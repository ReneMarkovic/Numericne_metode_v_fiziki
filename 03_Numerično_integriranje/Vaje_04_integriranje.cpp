// Vaje_04_integriranje.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 4*atan(1)

#include "num_int.h"
FILE* pisi;

int main(){

	fopen_s(&pisi, "numericno_integiranje.txt", "w+");
	double I1, I2, I3;
	double xmin = 0.0;
	double xmax = 2*PI;
	double dx;
	int i;
	for (i = 2; i < 10000; i++) {
		dx = (xmax - xmin) / (double)(i);
		I1 = pravilo_pravokotnikov(xmin, xmax, dx, 1);
		I2 = trapezno_pravilo(xmin, xmax, dx, 1);
		I3 = simpsonovo_pravilo(xmin, xmax, dx, 1);
		fprintf(pisi,"%f\t%f\t%f\t%f\n", dx, I1, I2, I3);
	}
	fclose(pisi);
	return 0;
}