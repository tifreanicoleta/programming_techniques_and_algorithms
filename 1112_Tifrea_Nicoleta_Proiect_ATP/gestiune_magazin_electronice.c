#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

typedef struct {
    int cod;
    char nume[50];
    char marca[30];
    float pret;
    int stoc;
} Produs;

// 1. Creare fisier
void creare(char* b1) {
    FILE* f;
    Produs b;
    f = fopen(b1, "wb"); // deschide fisierul pentru scriere binara
    if (!f) {
        printf("\n Fisierul %s nu poate fi deschis", b1);
        return;
    }
    printf("Introduceti datele produsului - apasati tasta 0 pentru a reveni la meniul principal \n");
    printf(" Cod produs: "); scanf("%d", &b.cod);
    while (b.cod != 0) {
        getchar();
        printf(" Nume produs: "); gets(b.nume);
        printf(" Marca: "); gets(b.marca);
        printf(" Pret: "); scanf("%f", &b.pret);
        printf(" Stoc: "); scanf("%d", &b.stoc);
        fwrite(&b, sizeof(Produs), 1, f); //scrie produsul in fisier
        printf("\n Cod: "); scanf("%d", &b.cod);
    }
    fclose(f);
}

// 2. Adaugare de produse
void adaugare(char* b1) {
    FILE* f;
    Produs b;
    f = fopen(b1, "rb+"); // deschide fisierul pentru citire si scriere
    if (!f) {
        printf("\n Fisierul %s nu poate fi deschis", b1);
        return;
    }
    fseek(f, 0, SEEK_END); // muta cursorul la final, pentru a adauga inregistrarile noi dupa cele deja existente
    printf("Introduceti datele produsului - apasati tasta 0 pentru a reveni la meniul principal \n");
    printf("Cod produs: "); scanf("%d", &b.cod);
    while (b.cod != 0) {
        getchar();
        printf("Nume produs: "); gets(b.nume);
        printf("Marca: "); gets(b.marca);
        printf("Pret: "); scanf("%f", &b.pret);
        printf("Stoc: "); scanf("%d", &b.stoc);
        fwrite(&b, sizeof(Produs), 1, f);
        printf("\n Cod: "); scanf("%d", &b.cod);
    }
    fclose(f);
}

// 3. Modificare stoc dupa cod
void modStoc(char* b1) {
    FILE* f;
    Produs b;
    int cod, i;

    f = fopen(b1, "rb+"); //deschide fisierul in modul citire si scriere
    if (!f) {
        printf("\n Fisierul %s nu poate fi deschis", b1);
        return;
    }

    printf("Introduceti datele produsului - apasati tasta 0 pentru a reveni la meniul principal \n");
    printf("\n Cod produs: "); scanf("%d", &cod);

    while (cod != 0) {
        rewind(f);
        i = 0;
        while (fread(&b, sizeof(Produs), 1, f) == 1) { //citeste din fisier
            if (b.cod == cod) {
                i = 1;
                printf("\n %s | Marca: %s | Pret: %.2f | Stoc curent: %d\n", b.nume, b.marca, b.pret, b.stoc);
                printf("Noul stoc: "); scanf("%d", &b.stoc);
                fseek(f, -sizeof(Produs), SEEK_CUR);
                fwrite(&b, sizeof(Produs), 1, f);
                break;
            }
        }
        if (!i) {
            printf("\n Produsul cu codul %d nu a fost gasit.\n", cod);
        }
        printf("\n Cod produs: "); scanf("%d", &cod);
    }

    fclose(f);
}

// 4. Reducere pret pentru produse cu stoc mare
void modPret(char* b1){
    FILE* f;
    Produs b;
    float reducere;
    int cod, prag, i;

    f = fopen(b1, "rb+");  // deschide fisierul pentru scriere/citire
    if (!f) {
        printf("\n Fisierul %s nu poate fi deschis", b1);
        return;
    }

    printf("Reducere pret produse - apasati tasta 0 pentru a reveni la meniul principal\n");
    printf("\n Cod produs: "); scanf("%d", &cod);

    while (cod != 0) {
        printf("Stoc mai mare de: "); scanf("%d", &prag);
        printf("Reducere de pret: "); scanf("%f", &reducere);

        rewind(f);
        i = 0;
        while (fread(&b, sizeof(Produs), 1, f) == 1) {
            if (b.cod == cod && b.stoc > prag) {
                i = 1;
                b.pret -= reducere;
                fseek(f, -sizeof(Produs), SEEK_CUR);
                fwrite(&b, sizeof(Produs), 1, f);
                printf("\n Pret actualizat: %.2f RON\n", b.pret);
                break;
            }
        }

        if (!i) {
            printf("\n Nu exista produsul cu codul %d si stoc mai mare de %d.\n", cod, prag);
        }

        printf("\n Cod produs: "); scanf("%d", &cod);
    }

    fclose(f);
}

