# Objašnjenje `resenje.c` – linija po liniju

## Include direktive

```c
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
```

- `stdio.h` — standardna biblioteka za rad sa fajlovima i ulazom/izlazom: `FILE*`, `fopen`, `fclose`, `fscanf`, `fprintf`.
- `stdlib.h` — potrebno zbog `exit()` i `atoi()`.
- `"lista.h"` — tvoj sopstveni header (navodnici umesto `<>` jer je lokalni fajl, ne sistemski). Iz koda se vidi da tu mora biti definisano:
  - struktura `CVOR` sa poljima `domacin`, `gost`, `cena_karte`, `sledeci`
  - konstante `MAX_DOMACI` i `MAX_GOST`
  - funkcije `create_node`, `add_to_list`, `init`, `clear`

---

## Funkcija `ucitaj_podatke`

```c
void ucitaj_podatke(FILE* ulaz, CVOR** glava) {
```
- Prima pokazivač na fajl (`ulaz`) i **pokazivač na pokazivač** (`CVOR** glava`). Zašto duplo `*`? Zato što unutra menjamo samu glavu liste (dodajemo nove čvorove), pa moramo da prosledimo adresu pokazivača da bi promena bila vidljiva i posle povratka iz funkcije (poziv po referenci, jer C nema `&` parametre kao C++).

```c
    char domacin[MAX_DOMACI];
    char gost[MAX_GOST];
    int cena_karte;
    CVOR* novi;
```
- Lokalne promenljive: dva string bafera fiksne veličine (definisane u `lista.h`) za ime domaćina i gosta, cena karte kao `int`, i pokazivač na novi čvor koji ćemo kreirati u petlji.

```c
    while(fscanf(ulaz, "%20s %20s %d", domacin, gost, &cena_karte) == 3) {
```
- `fscanf` čita iz fajla po formatu: string, string, broj.
- `%20s` znači "čitaj najviše 20 karaktera" — ovo je **zaštita od prepunjavanja bafera** (buffer overflow). Ako bi neko upisao string duži od 20 karaktera u fajl, bez ovog ograničenja bi se pisalo van granica niza `domacin`/`gost` i izazvalo undefined behavior. Broj 20 treba da odgovara veličini `MAX_DOMACI`/`MAX_GOST` minus 1 (za null-terminator).
- `fscanf` vraća broj **uspešno učitanih** vrednosti. Petlja se izvršava dok god uspe da učita sva 3 polja. Kad dođe do kraja fajla ili loš format, `fscanf` vraća manje od 3 (npr. `EOF` = -1), i petlja se prekida — ovo je standardan idiom za čitanje "dok ima podataka".

```c
        novi = create_node(domacin, gost, cena_karte);
        add_to_list(glava, novi);
```
- Za svaki red iz fajla: pravi se novi čvor sa pročitanim podacima (funkcija iz `lista.h`, verovatno interno radi `malloc` i `strcpy`), pa se taj čvor dodaje u listu. `add_to_list` prima `CVOR**` jer možda menja glavu liste (npr. ako se dodaje na početak, ili ako je lista prazna).

```c
    }
}
```
- Kraj `while` petlje i kraj funkcije. Nema `return` jer je tip `void`.

---

## Funkcija `ispisi_podatke`

```c
void ispisi_podatke(FILE* izlaz, CVOR* glava, int broj_ljudi) {
```
- Ovde je `glava` obično `CVOR*` (ne dupli pokazivač), jer ova funkcija **ne menja** listu, samo je čita/ispisuje.

```c
    CVOR* trenutni = glava;
    int ukupna_cena;
```
- `trenutni` je pomoćni pokazivač kojim ćemo šetati kroz listu, a `glava` ostaje netaknuta (dobra praksa — ne diraj original ako ne moraš).
- `ukupna_cena` — pomoćna promenljiva za izračunatu cenu.

```c
    while(trenutni != NULL) {
        ukupna_cena = trenutni->cena_karte * broj_ljudi;
        fprintf(izlaz, "%-15s %-15s %d$\n", trenutni->domacin, trenutni->gost, ukupna_cena);
        trenutni = trenutni->sledeci;
    }
}
```
- Klasična šetnja kroz povezanu listu: dok pokazivač nije `NULL`, obradi čvor pa pređi na sledeći (`trenutni->sledeci`).
- `ukupna_cena = trenutni->cena_karte * broj_ljudi;` — cena karte po osobi pomnožena brojem ljudi iz komandne linije = ukupna cena za tu utakmicu/događaj.
- `fprintf(izlaz, "%-15s %-15s %d$\n", ...)`:
  - `izlaz` — fajl u koji pišemo (ne `stdout`, već fajl otvoren u `main`-u).
  - `%-15s` — string poravnat **levo** (`-`) u polju širine 15 karaktera (popunjava razmacima do 15 ako je kraći). Koristi se za lepo formatiran, tabelaran ispis.
  - `%d$` — broj pa odmah znak dolara, bez razmaka.
  - `\n` — nova linija na kraju svakog reda.

