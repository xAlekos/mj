#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "classifica.h"
#include "entity.h"
#include "mappa.h"


void istruzioni(){
    printf("\n\n\t\tCOMANDI\n\n W: MUOVI SU \t\t T: SPARA SU \n A: MUOVI SINISTRA \t F: SPARA SINISTRA \n S: MUOVI GIU' \t\t G: SPARA GIU' \n D: MUOVI DESTRA \t H: SPARA DESTRA\n"); 
    printf("\n\n\t\tITEMS\n\n +: VITA EXTRA \n A: +10 MUNIZIONI \n S: DANNO AUMENTATO \n");  
    printf("\n\n Premi un qualsiasi tasto per continuare\n");
    getchar();
    while ( getchar() != '\n' );
    ClearScn();
}

void freemap(mappa head){
   struct colonne* column=head->colonne;
   struct righe* tmp1;
   struct colonne* tmp2;
    while(head!=NULL){
        while(column!=NULL){
            tmp2=column;
            column=column->east;
            free(tmp2);
            
        }
        tmp1=head;
        head=head->next;
        if(head!=NULL)
            column=head->colonne;
        free(tmp1);
    }
}

void freeclassifica(classifica head){
    classifica tmp;
    while(head!=NULL){
        tmp=head;
        head=head->next;
        free(tmp);
        
    }
}

void totalfreeslots(enemyslot head){
    enemyslot tmp;
    while(head!=NULL){
        tmp=head;
        head=head->prossimo;
        free(tmp);
    }
}


void game(classifica* head){
    int ancora=1;
    int turni;
    int scelta;
    mappa griglia;
    enemyslot enslots=NULL;
    CreateEnemySlots(&enslots,50);
    turni=0;
    creamappa(&griglia);
    inizializza_mappa(griglia,16,19);
    do{
        RiempiCaselle(griglia);
        entity player = SpawnEntity(griglia,(rand() % (RowCount(griglia) - 2)) + 2,(rand() % (ColCount(griglia) - 2)) + 2,1,3,15);
        SpawnEnemies(griglia,enslots,player,3);
        printf("\n\n\t Premi un qualsiasi tasto per iniziare");
        while ( getchar() != '\n' );
        while(player->vite>0){
            PlayerMovement(griglia,player);
            EnemiesMovement(griglia,enslots,player);
            checkturn(&turni, griglia,player, enslots);
        }
        sleep(2);
        refreshmap(griglia);
        printf("\n\t\t\t\e[1;31m*SEI MORTO*\e[0;37m\n");
        printf("\n\n\t (1) Riprova \n\n\t (2) Salva punteggio e vai al menu principale \n\n Scelta: ");
        do{
            scanf("%d",&scelta);
            switch(scelta){
                case 1:  FreeSlots(enslots);  ClearScn(); free(player); break;
                case 2: salvasuclassifica(player->uccisioni,head);free(player); freemap(griglia); FreeSlots(enslots); totalfreeslots(enslots); ancora=0;break;
                default: while ( getchar() != '\n' ); printf("\n Comando non supportato, riprovare\n"); break;
            }
        }
        while(scelta != 1 && scelta != 2);
    }    
    while(ancora==1);
}

void menu(){
    classifica classifica=NULL;
    CreaFile();
    riempiclassifica(&classifica);
    int scelta;
    do{
        printf("\e[1;31m");
        printf("\t __  __ ___ _  _ _   _ \n\t|  \\/  | __| \\| | | | |\n\t| |\\/| | _|| .` | |_| |\n\t|_|  |_|___|_|\\_|\\___/\n\n");
        printf("\e[1;37m");    
        printf("\n (1) Nuova partita\n\n (2) Istruzioni \n\n (3) Classifica \n\n (4) Esci \n\n \n Scelta: ");
    fflush(stdout);
        scanf("%d",&scelta);
        switch(scelta){
            case 1: ClearScn(); game(&classifica); break;
            case 2: ClearScn(); istruzioni(); break;
            case 3: stampaclassifica(classifica); ClearScn(); break;
            case 4: scelta = 4; break;
            default: while ( getchar() != '\n' ); ClearScn();  printf("Scelta non disponibile\n"); break;
        }
    }
    while(scelta != 4);
    freeclassifica(classifica);
    
}


//zona game 




int main(){
    srand(time(NULL));
    menu();
    return 0;
}




















