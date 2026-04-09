# Naključna števila

Naključna števila so števila, katerih zaporedje ne moremo razumsko napovedati, temveč so posledica naključnega izbora. Realni sistemi velikokrat niso deterministični — določena vrednost vhodne spremenljivke $x$ ne izzove vedno iste izhodne vrednosti $x \to y$. Zelo pomembno je, da znamo generirati naključna števila in ovrednotiti, kako naključna so.


# Kako sploh generiramo naključna števila

Metoda **L**inearnega **K**ongruenčnega **G**eneratorja je ena izmed najbolj preprostih in pogosto uporabljenih metod za generiranje zaporedij naključnih števil. Ta metoda temelji na matematični formuli, ki generira zaporedje števil z uporabo predhodnega števila v zaporedju in konstante. Splošna oblika formule za generiranje zaporedja je:

$$X_{n+1} = (a \cdot X_{n} + c) \mod m $$

kjer je:

- $X_{n}$ število v $n$-ti iteraciji (tj. $n$-ti člen zaporedja),
- $a$ množitelj (angl. multiplier),
- $c$ inkrement (angl. increment),
- $m$ modulo (angl. modulus).

Veljati mora tudi:
- $m > 0$
- $0 < a < m$
- $0 < c < m$
- $0 < X_0 < m$

Za začetno število $X_0$ se običajno uporablja neka vrednost, ki se pridobi iz sistema (npr. iz časa) ali jo uporabnik navede ročno (`seme` ali `seed`). Zaporedje števil, ki jih generira ta metoda, ima lahko omejeno dolžino, odvisno od izbrane vrednosti za $a$, $c$ in $m$.

Ta metoda lahko proizvede dolga zaporedja naključnih števil. Vendar pa so ti generatorji nagnjeni k ponavljajočim se vzorcem, če se izberejo neustrezne vrednosti. Poleg tega je treba biti previden pri izbiri začetnega števila $X_0$, saj slaba izbira povzroči ponavljajoče se vzorce.


# Random Generator

Ta program vsebuje štiri vaje in dve domači nalogi, kjer se uporablja generator naključnih števil.

## Vaja 1

V programskem jeziku C/C++ so implementirane funkcije za generiranje različnih tipov naključnih števil:

- `rand_int_ab(a, b)` — naravno naključno število v intervalu $[a, b]$,
- `rand_double()` — realno naključno število v intervalu $[0, 1)$,
- `rand_double_ab(a, b)` — realno naključno število v intervalu $[a, b]$.

Funkcija `vaje_01()` v datoteko `vaja_01.txt` zapiše $10^4$ vrstic s tremi stolpci naključnih števil ter preveri, ali so generirana števila res naključna.

## Vaja 2

Funkcija `vaje_02()` uporablja generator naključnih števil za določitev vrednosti števila $\pi$ z metodo zadetka in zgrešitve (hit-or-miss). Rezultate shranjuje v datoteko `Vaja_02.txt`, kjer je prikazano, kako se natančnost izračunane vrednosti $\pi$ spreminja v odvisnosti od števila generiranih naključnih točk.

## Vaja 3 — Problem Monty Hall

Funkcija `vaja_03()` simulira znani problem Monty Hall. V igri nastopajo tri vrata, za katerimi se skriva ena nagrada (npr. avtomobil), za ostalima dvema pa ne (npr. koza). Igralec najprej naključno izbere ena vrata. Voditelj nato odpre ena izmed preostalih dveh vrat, za katerimi zagotovo ni nagrade. Simulacija statistično primerja dve strategiji:

- strategijo, pri kateri igralec **ne zamenja** začetne izbire,
- strategijo, pri kateri igralec **vedno zamenja** izbiro po tem, ko voditelj odpre vrata.

Rezultati (verjetnost zmage za vsako strategijo) se sproti zapisujejo v datoteko `Vaja_03.txt`.

## Vaja 4 — Simulacija igre Loto

Funkcije `loto_generator()`, `igra_loto()` in pomožne funkcije `sortiraj()`, `izpisi_listek()` ter `izpisi_rezultat()` simulirajo igro loto. Funkcija `loto_generator()` generira naključno kombinacijo 7 različnih števil med 1 in 39. Funkcija `igra_loto()` simulira določeno število krogov loto igre in beleži, kolikokrat se je v posameznem krogu ujemalo 0, 1, 2, … ali 7 izžrebanih in naključno izbranih števil.

## Domača naloga 1 — Razširjeni problem Monty Hall

Razširitev problema Monty Hall na primere s 4, 5 in 6 vrati. Za enimi vrati je skrita nagrada, za ostalimi pa ne. Simulacija primerja obe strategiji (ostani / zamenjaj) za vsak primer pri velikem številu ponovitev in izračuna relativno verjetnost zmage.

## Domača naloga 2 — Dinamična vs. statična loto strategija

Simulacija primerja dve strategiji igranja lota:
- **Statična strategija** (kot v Vaji 4): v vsakem krogu enaka kombinacija števil,
- **Dinamična strategija**: v vsakem krogu nova naključno generirana kombinacija.

Uspešnost obeh strategij se primerja kratkoročno in dolgoročno na podlagi večjega števila simuliranih krogov.


## Kako uporabiti program?

Zaženite datoteko `Random_generator.cpp` v razvojnem okolju ali terminalu. Program zaporedno izvede vse vaje in ustvari izhodne datoteke `vaja_01.txt`, `Vaja_02.txt` in `Vaja_03.txt`.

Program je bil napisan in testiran na operacijskem sistemu Windows s programskim jezikom C++. Za druge operacijske sisteme ali razvojna okolja bo morda potrebna prilagoditev.


# Funkcije v datoteki `random_nenerator.h`

| Funkcija | Opis |
|---|---|
| `int rand_int_ab(int a, int b)` | Vrne naključno celo število med $a$ in $b$ (vključno z mejama). |
| `double rand_double()` | Vrne naključno realno število med 0 in 1. |
| `double rand_double_ab(double a, double b)` | Vrne naključno realno število med $a$ in $b$. |
| `void vaje_01()` | Zapiše $10^4$ vrstic naključnih števil v `vaja_01.txt`. |
| `bool hit_or_miss(double x, double y, double polmer)` | Vrne `true`, če točka $(x, y)$ leži znotraj kroga s polmerom `polmer`. |
| `void vaje_02(int N)` | Oceni vrednost $\pi$ in zapiše rezultate v `Vaja_02.txt`. |
| `vector<double> vaja_03(int N)` | Simulira Monty Hall problem in vrne verjetnosti zmage za obe strategiji. |
| `vector<int> loto_generator()` | Vrne 7 naključno izbranih različnih števil med 1 in 39. |
| `vector<int> sortiraj(vector<int>& x)` | Uredi vektor celih števil v naraščajočem vrstnem redu. |
| `void izpisi_listek(vector<int> x)` | Izpiše elemente loto listka. |
| `void izpisi_rezultat(vector<int> x)` | Izpiše, kolikokrat se je pojavilo posamezno število ujemanj. |
| `void igra_loto()` | Simulira igro loto in beleži rezultate po krogih. |