---

## Funkcija `main`

```c
int main(int argc, char* argv[]) {
    if(argc != 4) {
        exit(55);
    }
```
- `argc` — broj argumenata komandne linije **uključujući ime programa**. Znači očekuje se poziv poput:
  `./program ulaz.txt izlaz.txt 5`
  (argv[0]=ime programa, argv[1]=ulazni fajl, argv[2]=izlazni fajl, argv[3]=broj ljudi → argc=4)
- Ako broj argumenata nije tačno 4, program se odmah prekida sa kodom greške `55`. Ovo su verovatno **dogovoreni kodovi grešaka** koje traži profesor/odbrana (svaki tip greške ima svoj šifrovani izlazni kod, tipično za školske zadatke).

```c
    FILE* ulaz = fopen(argv[1], "r");
    if(ulaz == NULL) {
        exit(66);
    }
```
- Otvara prvi argument kao fajl za **čitanje** (`"r"`). Ako fajl ne postoji ili ne može da se otvori, `fopen` vraća `NULL`, i program se prekida sa kodom `66`.

```c
    FILE* izlaz = fopen(argv[2], "w");
    if(izlaz == NULL) {
        fclose(ulaz);
        exit(8);
    }
```
- Otvara drugi argument kao fajl za **pisanje** (`"w"` — ako fajl postoji, briše sadržaj; ako ne postoji, kreira ga).
- Ako ne uspe, **prvo zatvara već otvoren `ulaz`** (da ne bi ostao "curenje" resursa / file descriptor-a), pa tek onda izlazi sa kodom `8`. Ovo je bitna razlika u odnosu na prethodni `exit(66)` gde `ulaz` još nije bio otvoren pa nema šta da se zatvara.

```c
    int broj_ljudi = atoi(argv[3]);
```
- Konvertuje treći argument (string) u `int`. `atoi` ne vrši proveru grešaka — ako `argv[3]` nije validan broj, vraća 0, bez upozorenja.

```c
    CVOR* glava;
    init(&glava);
```
- Deklariše pokazivač na glavu liste (još neinicijalizovan — sadrži "smeće").
- `init(&glava)` — poziva funkciju iz `lista.h` koja inicijalizuje listu, najverovatnije postavlja `glava = NULL` unutra. Prima `&glava` (adresu) jer mora da promeni sam pokazivač `glava` u `main`-u, ne kopiju.

```c
    ucitaj_podatke(ulaz, &glava);
    ispisi_podatke(izlaz, glava, broj_ljudi);
```
- Prvo se učitavaju svi podaci iz ulaznog fajla u listu (`&glava` jer ova funkcija dodaje čvorove i menja glavu).
- Zatim se ispisuju (transformisani) podaci u izlazni fajl (`glava` bez `&`, jer ova funkcija ne menja listu, samo je čita).

```c
    clear(&glava);
    fclose(ulaz);
    fclose(izlaz);

    return 0;
}
```
- `clear(&glava)` — oslobađa svu memoriju alociranu za čvorove liste (verovatno prolazi kroz listu i poziva `free()` za svaki čvor), i postavlja `glava` na `NULL`. Ovo je **ključno** da ne bi bilo memory leak-ova (curenja memorije) — nešto što će sigurno proveravati Valgrind/ASan na odbrani.
- `fclose` za oba fajla — oslobađa file descriptor-e i **flush-uje** bafer (osigurava da je sve što je `fprintf` napisao zaista upisano na disk).
- `return 0;` — program se uspešno završio (standardna konvencija: 0 = uspeh).

---

## Ključne stvari za odbranu

1. **Zašto `CVOR**` u nekim funkcijama a `CVOR*` u drugima?** — Dupli pokazivač se koristi svuda gde funkcija treba da promeni **samu glavu liste** (inicijalizacija, dodavanje, brisanje). Obično `*` se koristi kad se lista samo čita/ispisuje.
2. **Redosled `fclose` u error-handling granama** — svaki put kad izlaziš iz programa, moraš zatvoriti sve fajlove koje si već otvorio, inače curi resurs.
3. **`%20s` u `fscanf`** — sprečava buffer overflow, verovatno pitanje "šta bi se desilo da nema limita" → odgovor: moguć overflow/undefined behavior ako je ulaz duži od bafera.
4. **Izlazni kodovi grešaka (55, 66, 8)** — specifični za tvoj zadatak, verovatno propisani od profesora za automatsku proveru.

Ako pošalješ i `lista.h`/`lista.c`, mogu i to da prođem liniju po liniju (posebno `create_node`, `add_to_list`, `init`, `clear` — to su verovatno najvažnija pitanja na odbrani).
