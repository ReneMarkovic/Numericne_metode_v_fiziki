# VAJE 03: Numerično reševanje diferencialnih enačb

Ta del je posvečen implementaciji in analizi numeričnih metod za reševanje **navadnih diferencialnih enačb (NDE)**. Implementirali bomo dve klasični metodi: **Eulerjevo metodo** in **Runge-Kutta metodo 4. reda (RK4)**, ter ju primerjali na fizikalnem primeru prostega pada z zračnim uporom.

---

## Diferencialna enačba

Diferencialna enačba je matematična enačba, ki vključuje funkcijo in njene odvode. Reševanje diferencialnih enačb je ključno za razumevanje številnih naravnih pojavov, kot so gibanje teles, rast populacij, širjenje bolezni in še veliko več. Diferencialno enačbo prvega reda lahko zapišemo v splošni obliki:

$$\frac{dy}{dx} = y' = f(x, y)$$

Cilj reševanja diferencialne enačbe je najti funkcijo $y(x)$, ki zadovoljuje to enačbo. V mnogih primerih analitične rešitve ni mogoče najti, zato se zatečemo k numeričnim metodam za približevanje rešitev.

---

## 1. Eulerjeva metoda

Eulerjeva metoda je ena najpreprostejših metod za numerično reševanje diferencialnih enačb. Temelji na ideji, da lahko rešitev aproksimiramo z uporabo tangente na trenutni točki. Pri izpeljavi si pomagamo z razvojem funkcije $y(x)$ v Taylorjevo vrsto okoli točke $x_n$:

$$y(x + h) = y(x) + y'(x, y(x))\, h + \mathcal{O}(h^2)$$

Z aproksimacijo $y'(x, y(x))$ z $f(x, y)$ in zanemarjanjem višjih redov dobimo iteracijsko formulo:

$$\boxed{y_{n+1} = y_n + f(x_n, y_n)\, h}$$

Animacija postopka (korak po korak, z naraščajočo napako glede na eksaktno rešitev $e^x$):

![EulerjevaMetoda](./media/EulerjevaMetoda_ManimCE_v0.20.1.gif)

kjer je $h = \Delta x$ velikost koraka. Lokalna napaka metode je reda $\mathcal{O}(h^2)$, globalna napaka pa reda $\mathcal{O}(h)$ — Eulerjeva metoda je metoda **prvega reda**.

---

# 2.Metoda Runge-Kutta

Pri reševanju diferencialne enačbe $y' = f(t, y)$ želimo iz trenutnega stanja $(t_n, y_n)$ ugotoviti, kje bo sistem ob času $t_{n+1}$. **Eulerjeva** metoda preprosto pogleda nagib v začetni točki in po njem "zakorači" naprej. Težava je, da se nagib med korakom spreminja, zato Euler hitro skrene s poti.Runge-Kutta metode to rešijo tako, da znotraj enega koraka $h$ vzamejo več "vzorčnih" nagibov ($k_i$) na različnih mestih, jih pametno utežijo in izračunajo povprečen nagib, ki nas nato pripelje do naslednje točke z veliko večjo natančnostjo. Metodo Runge-Kutta (RK) n-tega reda dobimo tako, da vzamemo $n$ vzorčnih nagibov in jih ustrezno utežimo. Najbolj znana je **Runge-Kutta metoda 4. reda (RK4)**, ki uporablja štiri vzorčne nagibe in dosega odlično natančnost z enostavno implementacijo. Določevanje vsake uteži zahteva, da koeficienti zapišemo v Butcherjevo tabelo, ki zagotavlja, da metoda doseže želeni red natančnosti.

## 2.1 RK2 metoda

Runge-Kutta metoda drugega reda (RK2) je preprostejša različica RK4, ki uporablja dva vzorčna nagiba. Ena izmed najbolj znanih RK2 metod je metoda srednje točke:

$$
k_1 = f(x_n, y_n) \cdot h$$
$$
k_2 = f\!\left(x_n + \tfrac{h}{2},
y_n + \tfrac{k_1}{2}\right) \cdot h$$
$$
y_{n+1} = y_n + k_2
$$

Lokalna napaka metode je reda $\mathcal{O}(h^3)$, globalna napaka pa reda $\mathcal{O}(h^2)$ — RK2 je metoda **drugiega reda**.



## 2. Runge-Kutta metoda 4. reda (RK4)

Runge-Kutta metoda 4. reda je ena najpogosteje uporabljenih metod za numerično reševanje diferencialnih enačb. V primerjavi z Eulerjevo metodo dosega bistveno večjo natančnost z enako velikostjo koraka $h$, ker upošteva vrednosti funkcije $f$ na več točkah znotraj koraka.

Metoda temelji na uteženi povprečini štirih ocen naklona:

$$k_1 = f(x_n,\, y_n) \cdot h$$

$$k_2 = f\!\left(x_n + \tfrac{h}{2},\, y_n + \tfrac{k_1}{2}\right) \cdot h$$

$$k_3 = f\!\left(x_n + \tfrac{h}{2},\, y_n + \tfrac{k_2}{2}\right) \cdot h$$

$$k_4 = f\!\left(x_n + h,\, y_n + k_3\right) \cdot h$$

Naslednja vrednost rešitve se izračuna kot:

