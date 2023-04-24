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

Projekt je zgrajen z uporabo orodja `CMake`. Za zgraditev projekta sledite naslednjim korakom:

1. Odprite terminal in se premaknite v koreninsko mapo projekta.
2. Vnesite naslednje ukaze v terminal:

```
mkdir build
cd build
cmake ..
make
```

3. Ko se projekt uspešno zgradi, lahko izvedete `./gauss_elimination` in uporabite program.

Uporaba programa je enost