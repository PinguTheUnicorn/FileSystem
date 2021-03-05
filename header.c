#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "header.h"

// Functie de initializare lista
void create_fs(Directory **root)
{
	// Se aloca memorie pentru nodul initial
	*root = (Directory *) malloc(sizeof(Directory));
	(*root)->name = (char *) malloc(2 * sizeof(char));

	// Se denumeste nodul radacina
	strcpy((*root)->name, "/");

	// Toate legaturile nodului trimit catre NULL
	(*root)->parentDir = NULL;
	(*root)->files = NULL;
	(*root)->kidDir = NULL;
	(*root)->parentDir = NULL;
}

// Functie pentru afisarea elementelor
// dintr-un director
void ls(Directory **dir)
{
	// Se foloseste nod temporar pentru parcurgerea
	// elemetelor din directorul curent
	Directory *curentDir = *dir;

	// Se verifica daca exista fisiere in directorul curent
	if (curentDir->files != NULL) {
		// In caz afirmativ, se ia un pointer temporar
		// pentru parcurgerea fisierelor
		datas *curentFile = curentDir->files;

		// Se afiseaza numele fisierelor
		// cat timp mai exista fisiere neparcurse
		while (curentFile != NULL) {
			printf("%s ", ((File *) curentFile->data)->name);
			curentFile = curentFile->next;
		}
	}

	// Se verifica daca exista directoare in directorul curent
	// In caz afirmativ, se repeta procesul folosit anterior
	if (curentDir->kidDir != NULL) {
		datas *showDir = curentDir->kidDir;

		while (showDir != NULL) {
			printf("%s ", ((Directory *) showDir->data)->name);
			showDir = showDir->next;
		}
	}

	// La final, se trece pe randul urmator
	printf("\n");
}

// Functie ce compara numele a doua fisiere
int cmpF(datas *fis1, datas *fis2)
{
	File *cmp1, *cmp2;

	cmp1 = (File *)fis1->data;
	cmp2 = (File *)fis2->data;
	return strcmp(cmp1->name, cmp2->name);
}

// Functie de adaugare a unui fisier nou in directorul curent
void touch(Directory **dir, char *numefisier)
{
	numefisier = strtok(NULL, " ");
	// Se aloca memorie pentru noul fisier
	datas *new = (datas *) malloc(sizeof(datas));
	int lenght;

	new->data = (File *) malloc(sizeof(File));

	// Se copiaza numele fisierului in datele din nod
	lenght = strlen(numefisier) + 1;
	((File *) new->data)->name = (char *) malloc(lenght * sizeof(char));
	strcpy(((File *) new->data)->name, numefisier);

	// Se stocheaza datele fisierului
	numefisier = strtok(NULL, " ");
	lenght = strlen(numefisier) + 1;
	((File *) new->data)->size = lenght - 1;
	((File *) new->data)->data = (char *) malloc(lenght * sizeof(char));
	strcpy(((File *) new->data)->data, numefisier);

	// Se face legatura intre nodul unde este stocat fiserul
	// Si directorul in care se afla
	((File *) new->data)->dir = *dir;

	// Daca nu mai exista alt fisier salvat anterior
	// Se salveaza fiserul ca atare
	if ((*dir)->files == NULL) {
		(*dir)->files = new;
		new->last = NULL;
		new->next = NULL;
	} else {
		//In caz contrar, se cauta in lista
		// de fisere a diretorului curent
		// pentru a fi adaugat in ordine lexicografica
		datas *curfile = (*dir)->files;

		// Se verifica daca noul fisier trebuie adaugat la inceput
		if (cmpF(new, curfile) < 0) {
			new->last = NULL;
			new->next = curfile;
			curfile->last = new;
			(*dir)->files = new;
		} else {
			// In caz contrar, se cauta in lista de fisiere
			// Pana se ajunge la final
			while (curfile->next != NULL) {
				curfile = curfile->next;
				// Daca se gaseste o pozitie favorabila
				// Se opreste cautarea
				if (cmpF(new, curfile) < 0)
					break;
			}
			// Se verifica motivul opririi cautarii
			if (cmpF(new, curfile) < 0) {
				// Daca s-a gasit un fisier al carui nume
				// este "mai mare" lexicografic
				// decat al celui nou
				// se fac legaturile dintre nodul nou
				// cu cel anterior si cel curent
				datas *backfile = curfile->last;

				backfile->next = new;
				curfile->last = new;
				new->last = backfile;
				new->next = curfile;
			} else {
				// Daca s-a ajuns la sfarsitul listei
				// inseamna ca nu s-a gasit
				// un fisier cu nume "mai mare"
				// asa ca trebuie sa fie adaugat la sfarsit
				curfile->next = new;
				new->last = curfile;
				new->next = NULL;
			}
		}
	}
}

