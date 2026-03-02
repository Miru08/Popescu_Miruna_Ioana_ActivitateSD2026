#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//[] index face deplasare si dereferentiere v[i] = *(v + i)
//-> dereferentiere + accesare ( accesare ca . )
//vectorul este o colectie de elemente de acelasi tip care vor ocupa o zona de memorie continua si pe care putem folosi indexarea in mod direct

struct Biblioteca
{
	int id;
	int nrCarti;
	char* numeBiblioteca;
	float suprafata;
	char sector; //retine un simbol sau o valoare numerica pana la 127 pt ca primul bit e de semn
};

struct Biblioteca initializare(int idDat, int nrCartiDate, const char* numeBibliotecaDat, float suprafataData, char sectorDat)
{
	struct Biblioteca b;
	b.id = idDat;
	b.nrCarti = nrCartiDate;
	b.numeBiblioteca = (char*)malloc(sizeof(char) * (strlen(numeBibliotecaDat) + 1));
	strcpy_s(b.numeBiblioteca, strlen(numeBibliotecaDat) + 1, numeBibliotecaDat);
	b.suprafata = suprafataData;
	b.sector = sectorDat;
	return b;
}

struct Biblioteca copiazaBiblioteca(struct Biblioteca sursa)
{
	return initializare(sursa.id, sursa.nrCarti, sursa.numeBiblioteca, sursa.suprafata, sursa.sector);
}


void afisare(struct Biblioteca sursa)
{
	printf("ID-ul: %d\t\n Numarul de carti: %d\n Denumire: %s\n Suprafata: %5.2f\n Sector: %d\n", sursa.id, sursa.nrCarti, sursa.numeBiblioteca, sursa.suprafata, sursa.sector);
}

void afisareVector(struct Biblioteca* vector, int nrElemente)
{
	for (int i = 0; i < nrElemente; i++)
	{
		afisare(vector[i]);
	}
}

struct Biblioteca* copiazaPrimeleNElemente(struct Biblioteca* vector, int nrElemente, int nrElementeCopiate)
{
	//copiem intr-un vector nou, pe care il vom returna, primele nrElementeCopiate
	struct Biblioteca* vectorNou = NULL;
	if (nrElementeCopiate > nrElemente)
	{
		nrElementeCopiate = nrElemente;
	}
	vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++)
	{
		vectorNou[i] = copiazaBiblioteca(vector[i]);
	}
	return vectorNou;
}
void dezalocare(struct Biblioteca** vector, int* nrElemente)
{
	//dezalocam elementele din vector si vectorul
	for (int i = 0; i < (*nrElemente); i++)
	{
		free((*vector)[i].numeBiblioteca);
	}
	free((*vector));
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaBibliotecileCuSuprafatMare(struct Biblioteca* vector, int nrElemente, float suprafataMinima, struct Biblioteca** vectorNou, int* dimensiune)
{
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].suprafata > suprafataMinima)
			(*dimensiune)++;
	}
	*vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * (*dimensiune));

	int contor = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].suprafata > suprafataMinima)
		{
			(*vectorNou)[contor++] = copiazaBiblioteca(vector[i]);
		}
	}
}

struct Biblioteca getPrimulElementConditionat(struct Biblioteca* vector, int nrElemente, const char* denumire)
{
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Biblioteca s;
	for (int i = 0; i < nrElemente; i++)
	{
		if (strcmp(vector[i].numeBiblioteca, denumire) == 0)
		{
			s = copiazaBiblioteca(vector[i]);
			return s;
		}
	}
	return initializare(-1, 0, "Nu a fost gasit", 0.0f, '-');
}



int main()
{
	struct Biblioteca b;
	b = initializare(1, 431, "Biblioteca Mea", 482.95, 5);

	int numarBiblioteci = 5;
	struct Biblioteca* vectorBiblioteci = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * numarBiblioteci);

	vectorBiblioteci[0] = b; // shallow copy
	vectorBiblioteci[1] = initializare(2, 6, "Apropo", 13, 1);
	vectorBiblioteci[2] = initializare(3, 127, "Creanga", 938, 2);
	vectorBiblioteci[3] = initializare(4, 921, "Slavici", 210, 5);
	vectorBiblioteci[4] = initializare(5, 73, "Mihai Viteazu", 97, 2);

	afisareVector(vectorBiblioteci, numarBiblioteci);
	int nrCopiate = 3;
	struct Biblioteca* copie = copiazaPrimeleNElemente(vectorBiblioteci, numarBiblioteci, nrCopiate);
	printf("\nElementele vectorului cu primele %d elemente copiate: \n", nrCopiate);
	afisareVector(copie, nrCopiate);

	dezalocare(&copie, &nrCopiate);

	struct Biblioteca* vBiblioteciCuSuprafataMare = NULL;
	int nrBibliotecaCuSuprafataMare = 0;
	copiazaBibliotecileCuSuprafatMare(vectorBiblioteci, numarBiblioteci, 100, &vBiblioteciCuSuprafataMare, &nrBibliotecaCuSuprafataMare);

	printf("\nElementele vectorului cu biblioteci mari: \n");
	afisareVector(vBiblioteciCuSuprafataMare, nrBibliotecaCuSuprafataMare);
	dezalocare(&vBiblioteciCuSuprafataMare, &nrBibliotecaCuSuprafataMare);

	printf("\nPrimul element care indeplineste conditia: \n");
	struct Biblioteca elementGasit = getPrimulElementConditionat(vectorBiblioteci, numarBiblioteci, "Slavici");
	afisare(elementGasit);
	free(elementGasit.numeBiblioteca);

	dezalocare(&vectorBiblioteci, &numarBiblioteci);

	return 0;
}