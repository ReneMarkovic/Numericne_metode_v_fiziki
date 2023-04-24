// DFT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <time.h> //Za merjenje trajanja dela kode, da se izvede

#define T_START 0.0 //sec
//#define T_END 101.0 //sec
#define pp 10.0//15.0
#define N (int)(pow(2,pp))
#define FS 10.0 //meritev na sekundo
//#define N (int)((T_END-T_START)*FS) //število vzorcev oz. število zapisov v časovni vrsti
#define T_END (double)N/FS + TS
#define DT 1/FS
#define T_END 101.0 //sec

/* N = TE*FS - TS*FS */
/* TE = N/FS + TS*/

#define PI 4*atan(1)

#include "Freq_ana.h"

double x0 = 1.0;
double f1 = 1 / 10.0, f2 = 1 / 4.0;
double o1 = 2.0 * PI * f1;
double o2 = 2.0 * PI * f2;

clock_t start, end;
double cpu_time_dft;
double cpu_time_fft;

int main(){
	get_info(true);
	double* y = (double*)malloc(N * sizeof(double));
	f(y,o1,o2,x0);
	zapisi_casovno_vrsto(y);

	start = clock();
	double** matrix;
	matrix = DFT(y);
	free(matrix);
	end = clock();
	cpu_time_dft = ((double)(end - start)) / CLOCKS_PER_SEC;
	//printf("DFT je za analizo %d podatkov potreboval %f s\n", N, cpu_time);


	start = clock();
	Complex* izhod = (Complex*)malloc(N * sizeof(Complex));
	FFT(y, izhod, N);
	end = clock();
	cpu_time_fft = ((double)(end - start)) / CLOCKS_PER_SEC;
	//printf("DFT je za analizo %d podatkov potreboval %f s\n", N, cpu_time);

	printf("%d\t%d\t%f\t%f\n", (int)(pp), (int)(N), cpu_time_dft, cpu_time_fft);

	FILE* file = NULL;
	fopen_s(&file, "FFT.txt", "w+");
	for (int i = 0; i < N; i++) {
		double freq = (double)i / (N * DT); // Calculate the frequency
		fprintf(file, "%f\t%f\t%f\ti%f\n", freq, complex_magnitude(izhod[i]), izhod[i].real, izhod[i].imag);
	}
	fclose(file);
	free(y);
	free(izhod);

	/*
	Complex a;
	a.real = 2.0;
	a.imag = 2.0;

	Complex b;
	b.real = 5.0;
	b.imag = 1.0;

	Complex c = complex_add(a, b);

	printf("a = %.1f i %.1f\t%f\t%f\n", a.real, a.imag, complex_magnitude(a), complex_phase(a));
	printf("b = %.1f i %.1f\t%f\t%f\n", b.real, b.imag, complex_magnitude(b), complex_phase(b));
	printf("b = %.1f i %.1f\n", c.real, c.imag);
	*/
	return 0;
}
