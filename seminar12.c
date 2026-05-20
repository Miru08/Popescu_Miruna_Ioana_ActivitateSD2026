#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina)
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

typedef struct NodPrincipal NodP;
typedef struct NodSecundar NodS;

struct NodPrincipal
{
	Masina info;
	NodP* next;
	NodS* vec;
};

struct NodSecundar
{
	NodP* info;
	NodS* next;
};

void inserarelistaP(NodP** cap, Masina masinaNoua)
{
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vec = NULL;

	if (*cap)
	{
		NodP* aux = *cap;

		while (aux->next)
		{
			aux = (aux)->next;
		}
		(aux)->next = nou;
	}
	else
	{
		*cap = nou;
	}

}

void inserareinlistaS(NodS** cap, NodP* vec)
{
	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->info = vec;
	nou->next = NULL;

	if (*cap)
	{
		NodS* aux = *cap;
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
	{
		(*cap) = nou;
	}
}


NodP* cautaNodDupaID(NodP* cap, int idCautat)
{
	while (cap != NULL && cap->info.id != idCautat)
	{
		cap = cap->next;
	}
	return cap;
}

//4.
//inserare muchie
void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop)
{
	if (listaPrincipala)
	{
		NodP* prim = cautaNodDupaID(listaPrincipala, idStart);
		NodP* ult = cautaNodDupaID(listaPrincipala, idStop);

		if (prim && ult)
		{
			inserareinlistaS(&(prim->vec), ult);
			inserareinlistaS(&(ult->vec), prim);

		}
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	NodP* listaP = NULL;

	while (!feof(file))
	{
		Masina m = citireMasinaDinFisier(file);
		inserarelistaP(&listaP, m);

	}
	fclose(file);
	return listaP;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* listaP)
{
	FILE* file = fopen(numeFisier, "r");
	NodP* listaS = NULL;

	while (!feof(file))
	{
		int idStart;
		int idStop;
		fscanf(file, "%d %d", &idStart, &idStop);
		inserareMuchie(listaP, idStart, idStop);
	}
	fclose(file);

}

void dezalocareNoduriGraf(NodP** listaPrincipala)
{
	if (listaPrincipala == NULL || *listaPrincipala == NULL)
	{
		return;
	}

	NodP* curentP = *listaPrincipala;

	while (curentP != NULL)
	{
		NodS* curentS = curentP->vec;
		while (curentS != NULL)
		{
			NodS* auxS = curentS;
			curentS = curentS->next;
			free(auxS);
		}

		if (curentP->info.model != NULL) {
			free(curentP->info.model);
		}
		if (curentP->info.numeSofer != NULL) {
			free(curentP->info.numeSofer);
		}
		NodP* auxP = curentP;
		curentP = curentP->next;
		free(auxP);
	}
	*listaPrincipala = NULL;
}
void afisareGraf(NodP* graf)
{
	while (graf) {
		afisareMasina(graf->info);
		printf("  Conexiuni (Vecini ID): ");
		NodS* vecini = graf->vec;
		if (!vecini) {
			printf("Niciuna");
		}
		while (vecini) {
			printf("%d ", vecini->info->info.id);
			vecini = vecini->next;
		}
		printf("\n---------------------------\n");
		graf = graf->next;
	}
}
int main()
{

	NodP* listaP = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", listaP);
	afisareGraf(listaP);
	dezalocareNoduriGraf(&listaP);

	return 0;
}