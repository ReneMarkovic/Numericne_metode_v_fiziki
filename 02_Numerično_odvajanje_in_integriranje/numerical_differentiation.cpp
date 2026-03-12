#include "numerical_differentiation.h"

double forward_difference(double (*f)(double), double x, double h) {
    return (f(x + h) - f(x)) / h;
}

double central_difference(double (*f)(double), double x, double h) {
    return (f(x + h) - f(x - h)) / (2.0 * h);
}

double central_difference4(double (*f)(double), double x, double h) {
    // 5-točkovni stencil za O(h^4) natančnost prvega odvoda
    return (-f(x + 2.0*h) + 8.0*f(x + h) - 8.0*f(x - h) + f(x - 2.0*h)) / (12.0 * h);
}

double second_derivative(double (*f)(double), double x, double h) {
    return (f(x + h) - 2.0*f(x) + f(x - h)) / (h * h);
}

double abs_error(double approx, double exact) {
    return fabs(approx - exact);
}

double geometric_factor(double h_start, double h_stop, int M) {
    return pow(h_stop / h_start, 1.0 / (double)(M - 1));
}
