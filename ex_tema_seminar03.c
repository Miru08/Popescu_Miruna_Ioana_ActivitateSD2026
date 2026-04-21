#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DetaliiConsultatii
{
	int id;
	float pret;
	char* numePacient;
	char* doctor;
	char cabinet;
};

typedef struct DetaliiConsultatii Consultatie;

void afisareConsultatie(Consultatie sursa)
{
	printf("Id: %d || Pret: %5.2f || Nume pacient: %s || Doctor: %s || Cabinet: %c.\n", sursa.id, sursa.pret, sursa.numePacient, sursa.doctor, sursa.cabinet);
}

void afisareVectorConsultatii(Consultatie* vector, int nrConsultatii)
{
	if (vector == NULL || nrConsultatii <= 0)
	{
		printf("Vectorul este gol ! :( \n");
		return;
	}
	for (int i = 0; i < nrConsultatii; i++)
	{
		afisareConsultatie(vector[i]);
	}
}

void adaugaConsultatieInVector(Consultatie** vectorConsultatii, int* nrConsultatii, Consultatie consultatiaCitita)
{
	Consultatie* temp = (Consultatie*)malloc(sizeof(Consultatie) * (*nrConsultatii + 1));
	for (int i = 0; i < (*nrConsultatii); i++)
	{
		temp[i] = (*vectorConsultatii)[i];
	}
	temp[(*nrConsultatii)] = consultatiaCitita;
	free(*vectorConsultatii);
	*vectorConsultatii = temp;
	(*nrConsultatii)++;
}

Consultatie citireConsultatieDinFisier(FILE* file)
{
	Consultatie temp;
	char buffer[100];//citim cate o linie si o retin in buffer
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	temp.id = atoi(strtok(buffer, sep));
	temp.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	temp.numePacient = (char*)malloc(sizeof(char) * strlen(aux) + 1);
	strcpy_s(temp.numePacient, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	temp.doctor = (char*)malloc(sizeof(char) * strlen(aux) + 1);
	strcpy_s(temp.doctor, strlen(aux) + 1, aux);
	temp.cabinet = strtok(NULL, sep)[0];
	return temp;
}

Consultatie* citireVectorConsultatiiDinFisier(const char* numeFisier, int* nrConsultatii)
{
	FILE* file = fopen(numeFisier, "r");
	Consultatie* vectorConsultatii = NULL;

	if (file == NULL)
	{
		printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
		return NULL;
	}
	else
	{
		*nrConsultatii = 0;
		while (!feof(file))
		{
			Consultatie consCitita = citireConsultatieDinFisier(file);
			adaugaConsultatieInVector(&vectorConsultatii, nrConsultatii, consCitita);
		}
	}
	fclose(file);
	return vectorConsultatii;
}

void dezalocare(Consultatie** vectorConsultatii, int* nrConsultatii)
{
	for (int i = 0; i < (*nrConsultatii); i++)
	{
		if ((*vectorConsultatii)[i].numePacient != NULL)
			free((*vectorConsultatii)[i].numePacient);
		if ((*vectorConsultatii)[i].doctor != NULL)
			free((*vectorConsultatii)[i].doctor);
	}
	free((*vectorConsultatii));
	*vectorConsultatii = NULL;
	*nrConsultatii = 0;
}

int main()
{
	Consultatie* vectorConsultatii = NULL;
	int nrConsultatii = 0;
	vectorConsultatii = citireVectorConsultatiiDinFisier("consultatii.txt", &nrConsultatii);
	afisareVectorConsultatii(vectorConsultatii, nrConsultatii);
	dezalocare(&vectorConsultatii, &nrConsultatii);

}