#include <stdio.h>
#include <stdlib.h>

struct Produs
{
	int id;
	int stoc;
	char* denumire; //ocupa in memorie 4/8 octeti, depinde de arhitecura, la noi e de 64 => 8 octeti
	float pret;
	char categorie;
};

struct Produs initializare(int idDat, int stocDat, const char* denumireData, float pretDat, char categorieData)
{
	struct Produs p;
	p.id = idDat;
	p.stoc = stocDat;
	//p.denumire=denumireData => shallow copy atribuie adresa
	p.denumire = (char*)malloc(sizeof(char) * (strlen(denumireData) + 1));	//cast explicit, transformam din void* in char *, 
	//se calculeaza exact dimens ocupata de ce o sa fie in denumireData inmultind dimens unui char cu cate char-uri sunt
	//p.denumire retine adresa de inceput a blocului alocat in heap => acum putem copia ce am primit in denumireData la zona alocata pe heap
	strcpy_s(p.denumire, strlen(denumireData) + 1, denumireData);
	p.pret = pretDat;
	p.categorie = categorieData;
	return p;
}

void afisare(struct Produs sursa)
{
	printf("Produsul cu id-ul %d, are denumirea %s, stocul: %d, pret: %5.2f si categoria: %c.\n", sursa.id, sursa.denumire, sursa.stoc, sursa.pret, sursa.categorie);
	//pe noul cadru de stiva, se rezerva spațiu fizic pentru o noua variabila locala numita sursa (care este de tip struct Produs).
	//Se copiaza automat fiecare octet din variabila originala prod1 (aflata în main) în noua structura sursa (aflata in afisare).
}

void modificaDenumire(struct Produs* sursa, char* denumireNoua)
{
	free((*sursa).denumire);
	sursa->denumire = (char*)malloc(sizeof(char) * (strlen(denumireNoua) + 1));
	strcpy_s(sursa->denumire, strlen(denumireNoua) + 1, denumireNoua);
}

int main()
{
	struct Produs prod1;
	prod1 = initializare(1, 20, "Telefon Smart", 2000, 'A'); // "" pune null la final, in timp ce '' nu pune, avand doar un caracter

	afisare(prod1);

	modificaDenumire(&prod1, "Tableta");
	afisare(prod1);


	return 0;
}