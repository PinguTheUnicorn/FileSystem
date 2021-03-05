#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "header.h"

int main(void)
{
	Directory *Dir;

	// Aplicatia functioneaza pana este data comanda de iesire
	while (1) {
		char *input = (char *) malloc(100 * sizeof(char));

		// Se preia comanda de la tastatura
		fgets(input, 100, stdin);

		// Se verifica daca s-a inregistrat din greseala
		// un separator de randuri la finalul sirului
		// caz in care se elimina
		if (strchr(input, '\n') != NULL)
			input[strchr(input, '\n') - input] = '\0';

		// Se verifica ce comanda este apelata
		// Dupa ce se indeplineste
		// se trece la inregistrarea urmatoarei comenzi
		if (!strcmp(input, "create fs")) {
			// Daca s-a apelat comanda "create fs"
			// Se apeleaza functia cu acelasi nume
			create_fs(&Dir);
			free(input);
			continue;
		}

		if (!strcmp(input, "delete fs")) {
			// Daca s-a apelat comanda "delete fs"
			// Se apelaza functia, apoi se opreste programul
			delete_fs(&Dir);
			free(input);
			return 0;
		}

		if (!strcmp(input, "pwd")) {
			// Daca s-a apelat comanda "pwd"
			// se va afisa calea absoluta pana la directorul curent
			pwd(&Dir);
			free(input);
			continue;
		}

		if (!strcmp(input, "tree")) {
			// Daca s-a apelat comanda "tree"
			// se va afisa pe ecran
			// intreg continutul directorului curent
			tree(Dir, 1);
			free(input);
			continue;
		}

		if (!strcmp("touch", strtok(input, " "))) {
			// Daca s-a apelat comanda "touch"
			// se va crea un fisier
			touch(&Dir, input);
			free(input);
			continue;
		}

		if (!strcmp(input, "rm")) {
			// Daca s-a apelat comanda "rm"
			// se va sterge fisierul dat ca argument
			// daca acesta exista
			rm(&Dir, input);
			free(input);
			continue;
		}

		if (!strcmp(input, "mkdir")) {
			// Daca s-a apelat comanda "mkdir"
			// se va crea un director
			mkdir(&Dir, input);
			free(input);
			continue;
		}

		if (!strcmp(input, "rmdir")) {
			// Daca s-a apelat comanda "rmdir"
			// se va sterge subdirectorul dat ca argument
			// si tot continutul acestuia
			// daca acesta exista
			rmdir(&Dir, input, 1);
			free(input);
			continue;
		}

		if (!strcmp(input, "cd")) {
			// Daca s-a apelat comanda "cd"
			// se schimba directorul curent
			// cu cel dat ca argument
			// daca acesta exista
			cd(&Dir, input);
			free(input);
			continue;
		}

		if (!strcmp(input, "ls")) {
			// Daca s-a apelat comanda "ls"
			// se ca afisa continutul directorului curent
			ls(&Dir);
			free(input);
			continue;
		}

		if (!strcmp(input, "find")) {
			// Daca s-a apelat comanda "find"
			// se vor afisa toate fisierele corespunzatoare
			int depth, min, max;

			// Se iau argumentele
			// si se salveaza fiecare intr-o variabila adecvata
			input = strtok(NULL, " ");
			depth = atoi(input);
			input = strtok(NULL, " ");
			min = atoi(input);
			input = strtok(NULL, " ");
			max = atoi(input);
			input = strtok(NULL, " ");
			find(Dir, 0, depth, min, max, input);
		}
	}
}
