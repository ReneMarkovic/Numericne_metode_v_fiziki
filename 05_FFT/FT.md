# Študijsko gradivo: Frekvencna analiza signala

---

## 1. Fourierova transformacija

### 1.1 Motivacija — Fourierova vrsta

Izhajamo iz postavke, ki jo je Joseph Fourier formaliziral leta 1822: **vsak periodičen signal s periodo $T$ je mogoče natančno zapisati kot neskončno vsoto sinusov in kosinusov** (oz. kompleksnih eksponentov):

$$f(t) = \frac{a_0}{2} + \sum_{n=1}^{\infty} \left[ a_n \cos\left(\frac{2\pi n}{T}t\right) + b_n \sin\left(\frac{2\pi n}{T}t\right) \right]$$

kjer so Fourierovi koeficienti:

$$a_n = \frac{2}{T}\int_0^T f(t)\cos\!\left(\frac{2\pi n}{T}t\right)dt, \qquad b_n = \frac{2}{T}\int_0^T f(t)\sin\!\left(\frac{2\pi n}{T}t\right)dt$$

S kompleksnim zapisom (Eulerjeva formula: $e^{i\theta} = \cos\theta + i\sin\theta$) to postane elegantnejše:

$$f(t) = \sum_{n=-\infty}^{\infty} c_n\, e^{i \frac{2\pi n}{T} t}, \qquad c_n = \frac{1}{T}\int_0^T f(t)\, e^{-i \frac{2\pi n}{T} t}\, dt$$

Fourierova vrsta velja za **periodične** signale. Kaj pa signal, ki ni periodičen?

---

### 1.2 Prehod na Fourierovo transformacijo

Če pustimo, da $T \to \infty$ (signal "razpotegnemo" čez celotno časovno os), postanejo diskretne frekvence $\frac{n}{T}$ zvezne, vsota pa preide v integral. Dobimo **Fourierovo transformacijo**:

$$\boxed{F(\omega) = \int_{-\infty}^{\infty} f(t)\, e^{-i\omega t}\, dt}$$

kjer je $\omega = 2\pi f$ kotna frekvenca [rad/s]. $F(\omega)$ je **frekvenčna reprezentacija** signala — pove nam, koliko vsake frekvence $\omega$ je vsebovane v $f(t)$.

> $F(\omega)$ je načeloma **kompleksno število**: $F(\omega) = |F(\omega)|\, e^{i\phi(\omega)}$, kjer je $|F(\omega)|$ amplitudni spekter in $\phi(\omega)$ fazni spekter.

---

### 1.3 Inverzna Fourierova transformacija

Transformacija je obrnljiva. Iz frekvenčne domene se vrnemo nazaj v časovno:

$$\boxed{f(t) = \frac{1}{2\pi}\int_{-\infty}^{\infty} F(\omega)\, e^{+i\omega t}\, d\omega}$$

Par transformacij $(f(t),\, F(\omega))$ tvori **Fourierjevo par**. Vsak signal ima enolično frekvenčno reprezentacijo in obratno.

> **Konvencija**: V fiziki se pogosto piše $F(f)$ namesto $F(\omega)$ in integral brez $\frac{1}{2\pi}$, kjer je faktor absorbiran v definiciji transformacije. V tej vaji sledimo konvenciji z $\omega$.

---

### 1.4 Ključne lastnosti

| Lastnost | Časovna domena | Frekvenčna domena |
|---|---|---|
| Linearnost | $\alpha f(t) + \beta g(t)$ | $\alpha F(\omega) + \beta G(\omega)$ |
| Časovni premik | $f(t - t_0)$ | $e^{-i\omega t_0} F(\omega)$ |
| Frekvenčni premik | $e^{i\omega_0 t} f(t)$ | $F(\omega - \omega_0)$ |
| Skaliranje | $f(at)$ | $\frac{1}{|a|}F\!\left(\frac{\omega}{a}\right)$ |
| Odvod | $\frac{d}{dt}f(t)$ | $i\omega\, F(\omega)$ |
| **Konvolucija** | $(f * g)(t)$ | $F(\omega)\cdot G(\omega)$ |

**Konvolucijski teorem** je posebej pomemben: konvolucija v časovni domeni (računsko draga) postane množenje v frekvenčni domeni. To je osnova za učinkovito filtriranje signalov.

---

### 1.5 Fizikalna interpretacija

Za realni signal $f(t) \in \mathbb{R}$ velja: $F(-\omega) = F^*(\omega)$ (Hermitska simetrija), torej spekter za negativne frekvence ne vsebuje novih informacij.

**Parsevalov izrek** — energija se ohrani med domenama:

$$\int_{-\infty}^{\infty} |f(t)|^2\, dt = \frac{1}{2\pi}\int_{-\infty}^{\infty} |F(\omega)|^2\, d\omega$$

