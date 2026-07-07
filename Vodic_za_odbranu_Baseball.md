---
title: "Vodič za pripremu odbrane — Zadatak \"Baseball\" (jednostruko spregnuta lista)"
author: "Priprema za predispitne obaveze"
date: ""
---

# 1. Uvod

Ovaj dokument je kompletan vodič za pripremu **kompajliranja, pokretanja, izmene i usmene odbrane** rešenja zadatka sa jednostruko spregnutom listom (Baseball). Pokriva:

- Predaju rešenja (šta se šalje, kako se potpisuje poruka)
- Kompajliranje i pokretanje (Windows/MSYS2, Linux)
- Objašnjenje svake linije koda u `resenje.c` i `lista.c`
- Osnovne pojmove iz C-a koji se ispituju na odbrani (string, pokazivač, struktura, dinamička memorija)
- Gotove primere dodatnih funkcija (traženje, filtriranje, računanje) koje se mogu tražiti da se dopišu uživo
- Najčešća pitanja i odgovore za odbranu

---

# 2. Predaja rešenja — PRAVILA (obavezno pročitati)

- Fajl sa rešenjem **mora** da se zove tačno **`resenje.c`** (malim slovima, bez razmaka).
- `lista.h`, `lista.c` i `Makefile` se **ne šalju** — to je već dato i ne menja se (osim ako profesor eksplicitno ne kaže drugačije).
- U **samoj poruci** (mejlu/sistemu za predaju), obavezno se potpisati sa:
  - **Punim imenom i prezimenom**
  - **Brojem indeksa**
- Neblagovremeno poslata rešenja se **ne prihvataju**, bez izuzetka (ni uz prijavu ponovnog polaganja).

**Checklist pre slanja:**

- [ ] Fajl se zove tačno `resenje.c`
- [ ] Kod se uspešno kompajlira sa datim `lista.c`, `lista.h` i `Makefile` (bez izmena tih fajlova)
- [ ] Testiran je izlaz i poklapa se sa primerom iz teksta zadatka
- [ ] Nema globalnih promenljivih
- [ ] Sve je organizovano u funkcije
- [ ] U poruci je upisano ime, prezime i broj indeksa

---

# 3. Struktura projekta

```
projekat/
├── lista.h        (dato, ne menja se)
├── lista.c         (dato, ne menja se)
├── resenje.c       (TVOJ rad — jedini fajl koji predaješ)
└── Makefile        (dato, ne menja se)
```

`lista.h` definiše strukturu čvora i deklariše funkcije koje su već implementirane u `lista.c`:

```c
typedef struct cvor_st{
    char domacin[MAX_DOMACI];
    char gost[MAX_GOST];
    int cena_karte;
    int ukupna_cena;
    struct cvor_st* sledeci;
} CVOR;

void init(CVOR** glava);
CVOR* create_node(char* domacin, char* gost, int cena_karte);
void add_to_list(CVOR** glava, CVOR* novi);
void clear(CVOR** glava);
```

`resenje.c` koristi ove funkcije da učita podatke, izračuna ukupnu cenu i ispiše rezultat.

---

# 4. Kompajliranje i pokretanje

## 4.1. Preko terminala (preporučeno, radi svuda)

Otvoriti terminal (na Windowsu: **MSYS2 UCRT64**, na Linuxu: obični terminal), pozicionirati se u folder projekta:

```bash
cd putanja/do/foldera
```

**Direktno preko gcc-a (bez Makefile-a):**

```bash
gcc lista.c resenje.c -o a.out
./a.out input.txt output.txt 6
```

Na Windowsu izlazni fajl će se zvati `a.exe`, pa se pokreće sa:

```bash
gcc lista.c resenje.c -o a.exe
a.exe input.txt output.txt 6
```

**Preko Makefile-a:**

```bash
make        # na Linuxu
mingw32-make    # na Windowsu (MSYS2), ako "make" ne radi
```

pa zatim pokrenuti `a.out` (ili `a.exe`) na isti način kao gore.

