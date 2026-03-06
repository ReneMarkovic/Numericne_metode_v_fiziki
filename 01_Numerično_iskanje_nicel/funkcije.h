#include <stdio.h>

const char* ime_funkcije(int i) {
    if (i == 1) return "A";
    if (i == 2) return "B";
    if (i == 3) return "C";
    if (i == 4) return "D";
    return "?";
}

//Definiramo funkcijo
double f(double x, int izberi) {
	double y;
	if (izberi == 1) {
		y = cos(x) - x*exp(x);
	}
	if (izberi == 2) {
		y = pow(x, 4) - x - 10;
	}
	if (izberi == 3) {
		y = x - exp(-x);
	}
	if (izberi == 4) {
		y = x - sin(x) - 0.5;
	}
	if (izberi == 5) {
		y = x;
	}
	return y;
}

double df(double x, int izberi) {
	double y;
	if (izberi == 1) {
		y = -sin(x) - exp(x)*(x + 1);
	}
	if (izberi == 2) {
		y = 4*pow(x, 3) - 1;
	}
	if (izberi == 3) {
		y = 1 + exp(-x);
	}
	if (izberi == 4) {
		y = 1 - cos(x);
	}
	if (izberi == 5) {
		y = 1.0;
	}
	return y;
}

double fabs(double x) {
	return sqrt(x*x);
}

bool preveri_ab_bisekcija(double a, double b, int i){
	double fa, fb;
	fa = f(a, i);
	fb = f(b, i);
	if (fa*fb > 0) return false;
	else return true;
}

double bisekcija(double a, double b, int i, double eps, FILE* out) {
	printf("#### ZACETEK BISEKCIJE ####\n");

	double xa, xb, xc;
	double fa, fb, fc;
	int iters = 0;

	bool check = preveri_ab_bisekcija(a, b, i);
	if (!check){
		printf("Izbran interval [%f,%f] ne ustreza pogoju f(a)*f(b) < 0\n", a, b);
		printf("Iskanje nićel ni bilo uspešno.\n");
		return 0.0;
	};

	xa = a;
	xb = b;
	xc = (xa + xb) / 2.0;

	fa = f(xa, i);
	fb = f(xb, i);
	fc = f(xc, i);

	while (fabs(fc) > eps) {
		if (fa*fc > 0) {
			xa = xc;
			fa = f(xa, i);
		}
		if (fb*fc > 0) {
			xb = xc;
			fb = f(xb, i);
		}

		xc = (xa + xb) / 2.0;
		fc = f(xc, i);
		iters++;
	}
	printf("#### KONEC BISEKCIJE ####\n");
	printf("Rešitev |f(x)|<=eps je pri x = %f\n", xc);

	if (out) {
		fprintf(out, "%s,BISEKCIJA,EPS,%.6f\n", ime_funkcije(i), eps);
		fprintf(out, "%s,BISEKCIJA,STEVILO ITERACIJ,%d\n", ime_funkcije(i), iters);
		fprintf(out, "%s,BISEKCIJA,RESITEV,%.6f\n", ime_funkcije(i), xc);
	}
	return xc;
}

double sekantna_metoda(double a, double b, int i, double eps, FILE* out) {
	double x0 = a;
	double x1 = b;
	int iters = 0;
	double k = (f(x1, i) - f(x0, i)) / (x1 - x0);
	double x2 = x1 - f(x1, i) / k;
	double f2 = f(x2, i);
	while (fabs(f2) > eps) {
		printf("%.2f %.2f %.2f\n", x0, x1, x2);
		x0 = x1;
		x1 = x2;
		k = (f(x1, i) - f(x0, i)) / (x1 - x0);
		x2 = x1 - f(x1, i) / k;
		f2 = f(x2, i);
		iters++;
	}
	if (out) {
		fprintf(out, "%s,SEKANTNA,EPS,%.6f\n", ime_funkcije(i), eps);
		fprintf(out, "%s,SEKANTNA,STEVILO ITERACIJ,%d\n", ime_funkcije(i), iters);
		fprintf(out, "%s,SEKANTNA,RESITEV,%.6f\n", ime_funkcije(i), x2);
	}
	return x2;
}

double Newton(double a, int i, double eps, FILE* out) {
	double x0 = a;
	int iters = 0;
	double k = f(x0, i)/df(x0, i);
	double x1 = x0 - k;

	double f1 = f(x1, i);

	while (fabs(f1) > eps) {
		x0 = x1;
		k = f(x0, i) / df(x0, i);
		x1 = x0 - k;
		f1 = f(x1, i);
		iters++;
	}
	if (out) {
		fprintf(out, "%s,NEWTON,EPS,%.6f\n", ime_funkcije(i), eps);
		fprintf(out, "%s,NEWTON,STEVILO ITERACIJ,%d\n", ime_funkcije(i), iters);
		fprintf(out, "%s,NEWTON,RESITEV,%.6f\n", ime_funkcije(i), x1);
	}
	return x1;
}

//Preveri ali sta začetna pogoja vredu
int preveri_init(double a, double b, int i, int metoda, int N, double eps, FILE* out) {
	double fa, fb;
	double dx = (b-a)/N+0.00001;
	int stej=0;
	double resitev;
	for (double x = a; x < b + dx; x += dx) {
		fa = f(x, i);
		fb = f(x+dx, i);
		if (fa*fb < 0) {
			if (metoda == 0) resitev = bisekcija(x, x+dx, i, eps, out);
			if (metoda == 1) resitev = sekantna_metoda(x, x+dx, i, eps, out);
			if (metoda == 2) resitev = Newton(x, i, eps, out);
			printf("Najdena je ni\xc4\x8dla pri x = %f\n", resitev);
			stej += 1;
		}
	}
	return stej;
}