---

## 2. Diskretna Fourierova transformacija (DFT)

### 2.1 Od zveznega k diskretnemu

V praksi signal poznamo le v **diskretnih časovnih točkah** — vzorčimo ga s frekvenco $f_s$ (v kodi `FS = 10.0 Hz`). Vzorčenje pomeni, da izmerimo vrednost signala vsakih $\Delta t = 1/f_s$ sekund:

$$x[j] = f(j \cdot \Delta t), \qquad j = 0, 1, 2, \ldots, N-1$$

V kodi (`Freq_ana.h`, funkcija `f()`):
```c
for (int i = 0; i < N; i++) {
    casovna_vrsta[i] = x0 * (sin(omega1 * t) + 0.25 * sin(omega2 * t));
    t += DT;   // DT = 1/FS = 0.1 s
}
```

#### Nyquistov teorem

Vzorčenje je veljavno le, če signal ne vsebuje frekvenc višjih od **Nyquistove frekvence**:

$$f_{Nyquist} = \frac{f_s}{2}$$

Če signal vsebuje frekvence nad $f_{Nyquist}$, pride do **aliasinga** — visokofrekvenčne komponente se "prepišejo" v nižje frekvence in jih ni mogoče ločiti od pravih nizko-frekvenčnih komponent. V naši vaji: $f_s = 10\,\text{Hz}$, $f_{Nyquist} = 5\,\text{Hz}$, signal ima komponenti pri $0.1\,\text{Hz}$ in $0.25\,\text{Hz}$ — pogoj je izpolnjen.

---

### 2.2 Formula DFT

Nadomestimo integral z vsoto in dobimo **DFT** za $N$ vzorcev:

$$\boxed{X[k] = \sum_{j=0}^{N-1} x[j]\, e^{-i \frac{2\pi}{N} j k}, \qquad k = 0, 1, \ldots, N-1}$$

Z Eulerjevo formulo razpišemo na realni in imaginarni del:

$$X[k] = \underbrace{\sum_{j=0}^{N-1} x[j]\cos\!\left(\frac{2\pi jk}{N}\right)}_{\text{Re}(X[k])} - i\underbrace{\sum_{j=0}^{N-1} x[j]\sin\!\left(\frac{2\pi jk}{N}\right)}_{\text{Im}(X[k])}$$

To je neposredno implementirano v `DFT()` (`Freq_ana.h`):
```c
double omega_n = 2.0 * PI / N;
for (k = 0; k < N; k++) {
    double real = 0.0, imag = 0.0;
    for (j = 0; j < N; j++) {
        real += cos(omega_n * j * k) * casovna_vrsta[j];
        imag -= sin(omega_n * j * k) * casovna_vrsta[j];
    }
    matrix[0][k] = real;
    matrix[1][k] = imag;
}
```

---

### 2.3 Interpretacija koeficientov

**Kateri frekvenci ustreza $k$-ti koeficient?**

$$f_k = \frac{k}{N \cdot \Delta t} = \frac{k \cdot f_s}{N}$$

Frekvenčna ločljivost (najmanjša razlika, ki jo DFT razloči):

$$\Delta f = \frac{f_s}{N} = \frac{1}{N \cdot \Delta t} = \frac{1}{T_{total}}$$

Večji signal (daljši čas merjenja) → boljša frekvenčna ločljivost.

**Amplitudni spekter** (spekter moči):

$$|X[k]| = \sqrt{\text{Re}(X[k])^2 + \text{Im}(X[k])^2}$$

V kodi (`Freq_ana.h`):
```c
p = sqrt(matrix[0][i]*matrix[0][i] + matrix[1][i]*matrix[1][i]);
fprintf(data, "%d\t%f\t%f\t%f\t%f\n", i, i/(N*DT), matrix[0][i], matrix[1][i], p);
```

**Simetrija**: Za realne signale je $X[N-k] = X[k]^*$, torej koeficienti za $k > N/2$ ne prinašajo novih informacij. Zapisujemo samo prvih $N/2$ koeficientov.

---

### 2.4 Inverzna DFT

$$x[j] = \frac{1}{N}\sum_{k=0}^{N-1} X[k]\, e^{+i \frac{2\pi}{N} j k}$$

Razlika od direktne DFT: predznak eksponenta je $+i$ in normalizacija z $\frac{1}{N}$.

---

### 2.5 Računska zahtevnost DFT

Dvojna zanka: za vsak od $N$ koeficientov $k$ izvajamo $N$ množenj in seštevanj → skupaj $N^2$ operacij:

$$\text{DFT}: \quad \mathcal{O}(N^2)$$

Iz meritev (`readme.md`):

