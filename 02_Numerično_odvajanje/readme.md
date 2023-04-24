# Določevanje odvodov z numeričnimi metodami

Ta program implementira tri numerične metode za določevanje odvodov funkcij. Za izbrane primere so uporabljene tudi analizične vrednosti odvoda funkcije. S tem lahko določamo natančnost posmezne metode.

## Numerične metode
Program implementira naslednje numerične metode za določevanje odvodov funkcij:

- Enostranska diferenca (df_en1): $f'(x)\approx \frac{f(x+h)-f(x)}{h}$
- Obe stranski diferenca (df_en2): $f'(x)\approx \frac{f(x+h)-f(x-h)}{2h}$
- Središčna diferenca (df_en3): $f'(x)\approx \frac{f(x+h)+f(x-h)-2*f(x)}{4h}$

## Primer uporabe
V datoteki main.cpp sta napisani funkciji f in dfdx, ki predstavljata funkcijo, katere odvod iščemo, in njen analitični odvod. V glavni funkciji programa (main) je nato prikazano, kako se za različne vrednosti h izračuna numerični odvod in se ta primerja z analitično izračunanim odvodom.

Program izpiše rezultate v dve datoteki, naloga_1.txt in naloga_2.txt. V datoteki naloga_1.txt so rezultati za določevanje odvoda v eni točki, v datoteki naloga_2.txt pa so rezultati za določevanje odvoda na celotnem intervalu.

Navodila za uporabo
Za uporabo programa sledite spodnjim korakom:

Prenesite celotno kodo v mapo na vašem računalniku.
Odprite datoteko main.cpp v vašem najljubšem urejevalniku besedila.
Zaženite program in sledite izpisom v konzoli.


## Naloga 1

Za funkcijo $f(x) = sin(x)$ numerično in analitično izračunaj vrednost odvoda v točki $x = 0,5$. Pri tem uporabite pravilo za eno- in dvo-stranski numerični odvod. To napravi za različne vrednosti intervala $ 10^{-6} \leq h \leq 1\ $. Vzporedno naj program tudi izpisuje, odstopanje od analitično določene vrednosti odvoda, pri posameznem primeru. Za povečevanje $h$-ja uporabimo geometrijsko zaporedje $h_{n+1}=f*{h}_n$. Vrednost faktorja f naj bo takšna, da bo interaval vrednosti $h$-ja razdeljen na **M** vrednosti. Rezultate izvozite v datoteko in jih prikaži v grafu.

## Naloga 2

Za isto funkcijo $f(x)\ = sin(x)$ numerično določite vrednosti odvodov v intervalu $ 0 \leq x \leq 2 \pi $. Interval od $ 0 $ do $2 \pi$ razdelite na **N** delov. V datoteko si sprotno zapisujte vrednosti za $x$, numerično določene vrednosti odvodov, analitično določene vrednosti odvodov ter absolutne napake. Rezultate izvozite v datoteko in jih prikaži v grafu.

Oglejte si rezultate v datotekah `naloga_1.txt` in `naloga_2.txt`.