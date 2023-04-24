#pragma once
double f(double x, int izbira) {
	if (izbira == 1) {
		return sin(x);
	}
	if (izbira == 2) {
		return x*x;
	}
	if (izbira == 3) {
		return exp(-x)* sin(x);
	}
}

double pravilo_pravokotnikov(double x_min, double x_max,double dx, int izbira) {
	/*
	x_min -> spodnja meja
	x_max -> zgornja meja
	dx -> širina intervala
	*/
	double N_int = 0.0;
	double x=x_min;
	while (x < x_max) {
		N_int += f(x, izbira) * dx;
		x += dx;
	}
	return N_int;
}

double trapezno_pravilo(double x_min, double x_max, double dx, int izbira){
	/*
	x_min -> spodnja meja
	x_max -> zgornja meja
	dx -> širina intervala
	*/
	double N_int = 0.0;
	double x = x_min;
	N_int += f(x, izbira) * dx/2.0;
	x += dx;
	while (x < x_max) {
		N_int += f(x, izbira) * dx;
		x += dx;
	}
	N_int += f(x, izbira) * dx / 2.0;

	return N_int;
}

double simpsonovo_pravilo(double x_min, double x_max, double dx, int izbira) {
	/*
	x_min -> spodnja meja
	x_max -> zgornja meja
	dx -> širina intervala
	*/
	double N_int = 0.0;
	double A;
	double x = x_min;
	double y0, y1, y2;

	while (x < x_max-2*dx) {
		y0 = f(x, izbira);
		y1 = f(x + dx, izbira);
		y2 = f(x + 2*dx, izbira);
		A= dx/3*(y0+4*y1+y2);
		N_int += A;
		x += 2*dx;
	}
	return N_int;
}