| $N$ | Čas DFT | Razmerje $t(2N)/t(N)$ |
|---|---|---|
| 256 | 0.045 s | — |
| 512 | 0.099 s | 2.2× |
| 1024 | 0.161 s | 1.6× |
| 2048 | 0.692 s | 4.3× |
| 8192 | 8.18 s | 3.8× |
| 16384 | 29.1 s | 3.6× |

Pričakujemo razmerje $\approx 4\times$ ob podvojitvi $N$ (ker $(2N)^2 = 4N^2$) — to vidimo v meritvah.

---

## 3. Hitra Fourierova transformacija (FFT)

### 3.1 Osnovna ideja — Cooley-Tukey algoritem

Cooley in Tukey sta leta 1965 opazila, da se DFT $N$-točkovnega signala da rekurzivno razdeliti na dve DFT po $N/2$ točkah. Ločimo **sode** ($j = 0, 2, 4, \ldots$) in **lihe** ($j = 1, 3, 5, \ldots$) vzorce:

$$X[k] = \sum_{j=0}^{N/2-1} x[2j]\, e^{-i\frac{2\pi}{N}(2j)k} + \sum_{j=0}^{N/2-1} x[2j+1]\, e^{-i\frac{2\pi}{N}(2j+1)k}$$

$$= \underbrace{\sum_{j=0}^{N/2-1} x[2j]\, e^{-i\frac{2\pi}{N/2}jk}}_{E[k]} + e^{-i\frac{2\pi}{N}k} \underbrace{\sum_{j=0}^{N/2-1} x[2j+1]\, e^{-i\frac{2\pi}{N/2}jk}}_{O[k]}$$

$$\boxed{X[k] = E[k] + W_N^k \cdot O[k]}$$

kjer je $W_N^k = e^{-i\frac{2\pi}{N}k}$ **twiddle factor**. Ker sta $E[k]$ in $O[k]$ periodični s periodo $N/2$:

$$X[k] = E[k] + W_N^k \cdot O[k]$$
$$X[k + N/2] = E[k] - W_N^k \cdot O[k]$$

Dve DFT dolžine $N/2$ sta cenejši od ene DFT dolžine $N$: namesto $N^2$ operacij potrebujemo $2\cdot(N/2)^2 = N^2/2$ — prepolovili smo delo. Rekurzivno to ponovimo do $N/2 = 1$ (posamezen vzorec) in dobimo:

$$\mathcal{O}(N^2) \longrightarrow \mathcal{O}(N \log_2 N)$$

---

### 3.2 Bit-reversal permutacija

Rekurzivno ločevanje sodih in lihih elementov premesti vzorce v specifičen vrstni red. Za $N = 8$:

```
Korak 0:  [x₀, x₁, x₂, x₃, x₄, x₅, x₆, x₇]   ← originalni vrstni red
Korak 1:  [x₀, x₂, x₄, x₆, x₁, x₃, x₅, x₇]   ← sodi | lihi
Korak 2:  [x₀, x₄, x₂, x₆, x₁, x₅, x₃, x₇]   ← sodi sodih | ...
```

Vzorec preureditve: indeksi v binarni obliki so **obrnjeni** (bit-reversed):

```
Indeks  Binarno   Obrnjeno   Bit-rev. indeks
  0      000        000          0
  1      001        100          4
  2      010        010          2
  3      011        110          6
  4      100        001          1
  5      101        101          5
  6      110        011          3
  7      111        111          7
```

V kodi (`Freq_ana.h`) to izvedemo **brez eksplicitnega računanja binarnih obratov** s pametnim algoritmom:

```c
j = 0;  n2 = n/2;
for (i = 1; i < n-1; i++) {
    n1 = n2;
    while (j >= n1) { j -= n1;  n1 /= 2; }
    j += n1;
    if (i < j)  SWAP(casovna_vrsta[i], casovna_vrsta[j]);
}
```

Spremenljivka `j` je bit-reversed kopija `i`. Algoritem jo inkrementira brez binarnih operacij z izkoriščanjem strukture bit-reversal zaporedja. Zamenjamo le pare `(i, j)` kjer `i < j`, da se vsak par zamenja natanko enkrat.

---

### 3.3 Butterfly operacija — jedro FFT

Po prerazporeditvi izvajamo $m = \log_2 N$ **stopenj butterfly operacij**. Vsaka stopnja združuje rezultate prejšnje:

```
Stopnja 1: butterfly dolžine 2  (N/2 butterfly-jev)
Stopnja 2: butterfly dolžine 4  (N/2 butterfly-jev)
Stopnja 3: butterfly dolžine 8  (N/2 butterfly-jev)
    ⋮
Stopnja m: butterfly dolžine N  (N/2 butterfly-jev)
```

Ena butterfly operacija (za par $(k,\, k+n_1)$):