// Functie ce compara numele a doua directoare
int cmpD(datas *dir1, datas *dir2)
{
	Directory *cmp1, *cmp2;

	cmp1 = (Directory *)dir1->data;
	cmp2 = (Directory *)dir2->data;
	return strcmp(cmp1->name, cmp2->name);
}

// Functie de adaugare a unui director nou in directorul curent
void mkdir(Directory **dir, char *numedir)
{
	// Se aloca memorie pentru noul nod
	numedir = strtok(NULL, " ");
	datas *new = (datas *) malloc(sizeof(datas));
	int lenght;

	// Se aloca memorie pentru numele directorului
	// Apoi se denumeste
	new->data = (Directory *) malloc(sizeof(Directory));
	lenght = strlen(numedir) + 1;
	((Directory *) new->data)->name = (char *) malloc(lenght*sizeof(char));
	strcpy(((Directory *) new->data)->name, numedir);

	// Se face legatura cu directorul parinte
	// Se initializeaza listele pentru fisiere si directoare
	// ale noului director
	((Directory *) new->data)->files = NULL;
	((Directory *) new->data)->kidDir = NULL;
	((Directory *) new->data)->parentDir = *dir;

	// Daca nu exista alt director salvat anterior
	// se salveaz ca atare
	if ((*dir)->kidDir == NULL) {
		(*dir)->kidDir = new;
		new->last = NULL;
		new->next = NULL;
	} else {
		// Altfel, se parcurge lista de subdirectoare
		// a directorului curent pentru
		// a fi adaugat in ordine lexicografica
		datas *curdir = (*dir)->kidDir;

		// Se verifica daca noul director trebuie salvat la inceput
		if (cmpD(new, curdir) < 0) {
			new->last = NULL;
			new->next = curdir;
			curdir->last = new;
			(*dir)->kidDir = new;
		} else {
			// In caz contrar, se parcurge lista de directoare
			// pana se ajunge la final
			while (curdir->next != NULL) {
				curdir = curdir->next;
				// Daca se gaseste un director
				// cu numele "mai mare"
				// din punct de vedere lexicografic
				// se opreste cautarea
				if (cmpD(new, curdir) < 0)
					break;
			}
			// Se verifica motivul opririi
			if (cmpD(new, curdir) < 0) {
				// Daca s-a gasit un director al carui nume
				// este "mai mare" lexicografic
				// decat al celui nou
				// se fac legaturile nodului nou
				// cu cel anterior si cel curent
				datas *backdir = curdir->last;

				backdir->next = new;
				new->last = backdir;
				new->next = curdir;
				curdir->last = new;
			} else {
				// Daca s-a ajuns la sfarsitul listei
				// inseamna ca nu s-a gasit
				// un director cu nume "mai mare"
				// asa ca trebuie sa fie adaugat la sfarsit
				curdir->next = new;
				new->last = curdir;
				new->next = NULL;
			}
		}
	}
}

// Functie de comparare a numelui unui director cu un sir
int cmpCD(datas *dir, char *sir)
{
	Directory *cmp;

	cmp = (Directory *)dir->data;
	return strcmp(cmp->name, sir);
}

// Functie de afisare a mesajului de eroare pentru functia cd
void eroare_cd(char *sir)
{
	printf("Cannot move to '%s': No such directory!\n", sir);
}

