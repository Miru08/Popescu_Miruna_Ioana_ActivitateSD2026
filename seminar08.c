#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

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

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap
{
	int lungime;
	int nrMasini;
	Masina* vector;
};

typedef struct Heap Heap;

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

void afisareMasina(Masina masina)
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Heap initializare(int lungimeData)
{
	//initializeaza heap-ul cu 0 elemente 
	//dar cu o lungime primita ca parametru
	Heap heap;
	heap.lungime = lungimeData;
	heap.nrMasini = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungimeData);
	return heap;
}

void filtreaza(Heap heap, int pozitieNod)
{
	//primeste heap prin valoare, se face o copie a structurii Heap pe stiva filtreaza(), nu trebuie cu * in antet  pt ca adresa 
	//vectorului de Masini este copiata si aici, pe stiva filtreaza(), aceeasi ca cea din main() si se pot face modificari direct

	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod; //presupunem ca nodul cel mai mare se afla pe pozitia data ca param

	//AICI PRIORITIZAREA SE FACE DUPA ID
	if (pozFiuSt < heap.nrMasini && heap.vector[pozMax].id < heap.vector[pozFiuSt].id)
	{
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrMasini && heap.vector[pozMax].id < heap.vector[pozFiuDr].id)
	{
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod)
	{
		Masina aux;
		aux = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = heap.vector[pozMax];
		heap.vector[pozMax] = aux;

		//este noul nod mutat pe poxMax nod frunza ? ( verificam cu indici)
		//daca raspunsul este da nu mai intra in if( cazul cand pozMax este <= )
		if (pozMax <= (heap.nrMasini - 2) / 2)
		{
			//daca nu este frunza, iar nodul cu indicele pozMax are fii se filtreaza iar
			filtreaza(heap, pozMax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");

	Heap heapCreat = initializare(10);
	if (file == NULL)
	{
		printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
		return;
	}
	else
	{
		while (!feof(file))
		{
			heapCreat.vector[heapCreat.nrMasini] = citireMasinaDinFisier(file);
			heapCreat.nrMasini++;
		}

		for (int i = (heapCreat.nrMasini - 2) / 2; i >= 0; i--)
		{
			filtreaza(heapCreat, i);
		}

	}
	fclose(file);
	return heapCreat;
}

void afisareHeap(Heap heapDat)
{
	for (int i = 0; i < heapDat.nrMasini; i++)
		afisareMasina(heapDat.vector[i]);
}

void afisareHeapAsuns(Heap heap)
{
	for (int i = heap.nrMasini; i < heap.lungime; i++)
		afisareMasina(heap.vector[i]);
}

Masina extrageMasinaDinHeap(Heap* heap)
{
	if (heap->nrMasini > 0)
	{
		Masina aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrMasini - 1];
		heap->vector[heap->nrMasini - 1] = aux;
		heap->nrMasini--;

		for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--)
			filtreaza(*heap, i);

		//filtreazaHeap(*heap, i);

		return aux;// atentie se face shallow copy 
	}
}

void dezalocare(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		if (heap->vector[i].model != NULL)
			free(heap->vector[i].model);
		if (heap->vector[i].numeSofer != NULL)
			free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->nrMasini = 0;
	heap->lungime = 0;
}

int main()
{
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");

	printf("Heap dupa filtrare: \n");
	afisareHeap(heap);

	printf("Masina extrasa: \n");
	afisareMasina(extrageMasinaDinHeap(&heap));

	printf("Heap dupa extragere: \n");
	afisareHeap(heap);

	printf("Heap-ul ascuns: \n");
	afisareHeapAsuns(heap);

	dezalocare(&heap);
	return 0;
}