$$\boxed{y_{n+1} = y_n + \frac{k_1 + 2k_2 + 2k_3 + k_4}{6}}$$

Lokalna napaka metode je reda $\mathcal{O}(h^5)$, globalna napaka pa reda $\mathcal{O}(h^4)$ — RK4 je metoda **četrtega reda**. To pomeni, da je pri enakem koraku $h$ bistveno natančnejša od Eulerjeve metode.

Animacija izračuna štirih naklonov $k_1$–$k_4$ za en korak:

![RK4Metoda](./media/RK4Metoda_ManimCE_v0.20.1.gif)

Primerjava obeh metod z eksaktno rešitvijo:

![Primerjava](./media/Primerjava_ManimCE_v0.20.1.gif)
---


# Primer

Uporabite metodi **Euler** in **RK4** ter rešite spodaj navedeno enačbo. Uporabite integracijski korak $h = 0.1$ v intervalu $0 \le x \le 1$. Primerjajte natančnost metod. Pri numeričnem reševanju uporabi začetni pogoj $y(0) = 1$.

$$
dy/dx = \frac{5x^2 - y}{e^{x+y}}
$$

---

## Naloge

### Naloga 3.1: Implementacija metod

Ustvari datoteko `solve_ode.h`, ki vsebuje naslednje funkcije:

- `ode_Euler_solver(x, y, h)` — en korak Eulerjeve metode za splošno enačbo $y' = f(x, y)$
- `ode_RK4_solver(x, y, h)` — en korak RK4 metode za splošno enačbo $y' = f(x, y)$
- `a(v)` — pospešek krogle pri hitrostni pri prostem padu z uporom
- `newton_euler_dynamics(v, dt)` — en korak Eulerjeve metode za prosti pad
- `newton_RK4_dynamics(v, dt)` — en korak RK4 metode za prosti pad

### Naloga 3.2: Prosti pad z zračnim uporom

Simuliraj prosti pad krogle za čas $t \in [0, 120]\ \text{s}$ z začetno hitrostjo $v(0) = 0$.

- Uporabi oba numerična algoritma (Euler in RK4) z velikostjo koraka $\Delta t = 0{,}1\ \text{s}$.
- Izračunaj analitično terminalno hitrost $v_T$.
- Rezultate ($t$, $v_\text{Euler}$, $v_\text{RK4}$, $v_T$) sproti zapisuj v datoteko in jih prikaži v grafu.
- Kako dolgo traja, da hitrost doseže $99\,\%$ terminalne hitrosti?

### Naloga 3.3: Vpliv velikosti koraka

Ponovi simulacijo z različnimi vrednostmi $\Delta t \in \{1{,}0,\ 0{,}1,\ 0{,}01\}\ \text{s}$.

- Primerjaj napako Eulerjeve in RK4 metode glede na analitično rešitev $v(t) = v_T \tanh(g t / v_T)$.
- Kako se napaka spreminja z zmanjševanjem koraka $\Delta t$?
- Pri kateri vrednosti $\Delta t$ Eulerjeva metoda postane nestabilna?

### Naloga 3.4: Splošna diferencialna enačba

Za diferencialno enačbo $y' = f(x, y) = \frac{5x^2 - y}{e^{x+y}}$ z začetnim pogojem $y(0) = 1$:

- Reši enačbo na intervalu $x \in [0, 2]$ z obema metodama in korakom $h = 0{,}1$.
- Primerjaj rešitvi med seboj in analiziraj razliko.

----

# Domača naloga

## Fizikalni primer: Prosti pad z zračnim uporom

Obravnavamo padanje krogle skozi zrak. Na kroglo delujeta dve sili:
- **Težnost:** $F_g = m g$ (navzdol)
- **Zračni upor:** $F_u = \frac{1}{2} \rho_{\text{atm}}\, c\, S\, v^2$ (navzgor, nasprotno gibanju)

Newtonov drugi zakon da diferencialno enačbo za hitrost $v(t)$:

$$m \frac{dv}{dt} = m g - \frac{1}{2} \rho_{\text{atm}}\, c\, S\, v^2$$

oziroma:

$$\frac{dv}{dt} = g - \frac{\rho_{\text{atm}}\, c\, S}{2 m}\, v^2 = a(v)$$

### Analitična rešitev

Za to enačbo obstaja analitična rešitev. Definiramo terminalno hitrost:

$$v_T = \sqrt{\frac{2\, m\, g}{\rho_{\text{atm}}\, c\, S}}$$

Analitična rešitev (z začetnim pogojem $v(0) = 0$) je:

$$v(t) = v_T \tanh\!\left(\frac{g}{v_T}\, t\right)$$

To nam omogoča neposredno primerjavo numeričnih metod z eksaktno rešitvijo.

### Parametri krogle

| Veličina | Vrednost |
|---|---|
| Polmer krogle $r$ | $0{,}05\ \text{m}$ |
| Gostota krogle $\rho_z$ | $10{,}0\ \text{kg/m}^3$ |
| Gostota zraka $\rho_\text{atm}$ | $1{,}29\ \text{kg/m}^3$ |
| Koeficient upora $c$ | $1{,}0$ |
| Gravitacijski pospešek $g$ | $10{,}0\ \text{m/s}^2$ |