// Functie de schimbare a directorului curent
void cd(Directory **dir, char *numedir)
{
	numedir = strtok(NULL, " ");
	// Se verifica daca se doreste mutarea in directorul parinte
	if (!strcmp(numedir, "..")) {
		// Caz in care se schimba directorul cu cel parinte
		*dir = (*dir)->parentDir;
	} else {
		// Altfel, se cauta in subdirectoarele directorului curent
		datas *chdir = (*dir)->kidDir;

		// Se parcurge lista pana se ajunge la final
		while (chdir != NULL) {
			// Daca se gaseste directorul, se opreste cautarea
			if (!cmpCD(chdir, numedir))
				break;
			chdir = chdir->next;
		}
		// Daca nu s-a gasit directorul, se afiseaza mesajul de eroare
		// si se iese din functie
		if (chdir == NULL)
			eroare_cd(numedir);
		else
			// Altfel, se schimba directorul curent cu cel gasit
			*dir = ((Directory *) chdir->data);
	}
}

// Functie de afisare a caii absolute pana la directorul curent
void pwd(Directory **dir)
{
	// Se verifica daca ne aflam in radacina sistemului de foisiere
	if (!strcmp((*dir)->name, "/"))
		//In caz afimativ, se afiseaza numele directorului
		printf("/\n");
	else {
		// Altfel, se parcurge lista in sens invers
		// de la directorul curent pana la root
		// Si se salveaza numele fiecarui director parcurs
		int lenght = strlen((*dir)->name) + 1;
		// Se aloca memorie pentru a se stoca path-ul
		// de la directorul root pana la cel curent
		char *path = (char *) malloc(lenght * sizeof(char));

		strcpy(path, (*dir)->name);

		Directory *curdir = (*dir)->parentDir;
		char *sirtmp;

		// La fiecare director, se salveaza numele acestuia
		// si se adauga la inceputul sirului unde este salvat path-ul
		// impreuna cu delimitatorii necesari
		while (curdir->parentDir != NULL) {
			lenght = lenght + strlen((*dir)->name) + 1;
			sirtmp = (char *) malloc(lenght * sizeof(char));
			strcpy(sirtmp, curdir->name);
			strcat(sirtmp, "/");
			strcat(sirtmp, path);
			path = (char *) realloc(path, lenght * sizeof(char));
			strcpy(path, sirtmp);
			free(sirtmp);
			curdir = curdir->parentDir;
		}
		// La final, se afiseaza path-ul
		// si se elibereaza memoria alocata
		printf("/%s\n", path);
		free(path);
	}
}

// Functie pentru afisarea continutului sistemului de fisiere
void tree(Directory *originaldir, int n)
{
	// Se iau listele de fisiere si de subdirectoare ale directorului curent
	datas *fis = originaldir->files;
	datas *dir = originaldir->kidDir;
	int i;

	// Daca suntem la primul nivel de afisare
	// se va printa numele directorului
	// din care s-a apelat functia
	if (n == 1)
		printf("%s\n", originaldir->name);

	// Se parcurge lista de fisiere
	while (fis != NULL) {
		// Pentru numele fiecarui fisier
		// se va face indentarea necesara nivelului
		for (i = 0; i < n; i++)
			printf("    ");

		// Dupa aceea se va afisa numele fisierului
		printf("%s\n", ((File *)fis->data)->name);
		fis = fis->next;
	}

	// Se parcurge lista de directoare
	while (dir != NULL) {
		// Pentru numele fiecarui director
		// se va face indentarea necesara
		for (i = 0; i < n; i++)
			printf("    ");

		// Dupa aceea se va afisa numele diectorului
		printf("%s\n", ((Directory *)dir->data)->name);

		// Pentru a se trece a urmatorul nivel
		// se reapeleaza functia
		// pentru a afisa si continutul subdirectoarelor
		tree(dir->data, n + 1);
		dir = dir->next;
	}
}