**Brisanje generisanih fajlova (.o, executable):**

```bash
make clean
```

## 4.2. Najčešće greške pri kompajliranju

| Greška | Uzrok | Rešenje |
|---|---|---|
| `undefined reference to 'WinMain'` | Pokušano kompajliranje samo jednog fajla bez `main` funkcije (npr. samo `lista.c`) | Kompajlirati **oba** fajla zajedno: `gcc lista.c resenje.c -o a.exe` |
| `undefined reference to 'create_node'` (i slične) | `resenje.c` kompajliran sam, bez `lista.c` | Isto — kompajlirati oba fajla zajedno ili koristiti Makefile |
| `lista.h: No such file or directory` | Header nije u istom folderu kao `.c` fajlovi | Sve fajlove (`.c`, `.h`) držati u istom folderu |
| VS Code "build active file" ne radi | Taj podrazumevani task kompajlira samo trenutno otvoreni fajl, a ovde ih ima više | Ne koristiti taj task — koristiti terminal (vidi 4.1) |

## 4.3. Pokretanje sa argumentima komandne linije

Program se poziva sa **tri argumenta**:

```bash
./a.out ulazni_fajl.txt izlazni_fajl.txt broj_ljudi
```

Primer:

```bash
./a.out input.txt output.txt 6
```

- `argv[0]` = ime programa (`./a.out`)
- `argv[1]` = ulazna datoteka (`input.txt`)
- `argv[2]` = izlazna datoteka (`output.txt`)
- `argv[3]` = broj ljudi, string koji se pretvara u broj pomoću `atoi()` (`"6"` → `6`)

Ako broj argumenata nije tačan (`argc != 4`), program se gasi sa **exit(55)**.

---

# 5. Očekivano ponašanje i test primer

**input.txt:**

```
YANKEES     REDSOX          60
DODGERS     GIANTS          75
METS        PHILLIES        55
YANKEES     MARINERS        80
ASTROS      CUBS            65
BRAVES      YANKEES         50
METS        BREWERS         70
PADRES      ROCKIES         85
YANKEES     ANGELS          90
BRAVES      NATIONALS       45
```

Poziv: `./a.out input.txt output.txt 6`

**output.txt (očekivano):**

```
YANKEES         REDSOX          360$
DODGERS         GIANTS          450$
METS            PHILLIES        330$
YANKEES         MARINERS        480$
ASTROS          CUBS            390$
BRAVES          YANKEES         300$
METS            BREWERS         420$
PADRES          ROCKIES         510$
YANKEES         ANGELS          540$
BRAVES          NATIONALS       270$
```

Svaka cena = `cena_karte * broj_ljudi` (npr. 60 × 6 = 360).

**Kodovi grešaka koje program vraća:**

| Kod | Kada se javlja |
|---|---|
| `55` | Loš broj argumenata komandne linije (`argc != 4`) |
| `66` | Ulazni fajl ne može da se otvori |
| `8` | Izlazni fajl ne može da se otvori |

Test grešaka:

```bash
./a.out                          # -> exit code 55
./a.out nepostojeci.txt out.txt 6   # -> exit code 66
```

(Kod na Linuxu/MSYS2 se proverava sa `echo $?` odmah posle pokretanja programa.)

---

# 6. Kompletno objašnjenje koda

## 6.1. `resenje.c` — pregled funkcija

