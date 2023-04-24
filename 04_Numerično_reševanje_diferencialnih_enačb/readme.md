# Numerično reševanje diferencialnih enačb

V tej vaji se ukvarjamo z numeričnim reševanjem diferencialnih enačb. V prvem delu implementiramo tri numerične metode za izračun integralov: pravilo pravokotnikov, trapezno pravilo in Simpsonovo pravilo. V drugem delu pa rešujemo diferencialno enačbo navadnega diferencialnega enačbe s pomočjo dveh numeričnih metod: Eulerjevo metodo in Runge-Kutta 4. reda.

## Naloga 1 - Numerično integriranje

V datoteki `num_int.h` so definirane tri numerične metode za izračun integralov:
* pravilo pravokotnikov: `pravilo_pravokotnikov`
* trapezno pravilo: `trapezno_pravilo`
* Simpsonovo pravilo: `simpsonovo_pravilo`

Vsaka metoda sprejme spodnjo in zgornjo mejo integrala ter širino intervala. Implementirane so za funkcije sin(x), x^2 ter e^-x*sin(x).

Implementirane metode so testirane v datoteki `main.cpp`, kjer so napisane funkcije, ki izračunajo natančnost metode glede na analitično vrednost integracije. Rezultati so zapisani v datoteki `numericno_integiranje.txt`.

## Naloga 2 - Numerično reševanje diferencialnih enačb

V datoteki `solve_ode.h` so definirane dve numerični metodi za reševanje diferencialnih enačb:
* Eulerjeva metoda: `ode_Euler_solver`
* Runge-Kutta 4. reda: `ode_RK4_solver`

Vsaka metoda sprejme vrednost neodvisne spremenljivke, vrednost odvisne spremenljivke ter korak. Implementirane so za diferencialno enačbo y'(x) = (5x^2 - y) / e^(x+y).

V datoteki `main.cpp` je implementiran padajoči gib telesa s pomočjo diferencialne enačbe drugega reda. Uporabljeni sta Eulerjeva metoda in Runge-Kutta 4. reda. Rezultati so zapisani v datoteki `rezultati_vaje_04_pt2.txt`. Funkcija za padajoči gib upošteva upor ter težnost, za njuno izračun uporablja izračune iz prvega dela naloge.

## Navodila za zagon

1. Odprite terminal in se premaknite v mapo z datotekami
2. Zaženite ukaz `make` (ali `mingw32-make`, odvisno od sistema)
3. Zaženite datoteko `main.exe` z ukazom `./main.exe`

## Zahteve

Program je bil uspešno preizkušen na naslednjih sistemih:
* Windows 10, GCC 11.2.0
* Ubuntu 20.04.3 LTS,