#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *niz;
    int velicina;
    int kapacitet;
} DinamickiNiz;

DinamickiNiz* kreirajNiz() {
    DinamickiNiz *dn = (DinamickiNiz*)malloc(sizeof(DinamickiNiz));
    dn->kapacitet = 10;
    dn->velicina = 0;
    dn->niz = (int*)malloc(dn->kapacitet * sizeof(int));
    return dn;
}

void prosiriNiz(DinamickiNiz *dn) {
    dn->kapacitet *= 2;
    dn->niz = (int*)realloc(dn->niz, dn->kapacitet * sizeof(int));
    if (dn->niz == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        exit(1);
    }
    printf("Kapacitet niza prosiren na %d elemenata.\n", dn->kapacitet);
}

void unosPocetnihElemenata(DinamickiNiz *dn) {
    int n, vrednost;
    printf("Unesite koliko elemenata zelite da unesete na pocetku (max 10): ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Broj elemenata ne moze biti negativan!\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (dn->velicina == dn->kapacitet) {
            prosiriNiz(dn);
        }
        printf("Unesite element %d: ", i + 1);
        scanf("%d", &vrednost);
        dn->niz[dn->velicina] = vrednost;
        dn->velicina++;
    }
    printf("Uspesno uneto %d elemenata.\n", n);
}

void ispisNiza(DinamickiNiz *dn) {
    if (dn->velicina == 0) {
        printf("Niz je prazan.\n");
        return;
    }
    printf("Elementi niza [velicina: %d | kapacitet: %d]: ", dn->velicina, dn->kapacitet);
    for (int i = 0; i < dn->velicina; i++) {
        printf("%d ", dn->niz[i]);
    }
    printf("\n");
}

void ispisElementa(DinamickiNiz *dn) {
    int indeks;
    printf("Unesite indeks: ");
    scanf("%d", &indeks);
    if (indeks < 0 || indeks >= dn->velicina) {
        printf("Greska: indeks %d je van opsega!\n", indeks);
        return;
    }
    printf("Element na indeksu %d: %d\n", indeks, dn->niz[indeks]);
}

void dodajNaKraj(DinamickiNiz *dn) {
    int vrednost;
    printf("Unesite vrednost: ");
    scanf("%d", &vrednost);
    if (dn->velicina == dn->kapacitet) {
        prosiriNiz(dn);
    }
    dn->niz[dn->velicina] = vrednost;
    dn->velicina++;
    printf("Dodat element %d na kraj niza.\n", vrednost);
}

void dodajNaIndeks(DinamickiNiz *dn) {
    int indeks, vrednost;
    printf("Unesite indeks: ");
    scanf("%d", &indeks);
    if (indeks < 0 || indeks > dn->velicina) {
        printf("Greska: indeks %d je van opsega!\n", indeks);
        return;
    }
    printf("Unesite vrednost: ");
    scanf("%d", &vrednost);
    if (dn->velicina == dn->kapacitet) {
        prosiriNiz(dn);
    }
    for (int i = dn->velicina; i > indeks; i--) {
        dn->niz[i] = dn->niz[i - 1];
    }
    dn->niz[indeks] = vrednost;
    dn->velicina++;
    printf("Dodat element %d na indeks %d.\n", vrednost, indeks);
}

void obrisiNaIndeksu(DinamickiNiz *dn) {
    int indeks;
    printf("Unesite indeks: ");
    scanf("%d", &indeks);
    if (indeks < 0 || indeks >= dn->velicina) {
        printf("Greska: indeks %d je van opsega!\n", indeks);
        return;
    }
    printf("Obrisan element %d sa indeksa %d.\n", dn->niz[indeks], indeks);
    for (int i = indeks; i < dn->velicina - 1; i++) {
        dn->niz[i] = dn->niz[i + 1];
    }
    dn->velicina--;
}

void oslobodiNiz(DinamickiNiz *dn) {
    free(dn->niz);
    free(dn);
}

int main() {
    DinamickiNiz *dn = kreirajNiz();
    int izbor;

    printf("===== DINAMICKI NIZ =====\n");
    unosPocetnihElemenata(dn);
    printf("\nPocetni niz: ");
    ispisNiza(dn);

    do {
        printf("\n===== MENI =====\n");
        printf("1. Ispis svih elemenata\n");
        printf("2. Ispis elementa na indeksu\n");
        printf("3. Dodaj element na kraj\n");
        printf("4. Dodaj element na indeks\n");
        printf("5. Obrisi element na indeksu\n");
        printf("0. Izlaz\n");
        printf("Vas izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1: ispisNiza(dn);       break;
            case 2: ispisElementa(dn);   break;
            case 3: dodajNaKraj(dn);     break;
            case 4: dodajNaIndeks(dn);   break;
            case 5: obrisiNaIndeksu(dn); break;
            case 0: printf("Izlaz iz programa.\n"); break;
            default: printf("Nepoznat izbor, pokusajte ponovo.\n");
        }

    } while (izbor != 0);

    oslobodiNiz(dn);
    return 0;
}