#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Telefon
{
    int id;
    int RAM;
    char *producator;
    float pret;
    char serie;
};

struct Telefon initializare(int idDat, int ramDat, const char *producatorDat, float pretDat, char serieData)
{
    //   1)Functia creeaza si populează o structură locală pe propriul său cadru de stivă.
    //   2)La întâlnirea instrucțiunii return, sistemul ia absolut toți octeții acelei structuri (id-ul, memoria RAM, prețul, etc.) și îi copiază bit cu bit în variabila t1 aflată pe stiva funcției main.
    //   3)Cadrul de stivă al funcției initializare este apoi distrus definitiv, împreună cu structura creata, dar struct este salvata acum in t1 in main
    struct Telefon temp;
    temp.id = idDat;
    temp.RAM = ramDat;
    temp.producator = (char *)malloc(sizeof(char) * (strlen(producatorDat) + 1)); // retine adresa de inceput a blocului de momorie, de pe heap
    strcpy(temp.producator, producatorDat);
    temp.pret = pretDat;
    temp.serie = serieData; // ocupa fix un octet, se copiaza codul ascii
    return temp;
}

void afisare(struct Telefon sursa)
{
    if (sursa.producator != NULL)
        printf("ID: %d. Telefonul %s cu seria %c are %d GB RAM si costa %5.2f RON.\n", sursa.id, sursa.producator, sursa.serie, sursa.RAM, sursa.pret);
    else
        printf("ID: %d. Telefonul cu seria %c are %d GB RAM si costa %5.2f RON.\n", sursa.id, sursa.serie, sursa.RAM, sursa.pret);
}

void afisareVector(struct Telefon *vectorTel, int nrElemente) //*vectorTel retine adresa de inceput
{
    for (int i = 0; i < nrElemente; i++)
    {
        afisare(vectorTel[i]); // index face dereferentiere si nu se trimite adresa
    }
}

struct Telefon copiazaTelefon(struct Telefon sursa)
{
    return initializare(sursa.id, sursa.RAM, sursa.producator, sursa.pret, sursa.serie);
}

struct Telefon *copiazaPrimeleNElemente(struct Telefon *vectorSursa, int nrElementeVector, int nrElementeCopiate)
{
    if (nrElementeCopiate > nrElementeVector)
        nrElementeCopiate = nrElementeVector;
    struct Telefon *vectorNou = (struct Telefon *)malloc(sizeof(struct Telefon) * nrElementeCopiate);

    for (int i = 0; i < nrElementeCopiate; i++)
        vectorNou[i] = copiazaTelefon(vectorSursa[i]);

    return vectorNou;
}

int main()
{
    struct Telefon t1 = initializare(1, 34, "Samsung", 3400, 'S');
    afisare(t1);

    int nrTel = 3;
    struct Telefon *vectorTelefoane = (struct Telefon *)malloc(sizeof(struct Telefon) * nrTel); // va stoca adresa de inceput a intregului bloc continuu de memorie
    vectorTelefoane[0] = initializare(2, 16, "Apple", 4500, 'P');
    vectorTelefoane[1] = initializare(3, 64, "Apple", 8000, 'A');
    vectorTelefoane[2] = initializare(4, 16, "Motorola", 3200, 'S');

    afisareVector(vectorTelefoane, nrTel);

    int nrTelCopiate = 2;
    struct Telefon *primeleNTelefoane = (struct Telefon *)malloc(sizeof(struct Telefon) * nrTelCopiate);
    primeleNTelefoane = copiazaPrimeleNElemente(vectorTelefoane, nrTel, nrTelCopiate);
    printf("\nPrimele %d telefoane din vectorul initial: \n", nrTelCopiate);
    afisareVector(primeleNTelefoane, nrTelCopiate);

    return 0;
}