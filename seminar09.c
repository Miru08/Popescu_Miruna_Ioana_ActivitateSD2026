#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include<stdlib.h>
#include<stdio.h>

struct Masina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	char serie;
};

typedef struct Masina Masina;
typedef struct Nod Nod;

struct Nod
{
	Masina info;
	Nod* stanga;
	Nod* dreapta;
};

Masina citireMasinaDinFisier(FILE* file)
{
	Masina masinaCitita;
	char buffer[100];
	char sep[3] = ",\n";
	char* aux = NULL;

	fgets(buffer, 100, file);

	masinaCitita.id = atoi(strtok(buffer, sep));
	masinaCitita.nrUsi = atoi(strtok(NULL, sep));
	masinaCitita.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	masinaCitita.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(masinaCitita.model, aux);

	aux = strtok(NULL, sep);
	masinaCitita.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(masinaCitita.numeSofer, aux);

	masinaCitita.serie = strtok(NULL, sep)[0];

	return masinaCitita;
}

void afisareMasina(Masina sursa)
{
	printf("Id: %d\n", sursa.id);
	printf("Nr usi: %d\n", sursa.nrUsi);
	printf("Pret: %2.f\n", sursa.pret);
	printf("Model: %s", sursa.model);
	printf("Nume sofer: %s\n", sursa.numeSofer);
	printf("Seroa: %c\n\n", sursa.serie);
}

void adaugaMasinaInArbore(Nod** root, Masina masinaAdaugata)
{
	if (*root)
	{
		if (masinaAdaugata.id < (*root)->info.id)
			adaugaMasinaInArbore(&((*root)->stanga), masinaAdaugata);

		if (masinaAdaugata.id > (*root)->info.id)
			adaugaMasinaInArbore(&((*root)->dreapta), masinaAdaugata);
	}
	else
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaAdaugata;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*root = nou;
	}
}

Nod* citireArboreMasiniDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Nod* root = NULL;

	if (file == NULL)
	{
		printf("Nu s-a putut deschide fisierul!");
		return root;
	}
	else
	{
		while (!feof(file))
		{
			Masina masinaCitita = citireMasinaDinFisier(file);
			adaugaMasinaInArbore(&root, masinaCitita);
		}
	}
	fclose(file);
	return root;
}

void afisareMasinaDinArborePreordine(Nod* root)
{
	//Preordine: nod, stanga, dreapta → 5, 1, 4, 2, 3, 7, 6, 10, 8, 9
	if (root != NULL)
	{
		afisareMasina(root->info);
		afisareMasinaDinArborePreordine(root->stanga);
		afisareMasinaDinArborePreordine(root->dreapta);
	}
}

void afisareMasinaInOrdine(Nod* root)
{
	//In ordine: stanga, nod, dreapta → 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 (sortata crescator mereu la ABC)
	if (root != NULL)
	{
		afisareMasinaInOrdine(root->stanga);
		afisareMasina(root->info);
		afisareMasinaInOrdine(root->dreapta);
	}
}

void afisareMasinaPostordine(Nod* root)
{
	//Postordine: stanga, dreapta, nod → 3, 2, 4, 1, 6, 9, 8, 10, 7, 5
	if (root != NULL)
	{
		afisareMasinaPostordine(root->stanga);
		afisareMasinaPostordine(root->dreapta);
		afisareMasina(root->info);
	}
}

void dezalocare(Nod** root)
{
	if (*root)
	{
		dezalocare(&((*root)->stanga));
		dezalocare(&((*root)->dreapta));
		if ((*root)->info.model != NULL)
			free((*root)->info.model);
		if ((*root)->info.numeSofer != NULL)
			free((*root)->info.numeSofer);
		free(*root);
		*root = NULL;
	}
}

int main()
{
	Nod* root = citireArboreMasiniDinFisier("masini_arbore.txt");

	printf("Afisare preordine:\n");
	afisareMasinaDinArborePreordine(root);

	printf("Afisare in ordine:\n");
	afisareMasinaInOrdine(root);

	printf("Afisare postordine:\n");
	afisareMasinaPostordine(root);

	dezalocare(&root);
	return 0;
}