```c
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

void ucitaj_podatke(FILE* ulaz, CVOR** glava) {
    char domacin[MAX_DOMACI];
    char gost[MAX_GOST];
    int cena_karte;
    CVOR* novi;

    while(fscanf(ulaz, "%20s %20s %d", domacin, gost, &cena_karte) == 3) {
        novi = create_node(domacin, gost, cena_karte);
        add_to_list(glava, novi);
    }
}

void ispisi_podatke(FILE* izlaz, CVOR* glava, int broj_ljudi) {
    CVOR* trenutni = glava;
    int ukupna_cena;

    while(trenutni != NULL) {
        ukupna_cena = trenutni->cena_karte * broj_ljudi;
        fprintf(izlaz, "%-15s %-15s %d$\n", trenutni->domacin, trenutni->gost, ukupna_cena);
        trenutni = trenutni->sledeci;
    }
}

int main(int argc, char* argv[]) {
    if(argc != 4) {
        exit(55);
    }

    FILE* ulaz = fopen(argv[1], "r");
    if(ulaz == NULL) {
        exit(66);
    }

    FILE* izlaz = fopen(argv[2], "w");
    if(izlaz == NULL) {
        fclose(ulaz);
        exit(8);
    }

    int broj_ljudi = atoi(argv[3]);

    CVOR* glava;
    init(&glava);

    ucitaj_podatke(ulaz, &glava);
    ispisi_podatke(izlaz, glava, broj_ljudi);

    clear(&glava);
    fclose(ulaz);
    fclose(izlaz);

    return 0;
}
```

### Objašnjenje red po red

**`ucitaj_podatke`**

- Čita iz ulazne datoteke tri vrednosti odjednom: `domacin`, `gost` (kao stringove) i `cena_karte` (kao ceo broj), koristeći `fscanf`.
- `%20s` ograničava upis na najviše 20 karaktera (plus null-terminator), da se ne bi prepunio bafer od `MAX_DOMACI`/`MAX_GOST` = 21 bajt — ovo je **bezbednosna mera protiv prepunjavanja bafera (buffer overflow)**.
- `fscanf` vraća **broj uspešno učitanih vrednosti**. Petlja `while(... == 3)` znači: "dokle god su sve tri vrednosti uspešno pročitane, nastavi". Kad se dođe do kraja fajla ili loš format, `fscanf` vraća manje od 3 i petlja se prekida.
- Za svaku učitanu liniju pravi se novi čvor (`create_node`) i dodaje na kraj liste (`add_to_list`).

**`ispisi_podatke`**

- `trenutni` je pokazivač koji "šeta" kroz listu, počevši od `glava`.
- Za svaki čvor računa `ukupna_cena = cena_karte * broj_ljudi`.
- Ispisuje u formatu `%-15s %-15s %d$` — objašnjeno detaljno u sekciji 6.3.
- `trenutni = trenutni->sledeci` pomera pokazivač na sledeći čvor — kada dođe do `NULL`, petlja se završava (to je kraj liste).

**`main`**

- Proverava broj argumenata komandne linije.
- Otvara ulaznu datoteku za čitanje (`"r"`), proverava da li je otvaranje uspelo.
- Otvara izlaznu datoteku za pisanje (`"w"`) — ako ulazni fajl ne postoji, otvaranje izlaznog fajla ga **kreira** (ako ne postoji) ili **briše sadržaj** (ako postoji).
- `atoi(argv[3])` pretvara string (npr. `"6"`) u `int` (6).
- `init(&glava)` inicijalizuje praznu listu (glava = NULL).
- Poziva se učitavanje pa ispisivanje.
- **`clear(&glava)`** oslobađa svu memoriju zauzetu za listu — ovo je **kritično**, jer je memorija zauzeta pomoću `malloc` u `create_node`, pa mora ručno da se oslobodi pomoću `free` (C nema automatsko "garbage collection").
- Zatvaraju se oba fajla na kraju (`fclose`).

## 6.2. `lista.c` — funkcije koje su već date

```c
void init(CVOR** glava) {
    *glava = NULL;
}
```
Postavlja početnu vrednost glave liste na `NULL`, što znači "lista je prazna".

