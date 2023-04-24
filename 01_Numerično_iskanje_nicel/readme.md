# Iskanje ničel funkcij

V tem projektu so implementirane tri metode za iskanje ničel funkcij:

1. Metoda bisekcije
2. Sekantna metoda
3. Newtonova metoda

V datoteki `funkcije.h` se nahajajo definicije funkcij, ki se uporabljajo pri izvajanju metod.

## Primer uporabe

V datoteki `main.cpp` je napisan primer uporabe, kjer se za funkcije sproti preverja, če so začetni pogoji primerni in se nato po potrebi uporabi ena od treh metod za iskanje ničel. Nato se izpišejo najdene ničle za vsako funkcijo.

```c++
double a, b;

a = -10.0;
b = 10.0;

int metoda = 2;
int N = 100000;

for (int i = 1; i < 5; i++) {
    int OK = preveri_init(a, b, i, metoda, N);
    printf("%d\n", OK);
}
```

V tem primeru se najprej določi interval $[a,b]$ v katerem bomo iskali ničle, nato se določi metoda za iskanje ničel in končno se za vsako funkcijo preveri, če so začetni pogoji primerni in se po potrebi uporabi izbrana metoda za iskanje ničel. Funkcija `preveri_init` izpiše število najdenih ničel in vrednosti najdenih ničel.