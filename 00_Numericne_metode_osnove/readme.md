# VAJE 00: Osnove programiranja za Fizike

## Uvod

Pri teh vajah se bomo osredotočili na osnovne koncepte programiranja. Ustvariti želimo trdna temelja, na katerih bomo gradili bolj kompleksne programe v prihodnosti. V teh vajah bomo tako spoznali:

- Temeljna struktura programa v C/C++, definiranje in uporaba spremenljivk
- Definicija in uporaba polj in pointerjev
- Logične operacije in zanka
- I/O operacije
- Uporaba funkcij in modularnost programa

Vse te koncepte bomo spoznali skozi praktične naloge, ki jih boste reševali v C/C++. Vse naloge bodo osredotočene na osnovne operacije, ki so ključne za razumevanje programiranja in bodo služile kot osnova za nadaljnje učenje.


## Struktura

Vsak program v C/C++ ima osnovno strukturo, ki vključuje. Primer takšne strukture je:

```cpp

#include <iostream> // Vključitev knjižnice za vhodno-izhodne operacije
#include "funkcije.h" // Vključitev vaše pomožne datoteke z funkcijami

using namespace std; // Uporaba standardnega imenskega prostora

#define PI 3.14159 // Definicija konstante PI

int main() { // Glavna funkcija, kjer se začne izvajanje programa
    // Tukaj napišite svojo kodo

    return 0; // Vrnitev 0 pomeni, da je program uspešno zaključen
}
```

## I/O operacije

V C++ za standardni vhod in izhod najpogosteje uporabljamo objektni pristop s knjižnico <iostream>.

- cout <<: Izpis podatkov na zaslon (Standard Output).
- cin >>: Branje podatkov s tipkovnice (Standard Input).
- cerr <<: Izpis napak (Standard Error).

```cpp
#include <iostream>
using namespace std;
#define PI 3.14159

int main() {
    double r;
    cout << "Vnesi polmer kroga: ";
    cin >> r;
    cout << "Obseg znaša: " << 2 * PI * r << endl;
    return 0;
}
```

Obstaja pa tudi možnost uporabe starejše knjižnice <stdio.h>, ki uporablja funkcije kot so printf() in scanf().
- printf(): Formatiran izpis podatkov.
- scanf(): Formatirano branje podatkov.
- fprintf(): Formatiran izpis podatkov v datoteko.
- fscanf(): Formatirano branje podatkov iz datoteke.
- sprintf(): Formatiran izpis podatkov v niz (string).
- snprintf(): Formatiran izpis podatkov v niz z omejitvijo velikosti.

```cpp
#include <stdio.h>
#define PI 3.14159

int main() {
    double r;
    printf("Vnesi polmer kroga: ");
    scanf("%lf", &r);
    printf("Obseg znaša: %lf\n", 2 * PI * r);
    return 0;
}
```

Pisanje in branje datotek:
- ofstream: Za pisanje v datoteko.
- ifstream: Za branje iz datoteke.
- fstream: Za branje in pisanje v datoteko.

```cpp
#include <fstream>
using namespace std;
#define PI 3.14159

int main()
{
    double r;
    cout << "Vnesi polmer kroga: ";
    cin >> r;

    ofstream datoteka("rezultati.txt"); // Odpri datoteko za pisanje
    if (datoteka.is_open()) {
        datoteka << "Obseg kroga z radijem " << r << " je: " << 2 * PI * r << endl; // Zapiši rezultat v datoteko
        datoteka.close(); // Zapri datoteko
    } else {
        cerr << "Ne morem odpreti datoteke!" << endl; // Izpiši napako, če datoteke ni mogoče odpreti
    }
    return 0;
}
```

Alternativno lahok uporabimo funkcije iz <stdio.h> za delo z datotekami:
- fopen(): Odpre datoteko.
- fclose(): Zapre datoteko.
- fprintf(): Formatiran izpis podatkov v datoteko.
- fscanf(): Formatirano branje podatkov iz datoteke.


```cpp
#include <stdio.h>
#define PI 3.14159

int main() {
    double r;
    printf("Vnesi polmer kroga: ");
    scanf("%lf", &r);
    FILE* datoteka = fopen("rezultati.txt", "w"); // Odpri datoteko za pisanje
    if (datoteka != NULL) {
        fprintf(datoteka, "Obseg kroga z radijem %lf je: %lf\n", r, 2 * PI * r); // Zapiši rezultat v datoteko
        fclose(datoteka); // Zapri datoteko
    } else {
        fprintf(stderr, "Ne morem odpreti datoteke!\n"); // Izpiši napako, če datoteke ni mogoče odpreti
    }
    return 0;
}
```