// Functie pentru a compara numele unui fisier cu un sir
int cmpRM(datas *fis, char *sir)
{
	File *cmp = fis->data;

	return strcmp(cmp->name, sir);
}

// Functie pentru eliberarea memoriei alocate unui fisier
void freefile(datas **fis)
{
	free(((File *)(*fis)->data)->name);
	free(((File *)(*fis)->data)->data);
	free((*fis)->data);
	free(*fis);
}

// Functie de afisare a mesajului de eroare a functiei rm
void eroare_rm(char *sir)
{
	printf("Cannot remove '%s': No such file!\n", sir);
}

// Functie de stergere a unui fisier
void rm(Directory **dir, char *numefis)
{
	numefis = strtok(NULL, " ");

	// Daca nu exista fisiere in directorul curent
	// Sa afiseaza mesajul de eroare si se iese din functie
	if ((*dir)->files == NULL) {
		eroare_rm(numefis);
		return;
	}

	// Daca lista nu e goala
	// aceasta este parcursa
	datas *delete = (*dir)->files;

	// Se verifica daca primul element este cel cautat
	if (!cmpRM(delete, numefis)) {
		// In caz afirmativ, se verific daca e singurul nod existent
		if (delete->next == NULL) {
			// Caz in care lista devine goala
			// si se elibereaza memoria pentru fisierul respectiv
			(*dir)->files = NULL;
			freefile(&delete);
		} else {
			//Daca lista nu este goala
			// se elimina nodul curent
			datas *next = delete->next;

			(*dir)->files = next;
			next->last = NULL;

			// Dupa ce s-a eliminat nodul
			// este delocata memoria fisierului
			freefile(&delete);
		}
	} else {
		// Daca fisierul cautat nu este primul in lista
		// se continua cautarea in lista
		while (delete != NULL) {
			// Daca se gaseste fisierul
			if (!cmpRM(delete, numefis)) {
				// se verifica daca acesta este la final
				// caz in care se elimina din lista
				// in modul corespunzator
				if (delete->next == NULL) {
					datas *back = delete->last;

					back->next = NULL;
				} else {
					// Daca nodul nu se afla
					// in capetele listei de fisiere
					// se fac eliminarile
					// si legaturile corespunzatoare
					datas *back, *next;

					back = delete->last;
					next = delete->next;
					back->next = next;
					next->last = back;
				}

				// Dupa ce s-a eliminat nodul din lista
				// se elibereaza memoria fisierului
				// si se iesei din functie
				freefile(&delete);
				return;
			}
		delete = delete->next;
		}

		// Daca nu s-a gasit fisierul in directorul dat
		// se va afisa mesajul de eroare
		eroare_rm(numefis);
	}
}

// Functie pentru comparat numele unui director cu un sir
int cmpRMDIR(datas *dir, char *sir)
{
	Directory *cmp = dir->data;

	return strcmp(cmp->name, sir);
}

// Functie pentru afisat mesajul de eroare la functia rmdir
void eroare_rmdir(char *sir)
{
	printf("Cannot remove '%s': No such directory!\n", sir);
}

// Functie ce elimina un nod din lista de directoare
void izolare(Directory **origin, datas *del)
{
	// Daca nodul este singurul din lista
	if (del->last == NULL && del->next == NULL)
		// Se goleste lista
		(*origin)->kidDir = NULL;

	// Altfel, daca este primul element
	else if (del->last == NULL) {
		// Se salveaza urmatorul element
		// drept capul listei de fisiere
		datas *next = del->next;

		(*origin)->kidDir = next;
		next->last = NULL;
	} else if (del->next == NULL) {
		// Daca este ultimul element
		// se "rupe" legatura
		// dintre nodul care trebuie izolat
		// si cel de dinaintea lui
		datas *back = del->last;

		back->next = NULL;
	} else {
		// In cazul in care nodul se afla la mijloc
		// se creaza legaturile necesare
		// intre nodurile adiacente
		// nodului ce va fieliminat
		datas *back = del->last;
		datas *next = del->next;

		back->next = next;
		next->last = back;
	}
}

