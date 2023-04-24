#include <math.h>

double f(double x) {
	return sin(x);
}
double dfdx(double x) {
	return cos(x);
}
//Numerične metode za dolo�evanje odvoda
double df_en1(double x, double h) {
	return (f(x + h) - f(x)) / h;
}
double df_en2(double x, double h) {
	double st = f(x + h) - f(x - h);
	double im = 2 * h;
	return st / im;
}
double df_en3(double x, double h) {
	double st = f(x + h) + f(x - h) - 2 * f(x);
	double im =  4*h;
	return st / im;
}

double fabs(double v1, double v2) {
	double delta = v1 - v2;
	return sqrt(delta * delta);
}

double faktor(double start, double stop, int steps) {
	double eksponent = 1.0 / (double)(steps - 1);
	return pow(stop / start, eksponent);
}