```c
CVOR* create_node(char* domacin, char* gost, int cena_karte) {
    CVOR* tmp = malloc(sizeof(CVOR));
    if(tmp==NULL) {
        printf("Ne moze se zauzeti memorija\n");
        exit(3);
    }
    strcpy(tmp->domacin, domacin);
    strcpy(tmp->gost, gost);
    tmp->cena_karte = cena_karte;
    tmp->sledeci = NULL;
    return tmp;
}
```
- `malloc(sizeof(CVOR))` **dinamički** zauzima memoriju na hipu (heap) veličine jedne strukture `CVOR`. Vraća pokazivač na tu memoriju (ili `NULL` ako nema dovoljno memorije).
- `strcpy` kopira sadržaj stringa (karakter po karakter, do `'\0'`) u polja strukture.
- `sledeci = NULL` znači da novi čvor **za sada** nema sledbenika (biće povezan kasnije u `add_to_list`).

```c
void add_to_list(CVOR** glava, CVOR* novi) {
    CVOR* trenutni;
    if(*glava==NULL) {
        *glava = novi;
    } else {
        trenutni = *glava;
        while(trenutni->sledeci!=NULL) {
            trenutni = trenutni->sledeci;
        }
        trenutni->sledeci = novi;
    }
}
```
- Ako je lista prazna, novi čvor postaje glava.
- Inače, "šeta" se kroz listu do **poslednjeg** čvora (onog čiji je `sledeci == NULL`) i tamo se "zakači" novi čvor.
- Ovo je dodavanje **na kraj liste** (za razliku od dodavanja na početak, koje bi bilo brže ali bi obrnulo redosled).

```c
void clear(CVOR** glava) {
    CVOR *tmp;
    while(*glava != NULL) {
        tmp = *glava;
        *glava = (*glava)->sledeci;
        tmp->sledeci = NULL;
        free(tmp);
    }
}
```
- Prolazi kroz listu i **oslobađa** svaki čvor pojedinačno (`free(tmp)`).
- Prvo se sačuva pokazivač na trenutni čvor (`tmp`), zatim se glava pomeri na sledeći čvor, tek onda se `tmp` oslobađa — ovo je bitno da se ne izgubi pokazivač na ostatak liste pre nego što se on sačuva.
- Nakon ove funkcije, `*glava` je opet `NULL` (prazna lista), i nema **memorijskih curenja** (memory leak).

## 6.3. Format specifikatori — `%-15s %-15s %d$`

| Deo | Značenje |
|---|---|
| `%s` | Ispisuje string (niz karaktera do `'\0'`) |
| `-15` | Levo poravnanje (`-`) unutar polja širine **15** karaktera; ako je string kraći, popunjava se razmacima sa desne strane |
| `%d` | Ispisuje ceo broj (`int`) |
| `$` | Literalni karakter — ispisuje se tačno kao znak dolara posle broja |
| `\n` | Prelazak u novi red |

Primer: `"YANKEES"` (7 karaktera) uz `%-15s` postaje `"YANKEES        "` (7 + 8 razmaka = 15 karaktera ukupno), pa se odmah nastavlja sledeće polje.

---

# 7. Osnovni pojmovi iz C-a (za usmena pitanja)

## 7.1. Šta je string u C-u?

String u C-u je **niz karaktera (`char`) koji se završava specijalnim karakterom `'\0'`** (null-terminator, vrednost 0). Ne postoji poseban tip "string" kao u nekim drugim jezicima — to je konvencija: niz `char` elemenata gde poslednji "pravi" karakter prati `'\0'` koji označava kraj.

Primer: `char ime[21]` može da smesti string dužine do 20 karaktera (jedno mesto se rezerviše za `'\0'`).

Funkcije koje rade sa stringovima (iz `<string.h>`):
- `strcpy(dest, src)` — kopira string iz `src` u `dest`
- `strlen(s)` — vraća dužinu stringa (bez `'\0'`)
- `strcmp(a, b)` — poredi dva stringa (vraća 0 ako su jednaki)
- `strcat(dest, src)` — nadovezuje `src` na kraj `dest`

## 7.2. Šta je pokazivač (pointer)?

Pokazivač je promenljiva koja **čuva adresu** druge promenljive u memoriji, a ne samu vrednost.

