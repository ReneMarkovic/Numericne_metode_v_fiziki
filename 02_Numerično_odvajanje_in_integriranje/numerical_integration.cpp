#include "numerical_integration.h"

double rectangle_rule(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / (double)n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += f(a + i * h);
    }
    return sum * h;
}

double trapezoid_rule(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / (double)n;
    double sum = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }
    return sum * h;
}

double simpson_rule(double (*f)(double), double a, double b, int n) {
    if (n % 2 != 0) n++;  // n mora biti sodo
    double h = (b - a) / (double)n;
    double sum = f(a) + f(b);
    for (int i = 1; i < n; i++) {
        sum += (i % 2 == 0) ? 2.0 * f(a + i * h) : 4.0 * f(a + i * h);
    }
    return sum * h / 3.0;
}