## Spremenljivke

Spremenljivke so imena za pomnilniške lokacije, kjer hranimo podatke. V fiziki je izbira pravega tipa ključna za natančnost izračuna.
- `int`: Celo število (npr. število delcev $N$).
- `double`: Realno število z dvojno natančnostjo (standard za fizikalne količine, npr. $\hbar$, masa).
- `char`: Posamezen simbol.
- `bool`: Logična vrednost (true/false).

Ob teh najpogosteje uporabljanih tipih je potrebno omeniti še:
- `float`: Realno število z enojno natančnostjo (manj natančno od `double`, redko se uporablja v fiziki).
- `long`: Celo število z večjo kapaciteto (uporablja se za zelo velika števila).
- `unsigned int`: Celo število brez predznaka (uporablja se, kadar vemo, da bodo vrednosti vedno pozitivne).
- `long long`: Celo število z še večjo kapaciteto (uporablja se za zelo velika števila, večja od `long`).

**Pravilo:** Vedno inicializirajte spremenljivke! Neinicializirana spremenljivka vsebuje "smeti" iz pomnilnika. Incicializacija pomeni, da vsaki spremenljivki ob njenem ustvarjanju dodelimo zaćetno vrednost. Na primer:

```cpp
int N = 0; // Inicializacija celoštevilske spremenljivke N
double hbar = 1.0545718e-34; // Inicializacija realnoštevilske spremenljivke hbar
char simbol = 'A'; // Inicializacija znakovne spremenljivke simbol
bool jeRes = true; // Inicializacija logične spremenljivke jeRes
```

## Polja

Polja (Arrays) omogočajo shranjevanje **več vrednosti istega tipa** v zaporednih pomnilniških blokih. Idealna so za komponente **vektorjev** ali **časovne vrstne meritve**. Definicija polja je zelo podobna definiciji spremenljivk, vendar z dodatkom oglatih oklepajev `[]`. Ob določitvi tipa in imena polja, moramo navesti tudi **velikost polja**, ki določa, koliko elementov lahko shranimo. Na primer:

- Če želimo shraniti 5 meritev, lahko definiramo polje takole:
```cpp
int N = 5; // Velikost polja
double meritve[N] =  {0}; // Definicija polja za shranjevanje meritev
```

- Če pa bi hoteli shraniti 100 leg delcev v 3D prostoru, bi definirali polje takole:
```cpp
int N = 100; // Število delcev
double delci[N][3] = {0}; // Definicija polja za shranjevanje položajev delcev v 3D prostoru
```

Omeniti je potrebno, da velikost polja mora biti znana v času prevajanja programa (compile-time). To pomeni, da **ne moremo uporabiti spremenljivke** za določanje velikosti polja. Polja so **statična**, kar pomeni, da se njihova velikost ne more spreminjati med izvajanjem programa.

Prednost polj je:
- So zelo hitra in enostavna za uporabo.
- Omogočajo enostaven dostop do elementov preko indeksov.
- So učinkovita za shranjevanje in obdelavo velikih količin podatkov, kadar je velikost znana vnaprej.

Slabosti polj so:
- Velikost mora biti znana v času prevajanja, kar omejuje njihovo fleksibilnost.
- Ne omogočajo dinamičnega spreminjanja velikosti med izvajanjem programa.
- Ne izvajajo samodejnega upravljanja pomnilnika, kar lahko vodi do napak, kot so prekoračitve polja (buffer overflow) ali puščanje pomnilnika (memory leaks), če niso pravilno uporabljena.

## Pointerji

V nasprotju s polji, pointerji (kazalec) omogočajo **dinamično dodeljevanje pomnilnika** in so ključni za delo z dinamičnimi strukturami podatkov. Pointer **je spremenljivka, ki shranjuje naslov druge spremenljivke**. To omogoča večjo fleksibilnost pri upravljanju pomnilnika, vendar zahteva tudi **večjo pozornost**, saj lahko nepravilna uporaba vodi do napak, kot so dereferenciranje null pointerja ali puščanje pomnilnika. V fizikalnih simulacijah jih uporabljamo za učinkovito upravljanje z velikimi količinami podatkov.

Uporaba pointerjev:
- `&`: Operator za pridobivanje naslova spremenljivke.
- `*`: Operator za dereferenciranje pointerja, ki omogoča dostop do vrednosti, na katero kaže pointer.

Primer uporabe pointerjev:
```cpp
double g = 9.81;
double* ptr = &g; // ptr zdaj kaže na lokacijo, kjer je shranjen 9.81
double vrednost = *ptr; // vrednost zdaj vsebuje 9.81
```