```c
int x = 5;
int* p = &x;   // p čuva ADRESU promenljive x
printf("%d", *p);  // *p čita VREDNOST na koju p pokazuje (ispisuje 5)
```

- `&x` — operator "adresa od" (address-of)
- `*p` — operator "dereferenciranje" (čitanje vrednosti sa te adrese)

**Zašto se koristi `CVOR**` (pokazivač na pokazivač)?**

Kada se funkciji prosledi `CVOR** glava`, funkcija može da **promeni samu glavu liste** (npr. da postavi novi prvi čvor). Da je prosleđeno samo `CVOR* glava` (obična kopija pokazivača), izmena unutar funkcije ne bi uticala na promenljivu u `main`-u — jer bi funkcija radila samo sa **kopijom** pokazivača.

## 7.3. Šta je struktura (struct)?

Struktura je korisnički definisan tip podataka koji grupiše više različitih polja (koja mogu biti različitog tipa) pod jednim imenom.

```c
typedef struct cvor_st{
    char domacin[21];
    char gost[21];
    int cena_karte;
    int ukupna_cena;
    struct cvor_st* sledeci;
} CVOR;
```

Ovde `CVOR` grupiše ime domaćina, gosta, cenu karte i pokazivač na sledeći čvor (što omogućava povezivanje u listu).

- `struct cvor_st* sledeci` je pokazivač **na istu strukturu** — to je ono što jednu strukturu pretvara u "čvor" liste, jer omogućava lančano povezivanje.

## 7.4. Dinamička alokacija memorije (`malloc`/`free`)

- `malloc(broj_bajtova)` traži od operativnog sistema komad memorije te veličine na **hipu (heap)** i vraća pokazivač na njega (ili `NULL` ako nema dovoljno memorije).
- `sizeof(CVOR)` automatski računa koliko bajtova zauzima jedna struktura `CVOR`.
- **Uvek proveriti da li je `malloc` vratio `NULL`** pre korišćenja (kao u `create_node`).
- `free(p)` **oslobađa** memoriju koju je `p` zauzeo — obavezno kad više nije potrebna, inače nastaje **memorijsko curenje (memory leak)**: program troši sve više memorije koju nikad ne vraća sistemu.
- Nakon `free(p)`, `p` postaje **"dangling pointer"** (pokazuje na oslobođenu memoriju) — ne sme se koristiti dok mu se ponovo ne dodeli nova validna adresa.

## 7.5. Šta je jednostruko spregnuta lista?

Struktura podataka gde je svaki element (čvor) povezan sa **sledećim** elementom preko pokazivača, ali ne i sa prethodnim (za razliku od dvostruko spregnute liste). Poslednji čvor ima `sledeci == NULL`.

Prednosti u odnosu na niz (array):
- Dinamička veličina (ne mora se unapred znati koliko će elemenata biti)
- Lako dodavanje/brisanje elemenata bez pomeranja ostalih

Mane:
- Nema direktnog pristupa i-tom elementu (mora se ići redom od glave)
- Zauzima malo više memorije po elementu (zbog pokazivača)

## 7.6. `argc` i `argv`

- `argc` (argument count) — broj argumenata prosleđenih programu, **uključujući ime samog programa**.
- `argv` (argument vector) — niz stringova; `argv[0]` je ime programa, `argv[1]`, `argv[2]`... su stvarni argumenti.

Za poziv `./a.out input.txt output.txt 6`:
- `argc` = 4
- `argv[0]` = `"./a.out"`, `argv[1]` = `"input.txt"`, `argv[2]` = `"output.txt"`, `argv[3]` = `"6"`

## 7.7. Rad sa fajlovima

| Funkcija | Namena |
|---|---|
| `fopen(ime, mod)` | Otvara fajl; `"r"` = čitanje, `"w"` = pisanje (briše postojeći sadržaj), `"a"` = dodavanje na kraj |
| `fscanf(f, format, ...)` | Čita formatirane podatke iz fajla, vraća broj uspešno pročitanih vrednosti |
| `fprintf(f, format, ...)` | Piše formatirane podatke u fajl |
| `fclose(f)` | Zatvara fajl i oslobađa resurse — **uvek obavezno** posle rada sa fajlom |

