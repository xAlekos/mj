#include "mappa.h"

void ClearScn(){
    printf("\e[1;1H\e[2J");
}

int creamappa(mappa* head){
    mappa nuovamappa=(mappa)malloc(sizeof(struct righe));
    if (nuovamappa==NULL)
        return 1;
    nuovamappa->next=NULL;
    nuovamappa->prec=NULL;
    nuovamappa->colonne=NULL;
    nuovamappa->i=1;
    *head=nuovamappa;
    return 0;
}

int addrow(mappa head){
    mappa nuovamappa=(mappa)malloc(sizeof(struct righe));
    if (nuovamappa==NULL)
        return 1;
    nuovamappa->next=NULL;
    nuovamappa->colonne=NULL;
    while(head->next!=NULL){
        head=head->next;
    }
    nuovamappa->prec=head;
    nuovamappa->i=nuovamappa->prec->i+1;
    head->next=nuovamappa;
}

int addcol(mappa head){
    struct colonne* ptr= head->colonne;
    struct colonne* nuovacolonna=(struct colonne*)malloc(sizeof(struct colonne));
    if(head->colonne==NULL){
        head->colonne=nuovacolonna;
        nuovacolonna->west=NULL;
        nuovacolonna->east=NULL;
        nuovacolonna->j=1;
        return 1;
    }
    while(ptr->east!=NULL){
        ptr=ptr->east;
    }
    nuovacolonna->west=ptr;
    nuovacolonna->j=ptr->j+1;
    ptr->east=nuovacolonna;
    return 1;
}

void riempimappa(mappa map,int i,int j){
    int it;
    int jt;
    mappa tmp=map;
    mappa tmp2=map;
    int check=1;
    for(it=0;it<i-1;it++){
        addrow(tmp);
        if(it!=0)
            tmp2=tmp2->next;
        for(jt=0;jt<j;jt++)
            addcol(tmp2);
    }
    tmp2=tmp2->next; //viene aggiunta l'ultima colonna in quanto le righe partivano da 1;
    for(jt=0;jt<j;jt++)
        addcol(tmp2);   
}

int concatena_riga(mappa lista, mappa riga, int i){
    mappa tmp=lista;
    struct colonne* tmp2;
    struct colonne* column= riga->colonne;
    if(riga==lista){
        while(column!=NULL){
            column->north=NULL;
            column=column->east;
        }
        column=riga->colonne;
    }
    if(riga->i==i){
        while(column!=NULL){
            column->south=NULL;
            column=column->east;
        }
        column=riga->colonne;
    }
    if(riga!=lista){
        tmp=riga->prec;
        tmp2=tmp->colonne;
        while(column!=NULL){
            column->north=tmp2;
            tmp2=tmp2->east;
            column=column->east;
        }
        column=riga->colonne;
        tmp=lista;
    }
    if(riga->i!=i){
        tmp=riga->next;
        tmp2=tmp->colonne;
        while(column!=NULL){
            column->south=tmp2;
            tmp2=tmp2->east;
            column=column->east;
        }
    }
}

int RowCount(mappa map){
    mappa tmp=map;
    int count=0;
    while(tmp!=NULL){
        count++;
        tmp=tmp->next;
    }
    return count;
}

int ColCount(mappa map){
    struct colonne* tmp=map->colonne;
    int count=0;
    while(tmp!=NULL){
        count++;
        tmp=tmp->east;
    }
    return count;
}

void concatena_liste(mappa map){
    int i=RowCount(map);
    mappa griglia = map;
    mappa tmp=map;
    while(tmp!=NULL){
        concatena_riga(map,tmp,i);
        tmp=tmp->next;
    }
    
}

void RiempiCaselle(mappa map){
    mappa tmp=map;
    int it=0;
    struct colonne* tmp2= tmp->colonne;
    int y=tmp->i;
    while(tmp!=NULL){
        while(tmp2!=NULL){
            tmp2->casella.stato=0;
            tmp2->casella.pos_hold=NULL;
            if(tmp2->j == 1 || tmp2->j==ColCount(tmp))
                tmp2->casella.stato=5;
            if(tmp->i==1 || tmp->i==RowCount(map))
                tmp2->casella.stato=4;    
            tmp2->casella.x=tmp2->j-1;
            tmp2->casella.y=(RowCount(map)-it)-1;
            tmp2=tmp2->east;
        }
        tmp=tmp->next;
        if(tmp!=NULL)
            tmp2=tmp->colonne;
        it+=1;
    }
}

void refreshsprites(mappa map){
    mappa tmp=map;
    struct colonne* tmp2= tmp->colonne;
    while(tmp!=NULL){
        while(tmp2!=NULL){
            switch(tmp2->casella.stato){
                case 0 : tmp2->casella.sprite=' '; break; //vuoto
                case 1 : tmp2->casella.sprite='*'; break; //player 
                case 2 : tmp2->casella.sprite='@'; break; //nemico
                case 3 : tmp2->casella.sprite='o'; break; //proiettile
                case 4 : tmp2->casella.sprite='-'; break; //muro orizz
                case 5 : tmp2->casella.sprite='|'; break; //muro vert
                case 6 : tmp2->casella.sprite='+'; break; //vita
                case 7 : tmp2->casella.sprite='A'; break; //ammo
                case 8 : tmp2->casella.sprite='S'; break; //dmg boost
            }
            tmp2=tmp2->east;
        }
        tmp=tmp->next;
        if(tmp!=NULL)
            tmp2=tmp->colonne;
    }
    
}



void stampamappa(mappa map){
    printf("\e[1;31m");
    printf("\n\t\t  ___   _   __  __ ___ \n\t\t / __| /_\\ |  \\/  | __|\n\t\t| (_ |/ _ \\| |\\/| | _|\n\t\t \\___/_/ \\_\\_|  |_|___|\n");
    printf("\e[1;37m");
    mappa tmp=map;
    struct colonne* column=tmp->colonne;
    printf("\n");
    while(tmp!=NULL){
        printf("\t");
        while(column!=NULL){
            printf(" %c",column->casella.sprite);
            column=column->east;
        }
        printf("\n");
        tmp=tmp->next;
        if(tmp!=NULL)
            column=tmp->colonne;
    }
    printf("\e[0;37m");
}

void inizializza_mappa(mappa griglia, int i, int j){
    riempimappa(griglia,i,j);
    concatena_liste(griglia);
    RiempiCaselle(griglia);
    refreshsprites(griglia);
    stampamappa(griglia);
}

void refreshmap(mappa map){
    ClearScn();
    refreshsprites(map);
    stampamappa(map);
    
}
