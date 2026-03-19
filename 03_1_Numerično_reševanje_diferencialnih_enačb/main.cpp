#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Parametri prostega pada (ostanejo enaki)
#define rho_z 10.0
#define rho_atm 1.29
#define g 10.0
#define r 0.05
#define PI (4.0 * atan(1.0))
#define S (4.0 * PI * r * r)
#define V (4.0 / 3.0 * PI * r * r * r)
#define m_z (rho_z * V)
#define c 1.0
#define k (0.5 * rho_atm * c * S / m_z)

#include "solve_ode.h"

int main() {
    FILE* pisi;
    
    // --- 1. DEL: Splošna enačba y' = f(x, y) ---
    double x = 0.0;
    double y_e = 1.0;   // Začetni pogoj y(0) = 1
    double y_rk4 = 1.0;
    double h = 0.1;

    printf("Resevanje splosne enacbe y' = f(x, y):\n");
    while (x <= 1.0) {
        printf("x = %.1f\t y_Euler = %.5f\t y_RK4 = %.5f\n", x, y_e, y_rk4);
        
        // Pomembno: ne uporabljaj +=, ker funkcije ze vrnejo y + dy
        y_e = ode_Euler_solver(x, y_e, h);
        y_rk4 = ode_RK4_solver(x, y_rk4, h);
        x += h;
    }

    printf("\n-----------------------------------\n\n");

    // --- 2. DEL: Prosti pad ---
    double v_rk4 = 0.0;
    double v_e = 0.0;
    double dt = 0.1;
    double t = 0.0;
    double vt = sqrt((2 * g * m_z) / (rho_atm * c * S)); // Terminalna hitrost

    pisi = fopen("rezultati_vaje_04_pt2.txt", "w+");
    if (pisi == NULL) return 1;

    fprintf(pisi, "t\tv_rk4\tv_e\tvt\n");

    while (t <= 120.0) {
        // Ponovno: funkcije vrnejo celotno novo hitrost
        v_e = newton_euler_dynamics(v_e, dt);
        v_rk4 = newton_RK4_dynamics(v_rk4, dt);

        if ((int)(t * 10) % 100 == 0) { // Izpis vsakih 10 sekund v konzolo
            printf("t = %.1f\t v_RK4 = %.2f\t v_Euler = %.2f\t vt = %.2f\n", t, v_rk4, v_e, vt);
        }

        fprintf(pisi, "%f\t%f\t%f\t%f\n", t, v_rk4, v_e, vt);
        t += dt;
    }

    fclose(pisi);
    printf("\nRezultati prostega pada so shranjeni v 'rezultati_vaje_04_pt2.txt'.\n");

    return 0;
}