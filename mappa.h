#ifndef MAPPA_H
#define MAPPA_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct casella{
    int stato; //0 = vuoto ,1= player ,2 = nemico, 3 = proiettile , 4= muro oriz 5 = muro vert, 6= vita, 7= ammo, 8= dmg
    char sprite;
    struct entity* pos_hold;
    int x,y;
};



struct righe{
    struct colonne* colonne;
    int i;
    struct righe* next;
    struct righe* prec;
};

struct colonne{
    struct casella casella;
    int j;
    struct colonne* north;
    struct colonne* south;
    struct colonne* east;
    struct colonne* west;
};

typedef struct righe* mappa;

void ClearScn();

int creamappa(mappa* head);

int addrow(mappa head);

int addcol(mappa head);

void riempimappa(mappa map,int i,int j);

int concatena_riga(mappa lista, mappa riga, int i);

int RowCount(mappa map);

int ColCount(mappa map);

void concatena_liste(mappa map);

void RiempiCaselle(mappa map);

void refreshsprites(mappa map);

void stampamappa(mappa map);

void inizializza_mappa(mappa griglia, int i, int j);

void refreshmap(mappa map);
#endif
