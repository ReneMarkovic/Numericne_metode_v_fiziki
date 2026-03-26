# Gaussova integracija

Gaussova integracija je numerična metoda, ki se uporablja za izračunavanje deločenih integralov. V splošnem velja, da ta metoda mnogokrat natančnejša od drugih metod, kot so trapezna pravila ali Simpsonova pravila, še posebej pri integraciji funkcij, ki so gladke ali imajo visoko stopnjo polinoma. Danes, je ta metoda ključna v številnih aplikacijah, ki se uporabljajo v znanosti in inženirstvu, kot so fizika, kemija, ekonomija in računalništvo in temeljijo na metodi končnih elementov (ang. finite element method ali FEM).

## Osnovna ideja

Glavna ideja Gaussove integracije je, da se integral funkcije, $f(x)$, približa z uporabo tehtanih vsot vrednosti funkcije v določenih točkah, imenovanih Gaussove točke. Te točke in njihove ustrezne uteži so izbrane tako, da zagotavljajo najboljšo možno natančnost za določeno število točk. S pomočjo Gaussove integracije skušamo določiti najmanjše število točk, ki nam omogočajo natančno oceno integrala. Obstaja več vrst Gaussove integracije, kot so Gauss-Legendre, Gauss-Chebyshev, Gauss-Hermite in Gauss-Laguerre, ki se uporabljajo za različne vrste funkcij in intervalov. Ključna razlika je, da se funkcija $f(x)$ oceni v različnih intervalih in z različnimi utežmi, odvisno od vrste Gaussove integracije, ki jo uporabljamo.

### Newton–Cotes metoda

Spomnimo se, da lahko integral funkcije $f(x)$ na intervalu $[a,b]$ približamo z uporabo Newton–Cotesove metode, ki vključuje deljenje intervala na **enake dele** in uporabo polinomov za oceno funkcije. Na primer, trapezna pravila in Simpsonova pravila so posebni primeri Newton–Cotesove metode. Vendar pa te metode pogosto zahtevajo več točk za dosego želene natančnosti, še posebej pri funkcijah, ki niso gladke ali imajo visoke stopnje polinoma.

Spomnimo se, da lahko torej funkcijo $f(x)$ zapišemo kot polinom s pomočjo Lagrangeovih interpolacijskih polinomov, ki so definirani na enakih delih intervala. Funkcijo $f(x)$ tako lahko zapišemo kot interpolacijsko funkcijo $L(x)$, tako da postane:

$$
L(x) = \sum_{i=1}^n f(x_i) \cdot L_i(x),
$$

kjer so $x_i$ točke deljenja intervala, $f(x_i)$ so vrednosti funkcije v teh točkah, in $L_i(x)$ so Lagrangeovi interpolacijski polinomi, ki so definirani kot:

$$
L_i(x) = \prod_{j=1, j\neq i}^n \frac{x - x_j}{x_i - x_j}
$$

Sedaj lahko integral funkcije $f(x)$ na intervalu $[a,b]$ približamo z integracijo interpolacijske funkcije $L(x)$:

$$
\int_a^b f(x) \cdot dx \approx \int_a^b L(x) \cdot dx = \sum_{i=1}^n f(x_i) \cdot \int_a^b L_i(x) \cdot dx
$$

Tukaj predstavlja člen $\int_a^b L_i(x) \cdot dx$ utež $w_i$, ki je odvisna od izbranih točk $x_i$. Gaussova integracija se osredotoča na izbiro optimalnih točk $x_i$ in ustreznih uteži $w_i$, da bi dosegli najboljšo možno natančnost z minimalnim številom točk.


### n = 1

Pri $n=1$ imamo na voljo dve prosti spremenljivki: eno vozlišče $x_1$ in eno utež $w_1$. Želimo, da je metoda točna za čim višjo stopnjo polinoma $f(x) = a_0 + a_1x + a_2x^2 + \dots$

**Nastavek problema:**
$$\int_{-1}^1 f(x) \cdot dx = w_1 \cdot f(x_1)$$

- za $f(x) = 1 \to \int_{-1}^1 1 \cdot dx = 2 \Rightarrow w_1 = 2$
- za $f(x) = x \to \int_{-1}^1 x \cdot dx = 0 \Rightarrow w_1 \cdot x_1 = 0 \Rightarrow x_1 = 0$

### n = 2

Tukaj postane zanimivo. Imamo 4 neznanke: $x_1, x_2, w_1, w_2$. Metoda bo točna za polinome do stopnje $2(2)-1 = 3$.

