#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

typedef struct NodCoada NodCoada;
typedef struct ListaDubla ListaDubla;
typedef struct Masina Masina;

struct Masina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

struct NodCoada
{
	Masina info;
	NodCoada* next;
	NodCoada* prev;
};

struct ListaDubla
{
	NodCoada* first;
	NodCoada* last;
	int nrNoduri;
};

Masina citireMasinaDinFisier(FILE* file)
{
	Masina masinaReturnata;
	char buffer[100];
	char sep[3] = ",\n";
	char* aux = NULL;

	fgets(buffer, 100, file);

	masinaReturnata.id = atoi(strtok(buffer, sep));
	masinaReturnata.nrUsi = atoi(strtok(NULL, sep));
	masinaReturnata.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	masinaReturnata.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(masinaReturnata.model, aux);

	aux = strtok(NULL, sep);
	masinaReturnata.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(masinaReturnata.numeSofer, aux);

	masinaReturnata.serie = strtok(NULL, sep)[0];

	return masinaReturnata;
}

void afisareMasina(Masina masina)
{
	printf("\nId: %d || Nr usi: %d || Pret: %.2f || Model: %s || Nume sofer: %s || Seria: %c\n", masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}

void afisareCoadaDeMasini(ListaDubla lista)
{
	while (lista.first != NULL)
	{
		afisareMasina(lista.first->info);
		lista.first = lista.first->next;
	}
}

void enqueue(ListaDubla* lista, Masina masinaAdaugata)
{
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->info = masinaAdaugata;
	nou->next = NULL; //mereu v-a fi ultimul nou adaugat
	nou->prev = lista->last;

	if (lista->last != NULL)
	{
		lista->last->next = nou;
	}
	else
	{
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

Masina dequeue(ListaDubla* lista)
{
	Masina masinaScoasa;

	if (lista->first != NULL)
	{
		masinaScoasa = lista->first->info;
		NodCoada* aux = lista->first;//folosim aux ca sa dam free(), lista->first trb sa retina mereu adresa primului nod
		lista->first = aux->next;
		free(aux);
		return masinaScoasa;
	}
	else
	{
		masinaScoasa.id = -1;
		return masinaScoasa;
	}
}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");

	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;

	if (file != NULL)
	{
		while (!feof(file))
		{
			Masina masinaCitita = citireMasinaDinFisier(file);
			enqueue(&lista, masinaCitita);
		}
		fclose(file);
	}
	else
	{
		printf("Fisierul nu se poate deschide!");
	}

	return lista;
}

void dezalocareCoadaDeMasini(ListaDubla* lista)
{
	while (lista->first != NULL)
	{
		NodCoada* aux = lista->first;

		if (lista->first->info.model != NULL)
			free(lista->first->info.model);
		if (lista->first->info.numeSofer != NULL)
			free(lista->first->info.numeSofer);

		lista->first = lista->first->next;
		free(aux);
	}
	lista->last = NULL;
}

int main()
{
	ListaDubla coada = citireCoadaDeMasiniDinFisier("masini.txt");
	afisareCoadaDeMasini(coada);
	dezalocareCoadaDeMasini(&coada);
}