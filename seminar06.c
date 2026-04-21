#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Stiva = structura de date liniara, omogena, LIFO => datele sunt asezate una dupa alta, in linie, si toate sunt de acelasi tip
// Coada -> FIFO = structura de date, omogena, liniara, FIFO 

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
	Masina masinaReturnata;
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	masinaReturnata.id = atoi(strtok(buffer, sep));
	masinaReturnata.nrUsi = atoi(strtok(NULL, sep));
	masinaReturnata.pret = atof(strtok(NULL, sep));

	char* aux;

	aux = strtok(NULL, sep);
	masinaReturnata.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(masinaReturnata.model, aux);

	aux = strtok(NULL, sep);
	masinaReturnata.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(masinaReturnata.numeSofer, aux);

	masinaReturnata.serie = strtok(NULL, sep)[0];
	//masinaReturnata.serie = *strtok(NULL, sep); //dereferentiaza si ia fix adresa primului caracter 

	return masinaReturnata;
}

void afisareMasina(Masina masina)
{
	printf("Id: %d || Nr usi: %d || Pret: %5.2f || Model: %s || Nume sofer: %s || Serie: %c.\n", masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}


//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

//LSI
typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
};


void afisareStackMasini(Nod* cap)
{
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void pushStack(Nod** cap, Masina masinaAdaugata)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaAdaugata;
	nou->next = *cap;
	*cap = nou;
}

Masina popStack(Nod** cap)
{
	// Verificam daca avem ce sa scoatem(daca stiva nu e goala)
	if (*cap != NULL)
	{
		Masina masinaScoasa = (*cap)->info;
		Nod* aux = *cap;//daca nu-l folosim nu stim la ce sa dam free deoarece cap trb sa retin MEREU adresa varfului
		// Elementul de dedesubt devine acum noul varf oficial al stivei.
		(*cap) = (*cap)->next;
		//Acum distrugem vechea cutie din varf.
		free(aux);
		return masinaScoasa;
	}
	else
	{
		// daca stiva e goala returnam o masina "falsa" 
		Masina noua;
		noua.id = -1;
		return noua;
	}
}

Nod* citireStckMasinaDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Nod* stackMasini = NULL;
	if (file == NULL)
	{
		printf("Nu s-a putut deschide fisierul %s.\n", numeFisier);
		return NULL;
	}
	else
	{
		while (!feof(file))
		{
			Masina masinaCitita = citireMasinaDinFisier(file);
			pushStack(&stackMasini, masinaCitita);
		}
		fclose(file);
	}
	return stackMasini;
}

int isEmptyStack(Nod* cap)
{
	return cap == NULL;
}

int size(Nod* cap)
{
	int nr = 0;
	while (cap != NULL)
	{
		nr++;
		cap = cap->next;
	}
	return nr;
}

void dezalocareStivaDeMasini(Nod** cap)
{
	Nod* p = NULL;
	while ((*cap) != NULL)
	{
		p = (*cap);
		(*cap) = (*cap)->next;
		if (p->info.model != NULL)
			free(p->info.model);
		if (p->info.numeSofer != NULL)
			free(p->info.numeSofer);
		free(p);
	}
}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

typedef struct NodCoad NodCoada;
typedef struct ListaDubla ListaDubla;

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


int main()
{
	Nod* stackMasini = citireStckMasinaDinFisier("masini.txt");
	afisareStackMasini(stackMasini);
	printf("\nE stiva goala ? %d\n", isEmptyStack(stackMasini));
	printf("\nDimensiunea stivei: %d\n", size(stackMasini));
	dezalocareStivaDeMasini(&stackMasini);
}