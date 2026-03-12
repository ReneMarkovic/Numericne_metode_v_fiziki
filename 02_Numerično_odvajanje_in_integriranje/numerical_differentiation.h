#pragma once
#include <math.h>

/*
 * Numerično odvajanje z metodo končnih razlik
 *
 * Vse funkcije sprejemajo:
 *   f  - kazalec na funkcijo f(x)
 *   x  - točka, v kateri računamo odvod
 *   h  - korak diskretizacije (h > 0)
 */

// Eno-stranski odvod (Forward Difference): napaka O(h)
// f'(x) ≈ [f(x+h) - f(x)] / h
double forward_difference(double (*f)(double), double x, double h);

// Dvo-stranski odvod (Central Difference): napaka O(h^2)
// f'(x) ≈ [f(x+h) - f(x-h)] / (2h)
double central_difference(double (*f)(double), double x, double h);

// Višjeredna središčna diferenca – 5-točkovni stencil: napaka O(h^4)
// f'(x) ≈ [-f(x+2h) + 8f(x+h) - 8f(x-h) + f(x-2h)] / (12h)
// Opomba: formula v README (f(x+h)+f(x-h)-2f(x))/(4h) ne konvergira k f'(x)
//         in je matematično napačna za odvod – pravilna O(h^4) formula je zgornja.
double central_difference4(double (*f)(double), double x, double h);

// Drugi odvod (Second Derivative): napaka O(h^2)
// f''(x) ≈ [f(x+h) - 2f(x) + f(x-h)] / h^2
double second_derivative(double (*f)(double), double x, double h);

// Absolutna napaka |approx - exact|
double abs_error(double approx, double exact);

// Geometrijski faktor za zaporedje h_{n+1} = faktor * h_n,
// da pokrijemo interval [h_start, h_stop] z M vrednostmi
double geometric_factor(double h_start, double h_stop, int M);
