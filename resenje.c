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