**Razlaga:** Predstavljajmo si, da se vaš program izvaja v pomnilniku.
- Ustvarimo spremenljivko `g`, ki shranjuje vrednost gravitacijske pospeška. Ta spremenljivka je shranjena na določeni lokaciji v pomnilniku, recimo na naslovu `0x7ffee3bff6c8`.
- Ko definiramo pointer `ptr` in mu dodelimo naslov `&g`, pointer `ptr` zdaj vsebuje vrednost `0x7ffee3bff6c8`, kar pomeni, da kaže na lokacijo, kjer je shranjena vrednost `9.81`.
- Ko **dereferenciramo** pointer z `*ptr`, dostopamo do vrednosti, ki je shranjena na tej lokaciji, in tako dobimo `9.81`.

Prednosti:
- Omogočajo dinamično dodeljevanje pomnilnika, kar je ključno za delo z velikimi podatkovnimi strukturami.
- Omogočajo učinkovito upravljanje z velikimi količinami podatkov, kar je pogosto potrebno v fizikalnih simulacijah.
- Omogočajo ustvarjanje kompleksnih podatkovnih struktur, kot so povezani seznami, drevesa in grafi.

Slabosti:
- Zahtevajo večjo pozornost pri upravljanju pomnilnika,
- Nepravilna uporaba lahko vodi do napak, kot so dereferenciranje null pointerja ali puščanje pomnilnika (memory leaks).
- Lahko so težki za razumevanje in uporabo, zlasti za začetnike.


## Logične operacije

Logične operacije nam omogočajo sprejemanje odločitev v kodi. V fiziki jih uporabljamo za preverjanje pogojev (npr. "ali je delec zapustil območje simulacije?" ali "ali je energija konvergirala?").

**Primerjalni operatorji:**
- == (enako),
- != (različno),
- < (manj),
- > (več),
- <= (manj ali enako),
- ->= (več ali enako).

**Logični vezniki:**
- && (IN - oba pogoja morata držati),
- || (ALI - vsaj eden mora držati),
- ! (NE - negacija).

**Pogojni stavek if-else:**
```cpp

double T = 105.0; // Temperatura vode
if (T >= 100.0) {
    cout << "Snov je v plinastem stanju." << endl;
} else if (T <= 0.0) {
    cout << "Snov je v trdnem stanju." << endl;
} else {
    cout << "Snov je v tekočem stanju." << endl;
}
```

## Zanke

Zanke so hrbtenica fizikalnih simulacij, saj omogočajo ponavljanje izračunov (npr. časovni koraki pri integraciji gibalnih enačb).

**Zanka `for`**
```cpp
double a = 0.0;
for (int i = 0; i < 10; i++) {
    a += 0.1; // Povečujemo a za 0.1 v vsakem koraku
}
```

**Zanka `while`**
```cpp
double b = 0.0;
while (b < 1.0) {
    b += 0.1; // Povečujemo b za 0.1 v vsakem koraku
}
```

## Funkcije

Funkcije so samostojni bloki kode, ki opravljajo specifično nalogo. V fiziki jih uporabljamo za zapis fizikalnih zakonov (npr. izračun sile, energije ali hitrosti), ki jih v programu večkrat pokličemo. S tem se izognemo podvajanju kode in zmanjšamo možnost napak.

Struktura funkcije:

- **Tip povratne vrednosti:** pove, kakšen podatek funkcija vrne (npr. double, int). Če ne vrne ničesar, uporabimo void.
- **Ime funkcije:** poljubno ime, ki opisuje namen.
- **Argumenti funkcije:** vrednosti, ki jih funkcija potrebuje za izračun.

```cpp
#include <iostream>
#include <cmath>
using namespace std;

// Definicija funkcije za kinetično energijo: E = 1/2 * m * v^2
double kineticna_energija(double m, double v) {
    double energija = 0.5 * m * pow(v, 2);
    return energija;
}

int main() {
    double masa = 2.5;
    double hitrost = 10.0;

    // Klic funkcije
    double Ek = kineticna_energija(masa, hitrost);

    cout << "Kinetična energija je: " << Ek << " J" << endl;
    return 0;
}
```

** Globalne in lokalne spremenljivke:**
- **Globalne spremenljivke** so dostopne v celotnem programu, vendar lahko povzročijo težave z vzdrževanjem kode in nepredvidljivim vedenjem, če jih uporabljamo nepravilno.
- **Lokalne spremenljivke** so dostopne samo znotraj funkcije, kjer so definirane, kar izboljšuje modularnost in varnost kode.