// 5. Stergere Produs
void stergere(char* b1) {
    FILE* f, *g;
    Produs b;
    int i, cod;
    f = fopen(b1, "rb"); //deschide fisierul pentru citire
    if (!f) {
        printf("\n Fisierul %s nu poate fi deschis", b1);
        return;
    }
    printf("Introduceti datele produsului - apasati tasta 0 pentru a reveni la meniul principal\n");
    printf("\n Cod produs de sters: "); scanf("%d", &cod);
    while (cod != 0) {
        rewind(f);
        g = fopen("temp.dat", "wb"); //creeaza un fisier temporar
        i = 0;
        while (fread(&b, sizeof(Produs), 1, f) == 1) {
            if (cod == b.cod) {
                i = 1;
                printf("\n Ati sters: %s, marca: %s, pret: %.2f, stoc: %d\n", b.nume, b.marca, b.pret, b.stoc);
            } else {
                fwrite(&b, sizeof(Produs), 1, g); //scrie in fisierul temporar
            }
        }
        fclose(g);
        if (!i) printf("\n Produsul nu a fost gasit!\n");
        printf("\n Cod produs de sters: "); scanf("%d", &cod);
    }
    fclose(f);
    remove(b1); //sterge fisierul vechi
    rename("temp.dat", b1); //redenumeste temp in fisierul original
}

// 6. Listare produse
void listare(char* s1) {
    FILE *f, *g;
    Produs b;
    int n = 0;
    f = fopen(s1, "rb"); //deschide fisier pentru citire
    if (!f) {
        printf("\n Fisierul %s nu poate fi deschis", s1);
        return;
    }
    printf("Introduceti datele produsului - apasati tasta 0 pentru a reveni la meniul principal\n");
    printf("\n Numele fisierului text: ");
    gets(s1);
    if (strcmp(s1, "0") == 0) {
        fclose(f);
        return;
    }
    g = fopen(s1, "w"); //creeaza fisier text
    fprintf(g, "\n Nr. Cod Nume produs         Marca        Pret     Stoc \n");
    while (fread(&b, sizeof(Produs), 1, f) == 1) {
        fprintf(g, "\n %-3d %-4d %-20s %-12s %-8.2f %-4d", ++n, b.cod, b.nume, b.marca, b.pret, b.stoc);
    }
    fclose(g);
    fclose(f);
}

// 7. Listare produse cu stoc mic
void listStoc(char* b1) {
    FILE* f, * g;
    Produs b;
    int prag, i;
    f = fopen(b1, "rb");
    if (!f) {
        printf("\n Fisierul %s nu poate fi deschis", b1);
        return;
    }
    printf("Introduceti datele produsului - apasati tasta 0 pentru a reveni la meniul principal\n");
    printf("\n Produse cu stoc mai mic de: "); scanf("%d", &prag);
    while (prag != 0) {
        rewind(f);
        getchar();
        printf("Numele fisierului text: "); gets(b1);
        g = fopen(b1, "w");
        i = 0;
        while (fread(&b, sizeof(Produs), 1, f) == 1) {
            if (prag > b.stoc) {
                i = 1;
                fprintf(g, "\n Cod: %-4d Nume: %-20s Marca: %-12s Pret: %-7.2f Stoc: %-3d", b.cod, b.nume, b.marca, b.pret, b.stoc);
            }
        }
        if (!i) printf("\n Nu exista produse cu stoc mai mic de %d\n", prag);
        fclose(g);
        printf("\n Prag stoc: "); scanf("%d", &prag);
    }
    fclose(f);
}

// 8. Afisare produs dupa cod
void afisare(char* b1) {
    FILE* f;
    Produs b;
    int cod, i;
    f = fopen(b1, "rb");
    if (!f) {
        printf("\n Fisierul %s nu poate fi deschis", b1);
        return;
    }
    printf("Introduceti datele produsului - apasati tasta 0 pentru a reveni la meniul principal\n");
    printf("\n Cod produs: "); scanf("%d", &cod);
    while (cod != 0) {
        rewind(f);
        i = 0;
        while (fread(&b, sizeof(Produs), 1, f) == 1) {
            if (cod == b.cod) {
                i = 1;
                printf("\n %s | Marca: %s | Pret: %.2f | Stoc: %d\n", b.nume, b.marca, b.pret, b.stoc);
                break;
            }
        }
        if (!i) printf("Produsul nu a fost gasit!\n");
        printf("\n Cod produs: "); scanf("%d", &cod);
    }
    fclose(f);
}

// Meniu principal
int main () {
    char b1[100];
    int k;
    do {
        printf("\n\n ------ Gestiune Magazin Electronice ------\n");
        printf("\n Alegeti o optiune:\n");
        printf(" 1. Creati un fisier nou\n");
        printf(" 2. Adaugati produse\n");
        printf(" 3. Modificati stocul\n");
        printf(" 4. Reduceti pretul\n");
        printf(" 5. Stergeti produs\n");
        printf(" 6. Listati produsele\n");
        printf(" 7. Listati produsele cu stoc mic\n");
        printf(" 8. Afisati datele unui produs\n");
        printf(" 9. Iesire\n");
        printf("\n Numarul optiunii este: ");
        scanf("%d", &k);
        getchar();
        if (k >= 1 && k <= 8) {
            printf("\n Introduceti numele fisierului (.dat): ");
            gets(b1);
        }
        switch (k) {
            case 1: creare(b1); break;
            case 2: adaugare(b1); break;
            case 3: modStoc(b1); break;
            case 4: modPret(b1); break;
            case 5: stergere(b1); break;
            case 6: listare(b1); break;
            case 7: listStoc(b1); break;
            case 8: afisare(b1); break;
            case 9: printf("\n Ati iesit din aplicatie!\n"); break;
            default: printf("\n Optiune invalida!\n");
        }
    } while (k != 9);
    return 0;
}
