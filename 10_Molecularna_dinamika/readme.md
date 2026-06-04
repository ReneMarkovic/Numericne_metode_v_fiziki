# Molekularna dinamika (Molecular Dynamics)

Ta mapa vsebuje računalniško simulacijo klasičnega sistema delcev (npr. žlahtnega plina argona) z uporabo molekularne dinamike (MD) v treh dimenzijah. Simulacija temelji na numeričnem integriranju Newtonovih enačb gibanja in računanju parnih interakcij preko Lennard-Jonesovega potenciala. Podoben primerm smo reševali že pri vaji 09_Monte_Carlo, kjer smo uporabili naključna števila in **Metropolisov algoritem**.


## Struktura mape

- **main.cpp:** Glavni simulacijski program v C++. Skrbi za inicializacijo, glavno MD zanko (Velocity Verlet), periodične robne pogoje in zapisovanje fizikalnih količin.
- **utils.h:** Pomožna glava, ki implementira Gaussov generator naključnih števil (gasdev()) z uporabo Box-Mullerjeve transformacije za inicializacijo hitrosti delcev po Maxwell-Boltzmannovi porazdelitvi.
- **analiza.ipynb:** Jupyter Notebook v Pythonu. Avtomatizira prevajanje C++ kode, zagon simulacij pri različnih temperaturah $T$ ter vizualizira rezultate (npr. radialno porazdelitveno funkcijo, nihanje temperature, časovni razvoj pozicij).

# Fizikalno ozadje in model

## 1. Lennard-Jonesov potencial

Interakcija med poljubnima dvema nevtralnima delcema na razdalji $r$ je modelirana z Lennard-Jonesovim $12-6$ potencialom:

$$V(r) = 4\varepsilon \left[ \left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^6 \right]$$

kjer $\varepsilon$ predstavlja globino potencialne jame, $\sigma$ pa je značilni premer delca (kjer je potencial enak 0). V kodi so zaradi računske učinkovitosti uporabljene brezdimenzijske (redukovane) enote, kjer velja $\varepsilon = 1$, $\sigma = 1$ in $m = 1$.Sila med delcema je odvod potenciala po razdalji:

$$\vec{F}(\vec{r}) = -\nabla V(r) = \frac{24\varepsilon}{r^2} \left[ 2\left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^6 \right] \vec{r}$$

## 2. Periodični robni pogoji (PBC) in Minimum Image Convention

Da bi se izognili robnim efektom majhnega števila delcev in simulirali neskončen makroskopski sistem, delce zapremo v simulacijsko škatlo dimenzij $L \times L \times L$. Ko delec zapusti škatlo na eni strani, vstopi nazaj na nasprotni strani. Pri računanju sil se upošteva pravilo najbližje slike (Minimum Image Convention) — delec čuti silo le od tiste slike sosednjega delca, ki mu je najbližje.

## 3. Integracijski algoritem: Velocity Verlet

Za posodabljanje koordinat $\vec{r}$ in hitrosti $\vec{v}$ delcev v časovnem koraku $\Delta t$ koda uporablja simplektični in energijsko stabilni Velocity Verlet algoritem:


$$\vec{r}(t + \Delta t) = \vec{r}(t) + \vec{v}(t)\Delta t + \frac{1}{2}\vec{a}(t)\Delta t^2$$

$$\vec{v}\left(t + \frac{\Delta t}{2}\right) = \vec{v}(t) + \frac{1}{2}\vec{a}(t)\Delta t$$

$$\vec{a}(t + \Delta t) = \frac{1}{m}\vec{F}(\vec{r}(t + \Delta t))$$

$$\vec{v}(t + \Delta t) = \vec{v}\left(t + \frac{\Delta t}{2}\right) + \frac{1}{2}\vec{a}(t + \Delta t)\Delta t$$

## Kako zaženemo simulacijo in analizo

Najlažji način za zagon celotnega projekta je preko priloženega Jupyter Notebooka analiza.ipynb, ki avtomatsko poskrbi za vse korake. Ročni zagon preko terminala:
Kompilacija C++ simulacije: Za doseganje maksimalne hitrosti izvajanja uporabite zastavico -Ofast:

```Bash
g++ -Ofast main.cpp -o run
```

Zagon simulacije z določitvijo začetne temperature $T$:Program sprejme argument za želeno začetno temperaturo termostatiranja (npr. $T = 0.5$):

```Bash
./run 0.5
```

Program bo ustvaril mapo RESULTS/ (če še ne obstaja) in vanjo shranil rezultate časovnega razvoja sistema.

## Rezultati in opazovalne količine

Simulacija med tekom generira naslednje datoteke s podatki v mapi RESULTS/:
- *_T_temperatura.txt: Zapis instantaneous temperature skozi čas. Omogoča spremljanje termalizacije sistema in nihanj okoli ravnovesne vrednosti.
- *_T_gr.txt: Radialna porazdelitvena funkcija $g(r)$. To je ključna strukturna količina, ki pove, kakšna je verjetnost, da najdemo delec na razdalji $r$ od izbranega referenčnega delca v primerjavi z idealnim plinom. Iz oblike $g(r)$ lahko študenti razberejo agregatno stanje sistema (trdno, tekoče ali plinasto):Nizke temperature (npr. $T=0.01$ ali $0.1$): Izraziti, ostri vrhovi, ki nakazujejo urejeno kristalno strukturo.Srednje temperature (npr. $T=1.0$): Prvi visok vrh (najbližji sosedje), sledi mu hitro dušenje proti vrednosti 1 (značilno za tekočine/amorfne snovi).
- *_T_min_dist.txt: Sledenje minimalnim meddelčnim razdaljam za kontrolo numerične stabilnosti (prevelik časovni korak $\Delta t$ vodi v "prekrivanje" delcev in eksplozijo sil).
- *_T_pozicije.txt: 3D koordinate delcev, ki se v Jupyter Notebooku uporabijo za izris začetne in končne prostorske razporeditve s pomočjo 3D raztresenega grafikona (ax.scatter).

## Cilji vaj za študente (Pedagoški poudarki)

Preučevanje agregatnih stanj: S spreminjanjem začetne temperature opazujte prehode med kristalno strukturo (trdnina), neurejeno gosto strukturo (tekočina) in redkim stanjem (plin) s pomočjo funkcije $g(r)$. Vpliv časovnega koraka $\Delta t$: Testirajte stabilnost ohranjanja skupne energije (Velocity Verlet je simplektični integrator) in ugotovite, pri kako velikem koraku $\Delta t$ numerična napaka povzroči fizikalno nestabilnost.Analiza hitrosti (Maxwell-Boltzmann): Preverite, ali se začetne hitrosti, generirane z Gaussovo porazdelitvijo, po določenem času relaksacije res uravnovesijo v stabilno Maxwell-Boltzmannovo porazdelitev za dano temperaturo.
