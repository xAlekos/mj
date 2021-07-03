#ifndef GRIGLIA_H
#define GRIGLIA_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct nodo{
    char nome[20];
    int punteggio;
    struct nodo* next;
    struct nodo* prec;
};

typedef struct nodo* classifica;

int InsOrdinato(classifica* head,char nome[20],int punteggio);

void CreaFile();

void stampasufile(char nome[20], int punteggio);

int riempiclassifica(classifica* head);

void salvasuclassifica(int punti,classifica* head);

int stampaclassifica(classifica head);

#endif