```
        ┌─────────────────────────────→  u + t·W    [k]
u ──────┤
        └──── × W ──→ t ──(−)─────────→  u - t·W   [k+n₁]
```

V kodi (`Freq_ana.h`):

```c
for (i = 0; i < m; i++) {          // m stopenj
    n1 = 1 << i;                   // n1 = 2^i  (velikost pol-butterfly)
    n2 = n1 << 1;                  // n2 = 2^(i+1) (velikost butterfly)
    for (j = 0; j < n1; j++) {     // j = indeks twiddle faktorja
        c = cos(-2*PI*j / n2);     // Re(W) = cos(-2π·j/n2)
        s = sin(-2*PI*j / n2);     // Im(W) = sin(-2π·j/n2)
        for (k = j; k < n; k += n2) {
            k1 = k + n1;
            t = complex_množenje(izhod[k1], complex_ustvari(c, s));  // t = dolnja × W
            u = izhod[k];                                             // u = zgornja
            izhod[k]  = complex_add(u, t);                           // u + t·W
            izhod[k1] = complex_sub(u, t);                           // u - t·W
        }
    }
}
```

---

### 3.4 Vizualizacija za N = 8

```
Vhod (bit-reversed):
x[0] ─┐
x[4] ─┤ butterfly ─┐
x[2] ─┤ butterfly ─┤ butterfly ─┐
x[6] ─┤ butterfly ─┤ butterfly ─┤
x[1] ─┤ butterfly ─┤ butterfly ─┤ butterfly ──→ X[0..7]
x[5] ─┤ butterfly ─┤ butterfly ─┤
x[3] ─┤ butterfly ─┘ butterfly ─┘
x[7] ─┘
       Stopnja 1    Stopnja 2    Stopnja 3
       (n2=2)       (n2=4)       (n2=8)
```

Skupaj: 3 stopnje × 4 butterfly-ji = 12 operacij, medtem ko bi DFT potreboval $8^2 = 64$.

---

### 3.5 Računska zahtevnost FFT

Na vsaki od $m = \log_2 N$ stopenj izvedemo $N/2$ butterfly operacij, vsaka zahteva eno kompleksno množenje in dve kompleksni seštevanje:

$$\text{FFT}: \quad \mathcal{O}\!\left(\frac{N}{2}\log_2 N\right) \approx \mathcal{O}(N\log_2 N)$$

Iz meritev (`readme.md`):

| $N$ | Čas FFT | Razmerje $t(2N)/t(N)$ |
|---|---|---|
| 512 | 0.001 s | — |
| 1024 | 0.001 s | 1.00 |
| 2048 | 0.002 s | 2.00 |
| 4096 | 0.003 s | 1.50 |
| 8192 | 0.006 s | 2.00 |
| 16384 | 0.012 s | 2.00 |
| 32768 | 0.031 s | 2.58 |

Pričakujemo razmerje $\approx 2\times$ ob podvojitvi (ker $2N\log_2(2N) = 2N(\log_2 N + 1) \approx 2N\log_2 N$) — to vidimo v meritvah (stolpec `rel_delta_FFT ≈ 2`).

---

### 3.6 Primerjava DFT in FFT

$$\frac{t_{DFT}}{t_{FFT}} \approx \frac{N^2}{N\log_2 N} = \frac{N}{\log_2 N}$$

| $N$ | $N / \log_2 N$ | Izmerjeno razmerje |
|---|---|---|
| 512 | 57 | 99 |
| 1024 | 102 | 161 |
| 4096 | 341 | 710 |
| 16384 | 1170 | 2428 |
| 32768 | 2185 | 3878 |

Izmerjene vrednosti so višje od teorije, ker FFT poleg asimptotske zahtevnosti pridobi tudi s **predpomnjenjem (cache)** — linearni dostop do pomnilnika je hitrejši kot naključni pri DFT.

---

## 4. Povzetek — od formule do kode

| Matematika | Koda |
|---|---|
| $x(t) = \sin(\omega_1 t) + 0.25\sin(\omega_2 t)$ | `f()` v `Freq_ana.h` |
| $X[k] = \sum_j x[j]\,e^{-i\frac{2\pi}{N}jk}$ | `DFT()` v `Freq_ana.h` |
| bit-reversal permutacija | `FFT()` — prvi del |
| butterfly: $X[k] = E[k] + W\cdot O[k]$ | `FFT()` — drugi del |
| $f_k = k/(N\Delta t)$ | `main.cpp`, izpis v `FFT.txt` |
| $\|X[k]\| = \sqrt{Re^2 + Im^2}$ | `DFT_power_spectrum.txt` izpis |
| $\mathcal{O}(N^2)$ vs $\mathcal{O}(N\log N)$ | `clock()` meritve v `main.cpp` |
