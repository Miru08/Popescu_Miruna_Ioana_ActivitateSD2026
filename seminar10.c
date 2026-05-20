#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<string.h>
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
	Masina m;
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;

	fgets(buffer, 100, file);

	m.id = atoi(strtok(buffer, sep));
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.model, aux);

	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.numeSofer, aux);

	m.serie = strtok(NULL, sep)[0];

	return m;
}

void afisareMasina(Masina sursa)
{
	printf("Id: %d\n", sursa.id);
	printf("Nr usi: %d\n", sursa.nrUsi);
	printf("Pret: %5.2f\n", sursa.pret);
	printf("Model: %s\n", sursa.model);
	printf("Nume sofer: %s\n", sursa.numeSofer);
	printf("Serie: %c\n\n", sursa.serie);
}

int calculeazaInaltimeArbore(Nod* root)
{
	if (root)
	{
		int st = calculeazaInaltimeArbore(root->stanga);
		int dr = calculeazaInaltimeArbore(root->dreapta);
		return 1 + max(st, dr);
	}
	return 0;
}

int getGradEchilibru(Nod* root)
{
	return calculeazaInaltimeArbore(root->stanga) - calculeazaInaltimeArbore(root->dreapta);
}

void rotireStanga(Nod** root)
{
	Nod* aux = (*root)->dreapta;
	(*root)->dreapta = aux->stanga;
	aux->stanga = (*root);
	*root = aux;
}

void rotireDreapta(Nod** root)
{
	Nod* aux = (*root)->stanga;
	(*root)->stanga = aux->dreapta;
	aux->dreapta = (*root);
	*root = aux;
}

void adaugaMasinaInArboreEchilibrat(Nod** root, Masina masinaAdaugata)
{
	if (*root)
	{
		if (masinaAdaugata.id < (*root)->info.id)
			adaugaMasinaInArboreEchilibrat(&((*root)->stanga), masinaAdaugata);
		if (masinaAdaugata.id > (*root)->info.id)
			adaugaMasinaInArboreEchilibrat(&((*root)->dreapta), masinaAdaugata);

		int echi = getGradEchilibru(*root);

		if (echi == -2)
		{
			if (getGradEchilibru((*root)->dreapta) == 1)
				rotireDreapta(&(*root)->dreapta);

			rotireStanga(root);
		}
		else if (echi == 2)
		{
			if (getGradEchilibru((*root)->stanga) == -1)
				rotireStanga(&(*root)->stanga);

			rotireDreapta(root);
		}
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

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier)
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
			adaugaMasinaInArboreEchilibrat(&root, masinaCitita);
		}
	}
	fclose(file);
	return root;
}

void afisareArborePreordine(Nod* root)
{
	if (root)
	{
		afisareMasina(root->info);
		afisareArborePreordine(root->stanga);
		afisareArborePreordine(root->dreapta);
	}
}

void afisareArboreInOrdine(Nod* root)
{
	if (root)
	{
		afisareArboreInOrdine(root->stanga);
		afisareMasina(root->info);
		afisareArboreInOrdine(root->dreapta);
	}
}

void afisarePostOrdine(Nod* root)
{
	if (root)
	{
		afisarePostOrdine(root->stanga);
		afisarePostOrdine(root->dreapta);
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

Masina getMasinaById(Nod* root, int idDat)
{
	Masina m;
	m.id = -1;

	if (root)
	{
		if (root->info.id == idDat)
		{
			m = root->info;
			m.model = (char*)malloc(sizeof(char) * (strlen(root->info.model) + 1));
			strcpy(m.model, root->info.model);
			m.numeSofer = (char*)malloc(sizeof(char) * (strlen(root->info.numeSofer) + 1));
			strcpy(m.numeSofer, root->info.numeSofer);
		}
		else if (idDat < root->info.id)
			return getMasinaById(root->stanga, idDat);
		else if (idDat > root->info.id)
			return getMasinaById(root->dreapta, idDat);
	}
	return m;
}

int getNrNoduri(Nod* root)
{
	if (root)
	{
		return 1 + getNrNoduri(root->stanga) + getNrNoduri(root->dreapta);
	}
	return 0;
}

float getPretTotal(Nod* root)
{
	if (root)
	{
		return root->info.pret + getPretTotal(root->stanga) + getPretTotal(root->dreapta);
	}
	return 0;
}

float getPretMasiniDupaSofer(Nod* root, const char* numeSoferDat)
{
	if (root)
	{
		float st = getPretMasiniDupaSofer(root->stanga, numeSoferDat);
		float dr = getPretMasiniDupaSofer(root->dreapta, numeSoferDat);

		if (strcmp(root->info.numeSofer, numeSoferDat) == 0)
		{
			return root->info.pret + st + dr;
		}
		else
		{
			return st + dr;
		}
	}
	return 0;
}

int main()
{
	Nod* root = citireArboreDeMasiniDinFisier("masini_arbore.txt");

	printf("Preordine: \n");
	afisareArborePreordine(root);

	printf("In ordine: \n");
	afisareArboreInOrdine(root);

	printf("Postordine: \n");
	afisarePostOrdine(root);

	printf("Masina cu id-ul 8: \n");
	Masina m = getMasinaById(root, 8);
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);


	printf("Nr de noduri: %d\n", getNrNoduri(root));

	printf("Pret total: %5.2f\n", getPretTotal(root));

	printf("Pret masina Ionescu: %5.2f", getPretMasiniDupaSofer(root, "Ionescu"));


	dezalocare(&root);
	return 0;
}