**Nastavek problema:**
$$\int_{-1}^1 f(x) \cdot dx = w_1 \cdot f(x_1) + w_2 \cdot f(x_2)$$

- za $f(x) = 1 \to \int_{-1}^1 1 \cdot dx = 2 \Rightarrow w_1 + w_2 = 2$
- za $f(x) = x \to \int_{-1}^1 x \cdot dx = 0 \Rightarrow w_1 \cdot x_1 + w_2 \cdot x_2 = 0$
- za $f(x) = x^2 \to \int_{-1}^1 x^2 \cdot dx = \frac{2}{3} \Rightarrow w_1 \cdot x_1^2 + w_2 \cdot x_2^2 = \frac{2}{3}$
- za $f(x) = x^3 \to \int_{-1}^1 x^3 \cdot dx = 0 \Rightarrow w_1 \cdot x_1^3 + w_2 \cdot x_2^3 = 0$

To lahko zapišemo v matrično obliko:
$$
\begin{bmatrix}1 & 1 \\
x_1 & x_2 \\
x_1^2 & x_2^2 \\
x_1^3 & x_2^3
\end{bmatrix}
\begin{bmatrix}w_1 \\ w_2 \end{bmatrix}
=
\begin{bmatrix}2 \\ 0 \\ \frac{2}{3} \\ 0 \end{bmatrix}
$$

Rezultat je $x_1 = -\frac{1}{\sqrt{3}}, x_2 = \frac{1}{\sqrt{3}}, w_1 = w_2 = 1$. To pomeni, da je metoda točna za polinome do stopnje 3, kar je impresivno glede na to, da uporabljamo le 2 točki.



## Kako brati tabelo v fizikalnem kontekstu?

1. **Analiza intervala:** Prvi korak fizika je pogled na meje integracije. Če integriramo čez celotno premico (npr. verjetnostne gostote v kvantni mehaniki), tabela takoj usmeri našo pozornost na **Gauss-Hermiteovo metodo**. Če je interval polneskončen (npr. čas od $0$ do $\infty$), izberemo **Gauss-Laguerreovo** metodo.

2. **Utežna funkcija kot "vgrajen" del fizike:** 
   - Pri **Gauss-Legendru** je utež $1$, kar pomeni, da funkcijo $f(x)$ obravnavamo enakomerno po celem intervalu. To je standard v metodi končnih elementov (FEM).
   - Pri **Gauss-Laguerru** je utež $e^{-x}$ naravno opisuje sisteme, kjer vpliv določenega pojava z razdaljo ali časom eksponentno pada (npr. absorpcija svetlobe v atmosferi).
   - Pri **Gauss-Hermitu** utež $e^{-x^2}$ ustreza Gaussovim porazdelitvam, ki so temelj statistične termodinamike in valovnih paketov.
   - Pri **Gauss-Chebyshevovi** metodi utež $1/\sqrt{1-x^2}$ odraža težave, ki se pojavijo pri integraciji funkcij z robnimi singularnostmi, kot so tiste, ki se pojavljajo v aerodinamiki.


3. **Vloga ortogonalnih polinomov:** Stolpec z ortogonalnimi polinomi pojasnjuje "magijo" za izbiro točk. Vozlišča $x_i$, v katerih moramo izračunati vrednost naše funkcije, so natanko ničle teh polinomov. Ker so ti polinomi med seboj ortogonalni glede na utež $w(x)$, nam zagotavljajo, da z minimalnim številom točk ($n$) natančno zajamemo obnašanje funkcije, kot bi jo sicer opisal polinom stopnje $2n-1$.


| Metoda | Interval | Utežna funkcija $w(x)$ | Ortogonalni polinomi | Tipična uporaba v fiziki |
|--------|----------|-------------------------|---------------------|---------------------------|
| Gauss-Legendre | $[-1,1]$ | $1$ | Legendrovi (Pn​) | Splošni integrali, FEM, prenos toplote. |
| Gauss-Chebyshev | $[-1,1]$ | $1/\sqrt{1-x^2}$ | Čebiševi (Tn​) | Problemi z robno singularnostjo, aerodinamika. |
| Gauss-Hermite | $(-\infty,\infty)$ | $e^{-x^2}$ | Hermitovi (Hn​) | Kvantni harmonični oscilator, statistika. |
| Gauss-Laguerre | $[0,\infty)$ | $e^{-x}$ | Laguerrovi (Ln​) | Atomski integrali, Maxwell-Boltzmannova porazdelitev. |