Zavedati se moramo, da je mesto, kjer se spremenljivka definira, ključno za njeno **vidljivost** in življenjsko dobo. Lokalna spremenljivka obstaja samo med izvajanjem funkcije, medtem ko globalna spremenljivka obstaja skozi celotno izvajanje programa.

Primer globalne spremenljivke:
```cpp
double globalna_spremenljivka = 42.0; // Globalna spremenljivka
double funkcija() {
    return globalna_spremenljivka; // Dostop do globalne spremenljivke znotraj funkcije
}
```

primer lokalne spremenljivke:
```cpp
double funkcija() {
    double lokalna_spremenljivka = 42.0; // Lokalna spremenljivka
    return lokalna_spremenljivka; // Dostop do lokalne spremenljivke znotraj funkcije
}
```

## Modularnost programa

Modularnost pomeni razdelitev programa na več manjših, logično povezanih enot ali datotek. Namesto ene ogromne datoteke `main.cpp`, kodo razbijemo na dele, ki jih je lažje vzdrževati, testirati in ponovno uporabiti.

V jeziku C/C++ modularnost dosežemo z uporabo dveh vrst datotek:
- **Header datoteke (.h):** vsebujejo deklaracije funkcij, struktur in konstant. So kot "pogodba", ki opisuje, kaj funkcija počne, brez razkrivanja, kako to počne.
- 
- **Izvorne datoteke (.cpp):** vsebujejo implementacijo funkcij, kjer je dejansko napisana koda, ki izvaja naloge, opisane v header datoteki.

**Zakaj uporabljati modularnost?**

- **Preglednost:** Glavna datoteka `main.cpp` vsebuje le logiko programa, fizikalni izračuni pa so "skriti" v modulih.
- **Ponovna uporaba:** Modul za računanje termodinamičnih lastnosti lahko preprosto kopirate v nov projekt.
- **Hitrejše prevajanje:** Ob spremembi ene datoteke prevajalnik ponovno prevede le to datoteko, ne pa celotnega projekta.

**Primer modularne strukture:**

Uporabljamo t.i. include guards (#ifndef, #define, #endif), da preprečimo večkratno vključevanje iste datoteke.

```cpp
#ifndef KROGLA_H
#define KROGLA_H

#define PI 3.14159265

// Deklaracije funkcij (brez telesa)
double povrsina_krogle(double r);
double volumen_krogle(double r);

#endif
```

**2. Datoteka: krogla.cpp (Implementacija)**

Tukaj vključimo lastno glavo in napišemo kodo funkcij.

```cpp
#include "krogla.h"
#include <cmath>

double povrsina_krogle(double r) {
    return 4.0 * PI * pow(r, 2);
}

double volumen_krogle(double r) {
    return (4.0 / 3.0) * PI * pow(r, 3);
}
```

**3. Datoteka: main.cpp (Uporaba)**
```cpp
#include <iostream>
#include "krogla.h" // Vključimo naš modul

using namespace std;

int main() {
    double radij = 5.0;
    
    cout << "Povrsina: " << povrsina_krogle(radij) << endl;
    cout << "Volumen: " << volumen_krogle(radij) << endl;
    
    return 0;
}
```

## Povzetek ključnih konceptov

V teh vajah smo postavili temelje, ki so nujni za vsakega fizika-programerja:

- Spremenljivke: Shranjevanje osnovnih fizikalnih parametrov.
- Polja in Pointerji: Delo z vektorji, matrikami in dinamičnimi podatki.
- Zanke in Logika: Avtomatizacija ponavljajočih se izračunov in simulacij.
- I/O Operacije: Branje meritev iz datotek in zapisovanje rezultatov simulacij.
- Funkcije in Modularnost: Organizacija kode v čiste, ponovno uporabne gradnike


## Naloge:

1. Naloga (Vektorska algebra): Ustvarite funkcijo, ki sprejme dve polji velikosti 3 (vektorja) in vrne njun skalarni produkt: $\vec{a} \cdot \vec{b} = \sum a_i b_i$.

2. Naloga (Numerično seštevanje): Z uporabo zanke for izračunajte vsoto prvih $N$ členov harmonične vrste $\sum_{n=1}^{N} \frac{1}{n}$. Program naj $N$ prebere s tipkovnice.

3. Naloga (Zapis trajektorije): Napišite program, ki v zanki računa položaj prostega pada $y(t) = y_0 - \frac{1}{2}gt^2$ za časovne korake $\Delta t = 0.1\,s$ in rezultate (čas in višino) sproti zapisuje v datoteko pad.dat.