Uvek proveriti da li je `fopen` vratio `NULL` (fajl ne postoji, nema dozvole za pristup, itd.) pre nego što se fajl koristi.

---

# 8. Priprema za "dopiši jednu funkciju" na odbrani

Na odbrani će se tražiti da se **doda jedna nova funkcija** (traženje/filtriranje/računanje). Ispod su gotovi primeri za sve tri kategorije — logiku treba razumeti, ne samo prepisati, jer će se tražiti *slična*, ne nužno *ista* funkcija.

## 8.1. Primer — TRAŽENJE (pronađi utakmicu po domaćinu)

```c
CVOR* pronadji_po_domacinu(CVOR* glava, char* domacin) {
    CVOR* trenutni = glava;
    while(trenutni != NULL) {
        if(strcmp(trenutni->domacin, domacin) == 0) {
            return trenutni;
        }
        trenutni = trenutni->sledeci;
    }
    return NULL;
}
```

Poziv u `main`-u:
```c
CVOR* rezultat = pronadji_po_domacinu(glava, "YANKEES");
if(rezultat != NULL) {
    printf("Pronadjeno: %s protiv %s\n", rezultat->domacin, rezultat->gost);
} else {
    printf("Nije pronadjeno\n");
}
```

## 8.2. Primer — FILTRIRANJE (ispiši samo utakmice sa cenom većom od zadate)

```c
void ispisi_skuplje_od(FILE* izlaz, CVOR* glava, int granica) {
    CVOR* trenutni = glava;
    while(trenutni != NULL) {
        if(trenutni->cena_karte > granica) {
            fprintf(izlaz, "%-15s %-15s %d$\n", trenutni->domacin, trenutni->gost, trenutni->cena_karte);
        }
        trenutni = trenutni->sledeci;
    }
}
```

## 8.3. Primer — RAČUNANJE (prosečna cena karte svih utakmica)

```c
double prosecna_cena(CVOR* glava) {
    CVOR* trenutni = glava;
    int suma = 0;
    int broj = 0;
    while(trenutni != NULL) {
        suma += trenutni->cena_karte;
        broj++;
        trenutni = trenutni->sledeci;
    }
    if(broj == 0) {
        return 0;
    }
    return (double) suma / broj;
}
```

## 8.4. Primer — RAČUNANJE (najskuplja utakmica — maksimum)

```c
CVOR* najskuplja_utakmica(CVOR* glava) {
    if(glava == NULL) {
        return NULL;
    }
    CVOR* trenutni = glava->sledeci;
    CVOR* max = glava;
    while(trenutni != NULL) {
        if(trenutni->cena_karte > max->cena_karte) {
            max = trenutni;
        }
        trenutni = trenutni->sledeci;
    }
    return max;
}
```

## 8.5. Primer — RAČUNANJE (broj čvorova u listi)

```c
int broj_utakmica(CVOR* glava) {
    int brojac = 0;
    CVOR* trenutni = glava;
    while(trenutni != NULL) {
        brojac++;
        trenutni = trenutni->sledeci;
    }
    return brojac;
}
```

## 8.6. Primer — FILTRIRANJE (utakmice gde je zadati tim domaćin ILI gost)

```c
void ispisi_utakmice_tima(FILE* izlaz, CVOR* glava, char* tim) {
    CVOR* trenutni = glava;
    while(trenutni != NULL) {
        if(strcmp(trenutni->domacin, tim) == 0 || strcmp(trenutni->gost, tim) == 0) {
            fprintf(izlaz, "%-15s %-15s %d$\n", trenutni->domacin, trenutni->gost, trenutni->cena_karte);
        }
        trenutni = trenutni->sledeci;
    }
}
```

## 8.7. Opšti obrazac (koristiti kao "šablon" na odbrani)

