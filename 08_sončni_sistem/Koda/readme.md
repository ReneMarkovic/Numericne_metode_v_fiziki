
# Simulacija gibanja nebesnih teles

Program `Planeti.cpp` omogoča izvajanje simulacije gibanja nebesnih teles v Sončevem sistemu. Trenutno vključuje simulacijo za Sonce, Zemljo in Luno, pri čemer se izračunajo in shranijo njihove lege v odvisnosti od časa. Program je tudi prilagodljiv za nadgradnjo s dodajanjem drugih objektov v simulacijo.

# Dokumentacija programske kode

Ta dokumentacija opisuje program za simulacijo gibanja teles v Sončevem sistemu. Program je napisan v jeziku C++ in uporablja knjižnico `solar_system.h`, ki vsebuje oblikovane funkcije za potrebe simulacije.

## Opis programa

Program simulira gibanje teles v Sončevem sistemu s pomočjo integracijske metode. Uporablja se metodo posodabljanja položaja in hitrosti glede na sile, ki delujejo na posamezna telesa.

# Vključene knjižnice

Program vključuje naslednje standardne C++ knjižnice:

<stdio.h> - zaglavje za standardni vnos/izhod
<stdlib.h> - zaglavje za upravljanje s spominom
<cmath> - zaglavje za matematične funkcije
<cstdlib> - zaglavje za splošne funkcije C/C++
<iostream> - zaglavje za vhodno/izhodno tokovno knjižnico
<vector> - zaglavje za uporabo vektorjev

# Konstante sistema

V programu so definirane naslednje konstante za mase teles v Sončevem sistemu:

MSonce - masa Sonca [kg]
Mz - masa Zemlje [kg]
Mv - masa Venere [kg]
Mr - masa Merkurja [kg]
Ml - masa Lune [kg]
AU - astronomska enota [m]

Dodane so tudi konstante za druge astronomske objekte, ki jih lahko uporabite v nadaljnjih razširitvah programa.

# Integracijski koraki

V programu so definirane naslednje spremenljivke za integracijo:

- $dt$ - integracijski korak v sekundah
- $leto$ - čas trajanja simulacije v letih (na Zemlji)
- $N$ - število integracijskih korakov
- $dn$ - časovno okno za posodobitev izpisa na zaslon

# Glavna funkcija - main()

Funkcija main() je glavna funkcija programa. V njej se izvaja simulacija gibanja teles v Sončevem sistemu. V funkciji main() so ustvarjeni objekti `Sonce`, `Zemlja` in `Luna` s pomočjo funkcije `nastavi_atribute()` iz knjižnice `solar_system.h`.

## Seznam objektov

Ustvarjen je seznam objektov list, ki vsebuje kazalce na objekte v simulaciji.

## Glavna *for* zanka

V glavni zanki programa se izvaja simulacija gibanja teles. V vsakem koraku se izračunajo sile, ki delujejo na vsako telo, nato pa se posodobijo pospeški, položaji in hitrosti teles. Rezultati se zapisujejo v datoteko **"rezultati.txt"** in na zaslonu se izpiše napredek simulacije.

# Izračun sil

Za vsako telo se izračunajo sile, ki delujejo nanj, s pomočjo funkcije `doloci_sile()` iz knjižnice solar_system.h. Sile se izračunajo na podlagi mase objektov in razdalje med njimi.

# Posodobitev vektorjev

Po izračunu sil se posodobijo pospeški, položaji in hitrosti teles s pomočjo funkcije `posodobi_vektorje()` iz knjižnice solar_system.h. Pospeški se nastavijo na vrednosti, izračunane iz sil.

# Zapis rezultatov

Rezultati simulacije, to je položaji teles, se zapišejo v datoteko "rezultati.txt" s pomočjo funkcije fprintf(). Za vsako telo se zapišejo x- in y-koordinata položaja, ločene z tabulatorjem. Zapis se izvaja na vsakih dn korakov simulacije.

# Prikaz napredka

Na zaslonu se izpiše napredek simulacije v odstotkih, ki se izračuna glede na število opravljenih korakov. Izpis se osvežuje na vsakih dn korakov simulacije.

# Zaključek

Program omogoča simulacijo gibanja teles v Sončevem sistemu in zapis rezultatov v datoteko. S pomočjo knjižnice solar_system.h so implementirane funkcije za izračun sil in posodobitev vektorjev. Program je prilagodljiv za nadaljnje razširitve in dodajanje novih teles v simulacijo.