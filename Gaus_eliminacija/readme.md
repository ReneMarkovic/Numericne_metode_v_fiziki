# Gaussova eliminacija

Gaussova eliminacija je numerična metoda za reševanje sistema linearnih enačb. Sestoji iz postopnega spreminjanja matrike sistema v stopljeno vrstično obliko (tj. zgornje trikotne matrike), kjer se nato lahko rešitev sistema enačb enostavno izračuna s postopkom povratne substitucije.

Gaussova eliminacija je široko uporabljena v matematiki, fiziki, inženirstvu, računalništvu in drugih znanstvenih in tehničnih panogah.

## Predstavitev kode

Ta projekt vsebuje implementacijo Gaussove eliminacije v programskem jeziku C++. Vključuje tri datoteke:

- `gauss_eliminacija.h` - ta datoteka vsebuje prototipe funkcij, ki so uporabljene pri implementaciji Gaussove eliminacije.
- `gauss_eliminacija.cpp` - ta datoteka vsebuje implementacijo funkcij, ki so definirane v `gauss_eliminacija.h`.
- `main.cpp` - ta datoteka vsebuje primer uporabe funkcij v `gauss_eliminacija.h`.

### Funkcije

- `void vnesi_matriko_in_vektor(vector<vector<double>>& A)` - Funkcija, ki uporabniku omogoča vnos matrike A in vektorja b.
- `void izpisi_matriko_na_terminal(vector<vector<double>> A)` - Funkcija, ki izpiše matriko A v terminal.
- `vector<double> Gauss_eliminacija(vector<vector<double>> A)` - Funkcija, ki izvede Gaussovo eliminacijo na matriki A in vrne vektor rešitev.
- `void izpis_rešitev(vector <double> R)` - Funkcija, ki izpiše vektor rešitev.
- `void shrani_matriko_v_datoteko(const vector<vector<double>>& A)` - Funkcija, ki shrani matriko A v datoteko z imenom, ki ga vnese uporabnik preko standardnega vhoda.
- `vector<vector<double>> preberi_matriko_iz_datoteke(const string& filename)` - Funkcija, ki prebere matriko, ki je shranjena v datoteki z imenom `filename`.
- `int main()` - Glavna funkcija, ki izvede primer uporabe funkcij v `gauss_eliminacija.h`.

### Uporaba

Kako uporabiti program za Linux, OS X in Windows:

#### Linux

Za uporabo programa na Linuxu moramo najprej prenesti datoteke projekta iz Github repozitorija. Lahko uporabimo ukaz `git clone`:

```sh
git clone https://github.com/your-username/your-repository.git
```

Nato se premaknemo v mapo s projektom:

```sh
cd your-repository
```

Za zagon programa lahko uporabimo ukaz:

```sh
./gauss
```

#### OS X

Za uporabo programa na OS X moramo najprej prenesti datoteke projekta iz Github repozitorija. Lahko uporabimo ukaz `git clone`:

```sh
git clone https://github.com/your-username/your-repository.git
```

Nato se premaknemo v mapo s projektom:

```sh
cd your-repository
```

Za zagon programa lahko uporabimo ukaz:

```sh
./gauss
```

Če imamo težave s zagonom programa, lahko poskusimo odpreti terminal in se premaknemo v mapo s projektom, nato pa uporabimo ukaz:

```sh
chmod +x gauss
```

Ta ukaz bo omogočil izvajanje programa.

#### Windows

Za uporabo programa na Windowsu moramo najprej prenesti datoteke projekta iz Github repozitorija. Lahko uporabimo ukaz `git clone`:

```sh
git clone https://github.com/your-username/your-repository.git
```

Nato se premaknemo v mapo s projektom:

```sh
cd your-repository
```

Za zagon programa lahko odpremo CMD in se premaknemo v mapo s projektom, nato pa uporabimo ukaz:

```sh
gauss.exe
```

Ali pa dvokliknemo na datoteko `gauss.exe`, ki se nahaja v mapi s projektom.

Če imamo težave s zagonom programa, lahko poskusimo z desnim klikom na datoteko `gauss.exe` in izberemo možnost "Properties". V oknu Properties izberemo zavihek "Compatibility" in nato označimo polje "Run this program as an administrator" ter shranimo spremembe.

## Povzetek

Uporaba programa na različnih operacijskih sistemih se nekoliko razlikuje, vendar je na splošno precej enostavna. Z uporabo git repozitorija lahko prenesemo datoteke projekta, nato pa lahko uporabimo ustrezne ukaze za zagon programa.