Skoro sve dodatne funkcije nad jednostruko spregnutom listom imaju **isti oblik**:

```c
POVRATNI_TIP ime_funkcije(CVOR* glava, /* dodatni parametri po potrebi */) {
    CVOR* trenutni = glava;
    /* inicijalizacija promenljivih za rezultat (suma, brojac, max...) */

    while(trenutni != NULL) {
        /* provera uslova i/ili računanje sa trenutni->polje */
        trenutni = trenutni->sledeci;
    }

    /* vraćanje ili ispis rezultata */
}
```

Ako se ovaj obrazac razume, svaka varijacija (traži po X, filtriraj po Y, izračunaj Z) se lako prilagođava.

---

# 9. Najčešća pitanja na odbrani i odgovori

**P: Kako se dodaje čvor u listu?**
O: Prvo se pravi novi čvor pomoću `malloc`-a (`create_node`), popune mu se polja, a zatim se poziva `add_to_list` koja: ako je lista prazna postavlja novi čvor za glavu, a inače prolazi kroz listu do poslednjeg čvora (onog sa `sledeci == NULL`) i tamo ga zakači.

**P: Šta je string?**
O: Niz karaktera koji se završava karakterom `'\0'`.

**P: Zašto se koristi `CVOR**` a ne `CVOR*` kao parametar za glavu liste?**
O: Da bi funkcija mogla da promeni samu promenljivu `glava` iz pozivajuće funkcije (npr. postavi novu glavu), a ne samo lokalnu kopiju pokazivača.

**P: Šta se dešava ako se ne pozove `free`?**
O: Nastaje memorijsko curenje (memory leak) — memorija ostaje zauzeta sve dok se program ne završi, jer C nema automatsko upravljanje memorijom.

**P: Kako se prolazi kroz listu?**
O: Postavi se pomoćni pokazivač na `glava`, i u petlji (`while (trenutni != NULL)`) se radi obrada, pa se pokazivač pomera na `trenutni->sledeci`, sve dok se ne dođe do `NULL` (kraj liste).

**P: Šta je razlika između `%d` i `%s` u `printf`/`fprintf`?**
O: `%d` ispisuje ceo broj (`int`), `%s` ispisuje string (niz karaktera do `'\0'`).

**P: Šta radi `fopen` i šta vraća ako ne uspe?**
O: Otvara fajl u zadatom modu (čitanje/pisanje) i vraća pokazivač na `FILE`; ako ne uspe, vraća `NULL`.

**P: Zašto se proverava povratna vrednost `malloc`-a?**
O: Zato što `malloc` može da vrati `NULL` ako nema dovoljno slobodne memorije — korišćenje takvog pokazivača bez provere dovodi do pada programa (segmentation fault).

**P: Šta je `argc`/`argv`?**
O: `argc` je broj argumenata komandne linije (uključujući ime programa), `argv` je niz tih argumenata kao stringova.

**P: Zašto se koristi `atoi`?**
O: Zato što svi argumenti komandne linije stižu kao stringovi (`char*`), pa je potrebno pretvoriti string (npr. `"6"`) u broj (`int 6`) da bi se mogao koristiti u matematičkoj operaciji.

---

# 10. Finalni checklist pred odbranu

- [ ] Znam da kompajliram rešenje iz terminala (gcc ili make) bez VS Code dugmadi
- [ ] Znam da pokrenem program sa tačnim brojem argumenata i protumačim izlaz
- [ ] Znam da objasnim svaku liniju u `resenje.c`
- [ ] Znam da objasnim `init`, `create_node`, `add_to_list`, `clear` iz `lista.c`
- [ ] Znam šta je string, pokazivač, struktura, dinamička memorija
- [ ] Mogu da dopišem jednostavnu funkciju za traženje/filtriranje/računanje po šablonu iz sekcije 8
- [ ] Fajl za predaju se zove tačno `resenje.c`
- [ ] U poruci za predaju upisano ime, prezime i broj indeksa