// Functie pentru eliberarea memoriei tuturor fisierelor dintr-un director
void dir_file(datas **file)
{
	datas *curfile;

	while (*file != NULL) {
		curfile = *file;
		*file = (*file)->next;
		free(((File *)curfile->data)->name);
		free(((File *)curfile->data)->data);
		free(curfile->data);
		free(curfile);
	}
}

// Functie pentru eliminarea unui director si a continutului acestuia
void rmdir(Directory **dir, char *numedir, int decizie)
{
	// Se verifica daca sirul numedir dat ca parametru
	// contine sau nu numele directorului cautat
	// si daca trebuie sa se faca vreo schimbare asupra acestuia
	if (decizie)
		numedir = strtok(NULL, " ");

	// Se verifica daca exista subdirectoare
	// Daca nu, se afiseaza un mesaj de eroare
	// si se iese din functie
	if ((*dir)->kidDir == NULL) {
		eroare_rmdir(numedir);
		return;
	}

	datas *delete = (*dir)->kidDir;

	// Daca exista, atunci se cauta subdirectorul in lista de directoare
	while (delete != NULL) {
		// Daca il gaseste, se opreste cautarea
		if (!cmpRMDIR(delete, numedir))
			break;
		delete = delete->next;
	}
	// Se verifica daca subdirectorul s-a gasit
	if (delete == NULL) {
		// In caz contrar, se afiseaza un mesaj de eroare
		eroare_rmdir(numedir);
		return;
	}
	datas *file = ((Directory *)delete->data)->files;

	// Daca s-a gasit, se elimina fisierele acestuia
	dir_file(&file);
	((Directory *)delete->data)->files = NULL;
	datas *kid = ((Directory *)delete->data)->kidDir;
	Directory *curkid, *deldir = delete->data;

	// Apoi se ia fiecare subdirector in parte
	while (kid != NULL) {
		// Si se repeta procesul pentru acesta
		// pana nu mai exista subdirectoare
		// in directorul ce trebuie eliminat
		curkid = kid->data;
		kid = kid->next;
		rmdir(&deldir, curkid->name, 0);
	}

	// Dupa ce nu mai sunt subdirectoare de sters
	// se elimina nodul din lista de directoare initiala
	izolare(dir, delete);
	// Dupa aceea se elibereaza memoria alocata nodului
	free(((Directory *)delete->data)->name);
	free(delete->data);
	free(delete);
}

// Functie pentru cautarea unui fisier
void find(Directory *dir, int depth, int dmax, int min, int max, char *search)
{
	Directory *curdir = dir;
	datas *files = curdir->files;

	// Se verifica lista de fisiere din directorul curent
	while (files != NULL) {
		int size = ((File *)files->data)->size;
		int lengh = 1 + strlen(((File *)files->data)->data);
		char *data = (char *)malloc(lengh * sizeof(char));

		strcpy(data, ((File *)files->data)->data);

		// Se verifica daca fisierul actual are dimensiunea potrivita
		// si daca se gaseste in acesta continutul cautat
		if (size <= max && size >= min && strstr(data, search) != NULL)
			// In caz afirmativ, se afiseaza numele fisierului
			printf("%s ", ((File *)files->data)->name);

		files = files->next;
	}

	// Se verifica daca s-a atins adancimea maxima
	if (depth < dmax) {
		datas *kid = curdir->kidDir;

		// In caz afirmativ, se repeta procesul
		// pentru subdirectoarele existente
		while (kid != NULL) {
			Directory *cauta = kid->data;

			find(cauta, depth + 1, dmax, min, max, search);
			kid = kid->next;
		}
	}

	// Daca s-a atins adancimea maxima
	// inseamna ca s-a terminat cautarea
	// si se trece pe randul urmator
	if (!depth)
		printf("\n");
}

// Functie pentru eliberarea memoriei listei
void delete_fs(Directory **root)
{
	// Se elimina memoria alocata numelui si nodului root
	free((*root)->name);
	free(*root);
}
