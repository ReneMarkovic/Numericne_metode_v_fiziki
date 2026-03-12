#pragma once

/*
 * Numerično integriranje za izračun določenega integrala ∫_a^b f(x) dx
 *
 * Vse funkcije sprejemajo:
 *   f - kazalec na funkcijo f(x)
 *   a - spodnja meja integrala
 *   b - zgornja meja integrala
 *   n - število delitev intervala [a, b]
 */

// Pravilo pravokotnikov (Left Riemann Sum): napaka O(h)
// F ≈ h * Σ_{i=0}^{n-1} f(a + i*h)
double rectangle_rule(double (*f)(double), double a, double b, int n);

// Trapezno pravilo (Trapezoid Rule): napaka O(h^2)
// F ≈ h * [f(a)/2 + Σ_{i=1}^{n-1} f(x_i) + f(b)/2]
double trapezoid_rule(double (*f)(double), double a, double b, int n);

// Simpsonovo pravilo (Simpson's 1/3 Rule): napaka O(h^4)
// F ≈ (h/3) * [f(x_0) + 4f(x_1) + 2f(x_2) + 4f(x_3) + ... + 4f(x_{n-1}) + f(x_n)]
// Zahteva sodo število delitev n. Če n ni sod, se samodejno poveča za 1.
double simpson_rule(double (*f)(double), double a, double b, int n);
