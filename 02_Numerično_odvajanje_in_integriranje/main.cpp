#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "numerical_differentiation.h"
#include "numerical_integration.h"

#define PI (4.0 * atan(1.0))

// ============================================================
// TESTNE FUNKCIJE
// ============================================================

double f_sin(double x)    { return sin(x); }
double df_sin(double x)   { return cos(x); }
double d2f_sin(double x)  { return -sin(x); }

double f_x2(double x)     { return x * x; }
double f_exsinx(double x) { return exp(x) * sin(x); }

// ============================================================
// FIZIKALNE FUNKCIJE
// ============================================================

// Lennard-Jones potencial (normalizirani parametri: eps=1, sigma=1)
// U(r) = 4 * [(1/r)^12 - (1/r)^6]
double U_LJ(double r) {
    double sr6  = pow(1.0 / r, 6);
    double sr12 = sr6 * sr6;
    return 4.0 * (sr12 - sr6);
}

// Sila Lennard-Jones: F(r) = -dU/dr (analitično)
// F(r) = 4 * [12/r^13 - 6/r^7]
double F_LJ(double r) {
    double sr6  = pow(1.0 / r, 6);
    double sr12 = sr6 * sr6;
    return (4.0 / r) * (12.0 * sr12 - 6.0 * sr6);
}

// Debyejev integrand: x^3 / (e^x - 1)
// Singularnost pri x=0 je odpravljiva: lim_{x->0} = 0, zato začnemo pri eps > 0
double debye_integrand(double x) {
    return (x * x * x) / (exp(x) - 1.0);
}

// Sila nelinearne vzmeti: F(x) = k1*x + k2*x^3
static double k1 = 100.0, k2 = 500.0;
double F_vzmet(double x) { return k1 * x + k2 * x * x * x; }

// ============================================================
// POMOZNA FUNKCIJA: Newton-Raphson z numeričnim odvodom
// g    - funkcija, katere ničlo iščemo
// x0   - začetni približ
// h    - korak za numerični odvod
// tol  - toleranca za konvergenco
// max_iter - največ iteracij
// ============================================================
double newton_raphson(double (*g)(double), double x0, double h, double tol, int max_iter) {
    double x = x0;
    for (int i = 0; i < max_iter; i++) {
        double gx  = g(x);
        double dgx = central_difference(g, x, h);
        if (fabs(dgx) < 1e-15) break;
        double dx = -gx / dgx;
        x += dx;
        if (fabs(dx) < tol) break;
    }
    return x;
}

// ============================================================
// MAIN
// ============================================================

