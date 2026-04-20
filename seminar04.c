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

// creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod
{
	Masina info;
	struct Nod* next;
};

typedef struct Nod Nod;

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
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

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

void afisareListaMasini(Nod* cap)
{
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaLaFinalInLista(Nod** cap, Masina masinaNoua)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod)); //retine adresa nodului care v-a fi adaugat la finalul listei
	nou->info = masinaNoua;
	nou->next = NULL;
	//s-a creat nodul care trebuie sa fie adaugat la finalul listei 

	if (*cap == NULL)
	{
		*cap = nou;
	}
	else
	{
		Nod* p = *cap; //facem o copie a primei adrese deoarece trebuie sa parcurgem lista pana la final pentru a 
		//adauga nodul nou si sa retinem adresa ultimuli nod si nu ne mai putem intoarce la prima adresa( nu putem merge inapoi in lista)
		while (p->next != NULL)
		{
			p = p->next; //deplasam adresa catre urmatorul nod pana ajungem la finalul listei
		}
		p->next = nou; // punem adresa nodului nou la finalul listei
	}
}

void adaugaLaInceputInList(Nod** cap, Masina masinaNoua)
{
	// adauga la inceputul listei o noua masina pe care o primim ca parametru

	//primul pas cream nodul care va fi adaugat la inceputul listei
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *cap; // noul nod va pointa catre primul nod din lista 
	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier)
{
	Nod* cap = NULL;

	FILE* file = fopen(numeFisier, "r");
	if (file == NULL)
	{
		printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
		return NULL;
	}
	else
	{
		while (!feof(file))
		{
			adaugaMasinaLaFinalInLista(&cap, citireMasinaDinFisier(file));
		}
	}
	fclose(file);
	return cap;
}

void dezalocareListaMasini(Nod** cap)
{
	while (*cap)
	{
		//cu p ma deplasez pozitie cu pozitie eliberez char-urile 
		//cu c ma mut mereu cu o pozitie inainte pentru a nu pierde adresa primului nod cand sterg cu p 
		Nod* p = *cap;
		(*cap) = p->next;
		if (p->info.model)
		{
			free(p->info.model);
		}
		if (p->info.numeSofer)
		{
			free(p->info.numeSofer);
		}
		free(p);

	}
}

float calculeazaPretMediu(Nod* cap)
{
	// calculeaza pretul mediu al masinilor din lista.
	float suma = 0;
	int contor = 0;
	while (cap != NULL)
	{
		suma += cap->info.pret;
		contor++;
		cap = cap->next;
	}
	if (contor > 0)
		return suma / contor;
	else
		return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata)
{
	while ((*cap) && (*cap)->info.serie == serieCautata)
	{
		Nod* aux = *cap;
		(*cap) = aux->next;
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
	}
	if ((*cap))
	{
		Nod* p = *cap;
		while (p)
		{
			while (p->next && p->next->info.serie != serieCautata)
			{
				p = p->next;
			}
			if (p->next)
			{
				Nod* aux = p->next;
				p->next = aux->next;
				if (aux->info.numeSofer)
				{
					free(aux->info.numeSofer);
				}
				if (aux->info.model)
				{
					free(aux->info.model);
				}
				free(aux);
			}
			else
			{
				p = NULL;
			}
		}
	}

}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer)
{
	// calculeaza pretul tuturor masinilor unui sofer.
	float pret = 0;
	while (cap != NULL)
	{
		if (strcmp(cap->info.numeSofer, numeSofer) == 0)
			pret += cap->info.pret;
		cap = cap->next;
	}
	return pret;
}

int main()
{
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);
	printf("Pretul mediu este: %.2f\n", calculeazaPretMediu(cap));
	printf("Pretul masinilor soferului %s este %.2f \n", "Ionescu", calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));

	printf("\nStergere seria A\n");
	stergeMasiniDinSeria(&cap, 'A');
	afisareListaMasini(cap);
	printf("\nStergere seria B\n");
	stergeMasiniDinSeria(&cap, 'B');
	afisareListaMasini(cap);

	dezalocareListaMasini(&cap);
	return 0;
}