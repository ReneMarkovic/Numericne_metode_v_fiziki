#pragma once
#include <math.h>
#include <stdio.h>

// --- DEL 1: Splošni reševalnik za y' = f(x, y) ---

// Definiramo funkcijo f(x, y), ki je podana v nalogi
double f(double x, double y) {
    // Primer funkcije iz tvoje kode: (5x^2 - y) / e^(x+y)
    return (5.0 * x * x - y) / (exp(x + y));
}

// En korak Eulerjeve metode: y_{n+1} = y_n + h * f(x_n, y_n)
double ode_Euler_solver(double x, double y, double h) {
    return y + f(x, y) * h;
}

// En korak RK4 metode za splošno enačbo
double ode_RK4_solver(double x, double y, double h) {
    double k1, k2, k3, k4;
    
    k1 = h * f(x, y);
    k2 = h * f(x + h / 2.0, y + k1 / 2.0);
    k3 = h * f(x + h / 2.0, y + k2 / 2.0);
    k4 = h * f(x + h, y + k3);
    
    return y + (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
}

// --- DEL 2: Dinamika prostega pada (Newton) ---

// Pospešek a(v) upošteva težnost in zračni upor
// a = g - (upor/masa) -> v tvojem primeru je smer definirana v main.cpp
double a(double v) {
    // Konstanta k vključuje rho, S, c in m_z (definirano v main.cpp)
    // Pazimo na predznak: če telo pada, g vleče dol (+), upor zavira (-)
    return g - k * v * v;
}

// Eulerjev korak za hitrost: v_{n+1} = v_n + dt * a(v_n)
double newton_euler_dynamics(double v, double dt) {
    return v + a(v) * dt;
}

// RK4 korak za hitrost
double newton_RK4_dynamics(double v, double dt) {
    double k1, k2, k3, k4;
    
    k1 = dt * a(v);
    k2 = dt * a(v + k1 / 2.0);
    k3 = dt * a(v + k2 / 2.0);
    k4 = dt * a(v + k3);
    
    return v + (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
}