int main() {
    FILE* data;

    printf("=== VAJE 02: Numericno odvajanje in integriranje ===\n\n");

    // --------------------------------------------------------
    // NALOGA 1.2: Analiza napake pri numeričnem odvajanju
    // f(x) = sin(x) v točki x = 0.5
    // h geometrijsko od 1e-6 do 1, M = 20 vrednosti
    // --------------------------------------------------------
    printf("--- Naloga 1.2: Analiza napake odvajanja ---\n");
    {
        double x0      = 0.5;
        double h_start = 1e-6, h_stop = 1.0;
        int    M       = 20;
        double dh      = geometric_factor(h_start, h_stop, M);
        double exact   = df_sin(x0);

        data = fopen( "naloga_1_2.txt", "w+");
        fprintf(data, "h\t\t\tForward\t\t\tCentral\t\t\tCD4\t\t\tErr_FD\t\t\tErr_CD\t\t\tErr_CD4\n");

        double h = h_start;
        for (int i = 0; i < M; i++) {
            double fd  = forward_difference(f_sin, x0, h);
            double cd  = central_difference(f_sin, x0, h);
            double cd4 = central_difference4(f_sin, x0, h);
            fprintf(data, "%.6e\t%.10f\t%.10f\t%.10f\t%.6e\t%.6e\t%.6e\n",
                h, fd, cd, cd4,
                abs_error(fd,  exact),
                abs_error(cd,  exact),
                abs_error(cd4, exact));
            h *= dh;
        }
        fclose(data);
        printf("Analiticna vrednost f'(0.5) = cos(0.5) = %.10f\n", exact);
        printf("Rezultati -> naloga_1_2.txt\n\n");
    }

    // --------------------------------------------------------
    // NALOGA 1.3: Odvod v intervalu [0, 2*pi]
    // N = 100 točk, korak h = 0.01
    // --------------------------------------------------------
    printf("--- Naloga 1.3: Odvod v intervalu [0, 2*pi] ---\n");
    {
        int    N  = 100;
        double h  = 0.01;
        double dx = (2.0 * PI) / (double)N;

        data = fopen("naloga_1_3.txt", "w+");
        fprintf(data, "x\t\tAnaliticno\tForward\t\tCentral\t\tCD4\t\tErr_FD\t\tErr_CD\t\tErr_CD4\n");

        for (int i = 0; i <= N; i++) {
            double x     = i * dx;
            double exact = df_sin(x);
            double fd    = forward_difference(f_sin, x, h);
            double cd    = central_difference(f_sin, x, h);
            double cd4   = central_difference4(f_sin, x, h);
            fprintf(data, "%.6f\t%.8f\t%.8f\t%.8f\t%.8f\t%.6e\t%.6e\t%.6e\n",
                x, exact, fd, cd, cd4,
                abs_error(fd,  exact),
                abs_error(cd,  exact),
                abs_error(cd4, exact));
        }
        fclose(data);
        printf("Rezultati -> naloga_1_3.txt\n\n");
    }

    // --------------------------------------------------------
    // NALOGA 2.2: Analiza napake pri numeričnem integriranju
    // Funkcije: sin(x), x^2, e^x*sin(x) na [-1, 1]
    // h geometrijsko od 1e-3 do 1, M = 20 vrednosti
    // --------------------------------------------------------
    printf("--- Naloga 2.2: Analiza napake integriranja ---\n");
    {
        double a = -1.0, b = 1.0;
        double h_start = 1e-3, h_stop = 1.0;
        int    M  = 20;
        double dh = geometric_factor(h_start, h_stop, M);

        // Analitične vrednosti
        double exact_sin    = 0.0;          // ∫_{-1}^{1} sin(x)dx = 0 (liha f.)
        double exact_x2     = 2.0 / 3.0;   // ∫_{-1}^{1} x^2 dx = 2/3
        // ∫_{-1}^{1} e^x*sin(x)dx = [e^x*(sin(x)-cos(x))/2]_{-1}^{1}
        double exact_exsinx = (exp(1.0)*(sin(1.0) - cos(1.0))
                             - exp(-1.0)*(sin(-1.0) - cos(-1.0))) / 2.0;

        printf("Analiticne vrednosti:\n");
        printf("  integral sin(x) na [-1,1]       = %.10f\n", exact_sin);
        printf("  integral x^2   na [-1,1]         = %.10f\n", exact_x2);
        printf("  integral e^x*sin(x) na [-1,1]    = %.10f\n\n", exact_exsinx);

        const char* filenames[] = { "naloga_2_2_sin.txt", "naloga_2_2_x2.txt", "naloga_2_2_exsinx.txt" };
        double (*funcs[])(double) = { f_sin, f_x2, f_exsinx };
        double exacts[] = { exact_sin, exact_x2, exact_exsinx };

        for (int fi = 0; fi < 3; fi++) {
            data = fopen(filenames[fi], "w+");
            fprintf(data, "h\t\t\tn\t\tRect\t\tTrap\t\tSimps\t\tErr_Rect\tErr_Trap\tErr_Simps\n");

            double h = h_start;
            for (int i = 0; i < M; i++) {
                int n = (int)((b - a) / h);
                if (n < 4) n = 4;
                double rect  = rectangle_rule(funcs[fi], a, b, n);
                double trap  = trapezoid_rule(funcs[fi], a, b, n);
                double simps = simpson_rule(funcs[fi], a, b, n);
                fprintf(data, "%.6e\t%d\t\t%.10f\t%.10f\t%.10f\t%.6e\t%.6e\t%.6e\n",
                    h, n, rect, trap, simps,
                    abs_error(rect,  exacts[fi]),
                    abs_error(trap,  exacts[fi]),
                    abs_error(simps, exacts[fi]));
                h *= dh;
            }
            fclose(data);
            printf("Rezultati -> %s\n", filenames[fi]);
        }
        printf("\n");
    }

    // --------------------------------------------------------
    // SAMOSTOJNO 1: Delo pri raztezanju nelinearne vzmeti
    // F(x) = k1*x + k2*x^3,  k1=100 N/m, k2=500 N/m^3
    // W = integral_0^0.2 F(x) dx
    // --------------------------------------------------------
    printf("--- Samostojno 1: Nelinearna vzmet ---\n");
    {
        double x1 = 0.0, x2 = 0.2;
        // Analitično: W = k1*x2^2/2 + k2*x2^4/4
        double exact_W = 0.5 * k1 * x2*x2 + 0.25 * k2 * x2*x2*x2*x2;
        printf("Analiticna vrednost W = %.6f J\n", exact_W);

        data = fopen("samostojno_1.txt", "w+");
        fprintf(data, "Nelinearna vzmet: F(x) = k1*x + k2*x^3,  k1=%.0f N/m, k2=%.0f N/m^3\n", k1, k2);
        fprintf(data, "W = integral_0^%.1f F(x) dx,  analiticno: W = %.6f J\n\n", x2, exact_W);
        fprintf(data, "N\t\tTrapezno\t\tSimpsonovo\t\tErr_Trap\t\tErr_Simps\n");

        int Nvals[] = { 10, 100, 1000, 10000 };
        for (int j = 0; j < 4; j++) {
            int n = Nvals[j];
            double trap  = trapezoid_rule(F_vzmet, x1, x2, n);
            double simps = simpson_rule(F_vzmet, x1, x2, n);
            fprintf(data, "%d\t\t%.10f\t\t%.10f\t\t%.6e\t\t%.6e\n",
                n, trap, simps,
                abs_error(trap,  exact_W),
                abs_error(simps, exact_W));
            printf("N=%5d: Trap=%.8f J, Simps=%.8f J, Err_T=%.2e, Err_S=%.2e\n",
                n, trap, simps,
                abs_error(trap,  exact_W),
                abs_error(simps, exact_W));
        }
        fclose(data);
        printf("Rezultati -> samostojno_1.txt\n\n");
    }

    // --------------------------------------------------------
    // SAMOSTOJNO 2: Toplotna kapaciteta – Debyejev model
    // I = integral_eps^x_D  x^3/(e^x - 1) dx,  x_D = 1
    // Analiza vpliva eps in N
    // --------------------------------------------------------
    printf("--- Samostojno 2: Debyejev model ---\n");
    {
        double x_D = 1.0;
        data = fopen("samostojno_2.txt", "w+");
        fprintf(data, "Debyejev integrand: x^3/(e^x - 1),  x_D = %.1f\n\n", x_D);

        // Vpliv eps pri fiksnem N
        int N_fixed = 10000;
        fprintf(data, "--- Vpliv spodnje meje eps (N = %d) ---\n", N_fixed);
        fprintf(data, "eps\t\t\tRect\t\t\tTrap\t\t\tSimps\n");
        printf("Vpliv eps (N=%d):\n", N_fixed);

        double eps_vals[] = { 1e-1, 1e-3, 1e-6, 1e-10 };
        for (int j = 0; j < 4; j++) {
            double eps   = eps_vals[j];
            double rect  = rectangle_rule(debye_integrand, eps, x_D, N_fixed);
            double trap  = trapezoid_rule(debye_integrand, eps, x_D, N_fixed);
            double simps = simpson_rule(debye_integrand, eps, x_D, N_fixed);
            fprintf(data, "%.1e\t\t\t%.10f\t\t%.10f\t\t%.10f\n", eps, rect, trap, simps);
            printf("  eps=%.1e: Rect=%.8f, Trap=%.8f, Simps=%.8f\n", eps, rect, trap, simps);
        }

        // Vpliv N pri fiksnem eps
        double eps_fixed = 1e-10;
        fprintf(data, "\n--- Vpliv stevila delitev N (eps = %.1e) ---\n", eps_fixed);
        fprintf(data, "N\t\tRect\t\t\tTrap\t\t\tSimps\n");
        printf("Vpliv N (eps=%.1e):\n", eps_fixed);

        int N_vals[] = { 10, 100, 1000, 10000 };
        for (int j = 0; j < 4; j++) {
            int n    = N_vals[j];
            double rect  = rectangle_rule(debye_integrand, eps_fixed, x_D, n);
            double trap  = trapezoid_rule(debye_integrand, eps_fixed, x_D, n);
            double simps = simpson_rule(debye_integrand, eps_fixed, x_D, n);
            fprintf(data, "%d\t\t%.10f\t\t%.10f\t\t%.10f\n", n, rect, trap, simps);
            printf("  N=%5d: Rect=%.8f, Trap=%.8f, Simps=%.8f\n", n, rect, trap, simps);
        }
        fclose(data);
        printf("Rezultati -> samostojno_2.txt\n\n");
    }

    // --------------------------------------------------------
    // SAMOSTOJNO 3: Lennard-Jones – ravnovesna razdalja + opravljeno delo
    // U(r) = 4*eps*[(sigma/r)^12 - (sigma/r)^6], eps=1, sigma=1
    // F(r) = -dU/dr = 0  →  r_eq = 2^(1/6)
    // Newtonova metoda z numeričnim odvodom F'(r)
    // W = integral_{0.8}^{r_eq} F(r) dr
    // --------------------------------------------------------
    printf("--- Samostojno 3: Lennard-Jones potencial ---\n");
    {
        double h_num   = 1e-5;
        double tol     = 1e-10;
        int    max_it  = 100;
        double r0      = 1.2;   // začetni približ blizu r_eq

        double r_eq      = newton_raphson(F_LJ, r0, h_num, tol, max_it);
        double r_analytic = pow(2.0, 1.0 / 6.0);   // 2^(1/6) * sigma

        printf("Ravnovesna razdalja (Newton):  r_eq = %.10f\n", r_eq);
        printf("Analiticna vrednost:           r_eq = %.10f\n", r_analytic);
        printf("Napaka:                              = %.2e\n", fabs(r_eq - r_analytic));

        // Delo F_LJ od r=0.8 do r_eq
        double r_start = 0.8;
        int    N       = 10000;
        double W_trap  = trapezoid_rule(F_LJ, r_start, r_eq, N);
        double W_simps = simpson_rule(F_LJ, r_start, r_eq, N);
        // Analitično: W = -Delta_U = U(r_start) - U(r_eq)
        double W_exact = U_LJ(r_start) - U_LJ(r_eq);

        printf("\nDelo sile F_LJ od r=%.2f do r_eq:\n", r_start);
        printf("  Trapezno:    W = %.8f [eps]\n", W_trap);
        printf("  Simpsonovo:  W = %.8f [eps]\n", W_simps);
        printf("  Analiticno:  W = %.8f [eps]  (= U(r_start) - U(r_eq))\n", W_exact);

        data = fopen("samostojno_3.txt", "w+");
        fprintf(data, "Lennard-Jones: U(r) = 4*[(1/r)^12 - (1/r)^6], eps=1, sigma=1\n\n");
        fprintf(data, "Ravnovesna razdalja (Newton-Raphson):\n");
        fprintf(data, "  r_eq     = %.10f\n", r_eq);
        fprintf(data, "  analiticno = %.10f  (2^(1/6))\n", r_analytic);
        fprintf(data, "  napaka   = %.2e\n\n", fabs(r_eq - r_analytic));
        fprintf(data, "Delo F_LJ od r=%.2f do r_eq (N=%d):\n", r_start, N);
        fprintf(data, "  Trapezno:    W = %.10f [eps]\n", W_trap);
        fprintf(data, "  Simpsonovo:  W = %.10f [eps]\n", W_simps);
        fprintf(data, "  Analiticno:  W = %.10f [eps]\n\n", W_exact);

        // Potek U(r) in F(r) za graf
        fprintf(data, "r\t\tU(r)\t\tF(r)\n");
        for (double r = 0.85; r <= 3.0; r += 0.05) {
            fprintf(data, "%.4f\t\t%.8f\t%.8f\n", r, U_LJ(r), F_LJ(r));
        }
        fclose(data);
        printf("Rezultati -> samostojno_3.txt\n\n");
    }

    printf("=== Vse naloge dokoncane ===\n");
    return 0;
}
