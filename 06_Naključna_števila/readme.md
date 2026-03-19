# Naključna števila

Realni sistemi velikokrat niso deterministični. Slednje pomeni, da določena vrednost vhodne spremenljivke ne izzove vedno iste izhodne vrednosti. Zelo pomembno je, da znano generirati naključna števila in tudi ovrednotiti, kako naključna so števila, ki so bila generirana.


# Kako sploh generiramo naključna števila

Metoda linearnega kongruenčnega generatorja je ena izmed najbolj preprostih in pogosto uporabljenih metod za generiranje zaporedij naključnih števil. Ta metoda temelji na matematični formuli, ki generira zaporedje števil z uporabo predhodnega števila v zaporedju in konstante. Splošna oblika formule za generiranje zaporedja je:

$$X_{n+1} = (a * X_{n} + c) \mod m $$

kjer je:

- $X_{n}$ število v $n$-ti iteraciji (tj. $n$-ti člen zaporedja),
- $a$ množitelj (angl. multiplier),
- $c$ inkrement (angl. increment),
- $m$ modulo (angl. modulus).

Veljati mora tudi:
- $m$ > 0
- 0 < $a$ < $m$
- 0 < $c$ < $m$
- 0 < $X_0$ < $m$

Za začetno število $X_0$ se običajno uporablja neka vrednost, ki se lahko pridobi iz sistema (npr. iz časa) ali jo lahko uporabnik navede ročno (`seme` ali `seed`). Zaporedje števil, ki jih generira ta metoda, ima lahko omejeno dolžino, kar je odvisno od izbrane vrednosti za *`a`*, *`c`* in *`m`*.

Ta metoda lahko proizvede dolge zaporedje naključnih števil. Vendar pa so ti generatorji nagnjeni k ponavljajočim se vzorcem, če se izberejo neustrezne vrednosti za *`a`*, *`c`* in *`m`*. Poleg tega je treba biti previden pri izbiri začetnega števila $X_0$, saj lahko slaba izbira povzroči, da se v zaporedju pojavijo ponavljajoči se vzorci. Zato je pomembno, da izberemo primerna števila za *`a`*, *`c`* in *`m`* ter da izberemo ustrezno vrednost za $X_0$.

# Random Generator

Ta program vsebuje tri vaje, kjer se uporablja generator naključnih števil.

## Vaja 1

Funkcije `rand_int_ab()`, `rand_double()` in `rand_double_ab()` generirajo naključna števila, ki se uporabijo v funkciji `vaje_01()`. Ta funkcija v datoteko "vaja_01.txt" zapiše $10^4$ vrstic s tremi stolpci. Vsaka vrstica vsebuje tri naključna števila.

## Vaja 2

Funkcija `vaje_02()` uporablja generator naključnih števil in določi vrednost števila $\\pi$. Generira tudi datoteko "Vaja_02.txt", kjer je prikazano, kako se natančnost izračunane vrednosti števila $\\pi$ spreminja v odvisnosti od števila neneriranih naključnih števil.

## Vaja 3

Funkcije `loto_generator()`, `izpisi()` in `igra_loto()` uporabljamo za simulacijo igre loto. Funkcija `loto_generator()` generira naključno kombinacijo sedmih števil med 1 in 39, funkcija `igra_loto()` pa simulira določeno število krogov loto igre, kjer primerjamo naključno generirane kombinacije s kombinacijo, ki smo jo generirali z `loto_generator()`.

## Kako uporabiti program?

Za uporabo programa zaženite datoteko "Random\_generator.cpp" v razvojnem okolju ali terminalu. Ko program teče, bo najprej izvedena vaja 1, ki ustvari datoteko "vaja\_01.txt". Sledi vaja 2, ki ustvari datoteko "Vaja_02.txt". Na koncu se izvede še vaja 3, ki simulira krog loto igre.

Upoštevajte, da je bil ta program napisan in testiran na operacijskem sistemu Windows s programskim jezikom C++. Morda bo potrebno prilagoditi kodo za druge operacijske sisteme ali razvojna okolja.

# V datoteki random_generator.h se nahajajo naslednje funkcije:

`int rand_int_ab(int a, int b)` Funkcija vrne naključno celo število med a in b, vključno z obema mejama.

`double rand_double()` Funkcija vrne naključno realno število med 0 in 1.

`double rand_double_ab(double a, double b)` Funkcija vrne naključno realno število med a in b.

`void vaje_01()` Funkcija v datoteko vaja_01.txt zapiše sample_size vrstic, v katerih se nahajajo naključna celoštevilska in realna števila.

`bool hit_or_miss(double x, double y,double polmer)`Funkcija sprejme koordinate točke (x, y) in polmer kroga, ter vrne true, če točka pripada krogu, in false, če ne.

`void vaje_02()`Funkcija v datoteko Vaja_02.txt zapiše ocene števila pi za različno število vrženih točk (do sample_size).

`vector<int> sortiraj(vector<int>& x)`Funkcija uredi vektor celih števil x v naraščajočem vrstnem redu.

`void izpisi(vector<int> x)`Funkcija izpiše elemente vektorja x.

`void izpisi_v2(vector<int> x)`Funkcija izpiše, kolikokrat se vsako število pojavlja v vektorju x.

`vector<int> loto_generator()`Funkcija vrne vektor 7 naključno izbranih števil med 1 in 39, urejenih v naraščajočem vrstnem redu.

`void igra_loto()`Funkcija simulira igro loto in zabeleži, kolikokrat se je v posameznem krogu ujemalo število izžrebanih in naključno izbranih števil.