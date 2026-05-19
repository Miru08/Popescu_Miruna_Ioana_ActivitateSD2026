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
	printf("Seroa: %c\n", sursa.serie);
}

void adaugaMasinaInArbore(Nod** root, Masina masinaAdaugata)
{
	if (*root)
	{
		if (masinaAdaugata.id < (*root)->info.id)
			adaugaMasinaInArbore(&((*root)->stanga), masinaAdaugata);

		if (masinaAdaugata.id > (*root)->dreapta)
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

int main()
{
	Nod* root = citireArboreMasiniDinFisier("masini_arbore.txt");

}