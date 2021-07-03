#include "classifica.h"

int InsOrdinato(classifica* head,char nome[20],int punteggio){
    classifica tmp=*head;
    classifica nuovonodo=(classifica)malloc(sizeof(struct nodo));
    if(nuovonodo==NULL)
        return 1;
    strcpy(nuovonodo->nome,nome);
    nuovonodo->punteggio=punteggio;
    if(*head==NULL){
        nuovonodo->next=NULL;
        nuovonodo->prec=NULL;
        *head=nuovonodo;
        return 0;
    }
    while(tmp->punteggio<=punteggio && tmp->next!=NULL){
        tmp=tmp->next;
    }
    if(tmp->next==NULL && tmp->punteggio<=punteggio){
        tmp->next=nuovonodo;
        nuovonodo->next=NULL;
        nuovonodo->prec=tmp;
        return 0;
    }
    if(tmp==*head){
        nuovonodo->next=tmp;
        tmp->prec=nuovonodo;
        nuovonodo->prec=NULL;
        *head=nuovonodo;
        return 0;
    }
    nuovonodo->next=tmp;
    nuovonodo->prec=tmp->prec;
    tmp->prec->next=nuovonodo;
    tmp->prec=nuovonodo;
    return 0;
}

void CreaFile(){
    FILE* nuovofile = fopen("classifica","a+");
    fclose(nuovofile);
}

void stampasufile(char nome[20], int punteggio){
        FILE* leaderboard=fopen("classifica","a+");
        fprintf(leaderboard,"%s  %d\n",nome,punteggio);
        fclose(leaderboard);
}

int riempiclassifica(classifica* head){
    char nome[20];
    int punteggio;
    char ch;
    FILE* leaderboard=fopen("classifica","r");
    ch=fgetc(leaderboard);
    if(ch==EOF)
        return 1;
    else {
        ungetc(ch,leaderboard);
    }
    while(feof(leaderboard)==0){
        fscanf(leaderboard,"%s %d\n",nome,&punteggio);
            InsOrdinato(head,nome,punteggio);
    }
    fclose(leaderboard);
}

void salvasuclassifica(int punti,classifica* head){
    char nome[20];
    printf("\n\tInserisci il tuo nome: ");
    scanf("%s",nome);
    InsOrdinato(head,nome,punti);
    stampasufile(nome,punti);
    while ( getchar() != '\n' ); printf("\e[1;1H\e[2J");
}

int stampaclassifica(classifica head){
    printf("\e[1;1H\e[2J");
    printf("\e[1;33m");
    printf("\n\t  ____ _        _    ____ ____ ___ _____ ___ ____    _  \n");
    printf("\t / ___| |      / \\  / ___/ ___|_ _|  ___|_ _/ ___|  / \\\n");
    printf("\t| |   | |     / _ \\ \\___ \\___ \\| || |_   | | |     / _ \\\n");
    printf("\t| |___| |___ / ___ \\ ___) |__) | ||  _|  | | |___ / ___ \\\n");
    printf("\t \\____|_____/_/   \\_\\____/____/___|_|   |___\\____/_/   \\_\\\n\n\n");
    printf("\e[1;37m");
    int pos=1;
    if(head==NULL){
        while ( getchar() != '\n' );
        printf("\n\n\n\n\n\t\t\t   CLASSIFICA VUOTA\n\n\n\t\tPremi un qualsiasi tasto per continuare\n");
        getchar();
        
        return 1;
    }
    while(head->next!=NULL){
        head=head->next;
    }
    while(head!=NULL){
        switch(pos){
            case 1:printf("\e[1;31m");break;
            case 2:printf("\e[1;33m");break;
            case 3:printf("\e[1;32m");break;
        }
        if(pos<=3)
            printf("\t%d° ",pos++);
        else
            printf("\t");
        printf("%s %d\n\n",head->nome,head->punteggio);
        printf("\e[0;37m");
        head=head->prec;
    }
    printf("\e[1;37m");
    printf("\t Premi un qualsiasi tasto per continuare\n");
    printf("\e[0;37m");
    getchar();
    while ( getchar() != '\n' );
    return 0;
    
}


