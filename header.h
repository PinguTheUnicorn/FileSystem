#ifndef header
#define A

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

void create_fs(Directory**root);

void ls(Directory **dir);

int cmpF(datas *fis1, datas *fis2);

void touch(Directory **dir, char *numefisier);

int cmpD(datas *dir1, datas *dir2);

void mkdir(Directory **dir, char *numedir);

int cmpCD(datas *dir, char *sir);

void eroare_cd(char *sir);

void cd(Directory **dir, char *numedir);

void pwd(Directory **dir);

void tree(Directory *originaldir, int n);

int cmpRM(datas *fis, char *sir);

void freefile(datas **fis);

void eroare_rm(char *sir);

void rm(Directory **dir, char *numefis);

void eroare_rmdir(char *sir);

void izolare(Directory **origin, datas *del);

void dir_file(datas **file);

void rmdir(Directory **dir, char *numedir, int decizie);

void find(Directory *dir, int depth, int dmax, int min, int max, char *search);

void delete_fs(Directory **root);

#endif
