#include "entity.h"

entity CreateEntity(mappa map, int i, int j,int tipo){
    mappa tmp=map;
    struct colonne* tmp2;
    entity newentity=(entity)malloc(sizeof(struct entity));
    newentity->tipo=tipo;
    while(tmp->i!=i){
        tmp=tmp->next;
    }
    tmp2=tmp->colonne;
    while(tmp2->j!=j){
        tmp2=tmp2->east;
    }
    newentity->posizione=tmp2;
    if(newentity->posizione->casella.stato!=0){
        free(newentity);
        return NULL;
    }
    if(newentity->posizione->casella.stato==0){
        newentity->posizione->casella.stato=tipo;
        newentity->posizione->casella.pos_hold=newentity;
        refreshmap(map);
        return newentity;
    }
}

void setstats(entity entity,int vite, int ammo){
    entity->vite=vite;
    entity->ammo=ammo;
    entity->dmg=1;
    entity->uccisioni=0;
}

void printstats(entity entity){
    printf("\t \e[1;31mVite:\e[1;37m %d\t\t  \e[1;33mMunizioni: \e[1;37m%d    \n\t X: %d Y: %d\t\t  \e[1;37mUccisioni: %d",entity->vite,entity->ammo,entity->posizione->casella.x,entity->posizione->casella.y,entity->uccisioni);
    fflush(stdout);
}

entity SpawnEntity(mappa map,int i,int j,int tipo, int vite, int ammo){
    entity NewEntity=CreateEntity(map,i,j,tipo);
    if(NewEntity==NULL)
        return NULL;
    setstats(NewEntity,vite,ammo);
    if(tipo==1)
        printstats(NewEntity);
    return NewEntity;
}


int IsLegal(struct colonne* posizione){
    if(posizione->casella.stato==0)
        return 1;
    if(posizione->casella.stato>=6){
        return 2;
    }    
    else
        return 0;
}


int moveup(mappa map, entity player){
    if(player->tipo==2 && IsLegal(player->posizione->north)==2)
        return 1;
    if(IsLegal(player->posizione->north)==0 )
        return 1;
    if(IsLegal(player->posizione->north)==2 && player->tipo == 1){
        ItemEffect(player->posizione->north->casella.stato,player);
    }
    int tipo=player->posizione->casella.stato;
    player->posizione->casella.stato=0;
    player->posizione->casella.pos_hold=NULL;
    player->posizione=player->posizione->north;
    player->posizione->casella.stato=tipo;
    player->posizione->casella.pos_hold=player;
    refreshmap(map);
    if(player->tipo==1)
    printstats(player);
    return 0;
}

int movedown(mappa map, entity player){
    if(player->tipo==2 && IsLegal(player->posizione->south)==2)
        return 1;
    if(IsLegal(player->posizione->south)==0)
        return 1;
    if(IsLegal(player->posizione->south)==2 && player->tipo == 1){
        ItemEffect(player->posizione->south->casella.stato,player);
    }    
    int tipo=player->posizione->casella.stato;
    player->posizione->casella.stato=0;
    player->posizione->casella.pos_hold=NULL;
    player->posizione=player->posizione->south;
    player->posizione->casella.stato=tipo;
    player->posizione->casella.pos_hold=player;
    refreshmap(map);
    if(player->tipo==1)
    printstats(player);
    return 0;
}

int moveright(mappa map, entity player){
    if(player->tipo==2 && IsLegal(player->posizione->east)==2)
        return 1;
    if(IsLegal(player->posizione->east)==0)
        return 1;
    if(IsLegal(player->posizione->east)==2 && player->tipo == 1){
        ItemEffect(player->posizione->east->casella.stato,player);
    }    
    int tipo=player->posizione->casella.stato;
    player->posizione->casella.stato=0;
    player->posizione->casella.pos_hold=NULL;
    player->posizione=player->posizione->east;
    player->posizione->casella.stato=tipo;
    player->posizione->casella.pos_hold=player;
    refreshmap(map);
    if(player->tipo==1)
    printstats(player);
    return 0;
}

int moveleft(mappa map, entity player){
    if(player->tipo==2 && IsLegal(player->posizione->west)==2)
        return 1;
    if(IsLegal(player->posizione->west)==0)
        return 1;
    if(IsLegal(player->posizione->west)==2 && player->tipo == 1){
        ItemEffect(player->posizione->west->casella.stato,player);
    }    
    int tipo=player->posizione->casella.stato;    
    player->posizione->casella.stato=0;
    player->posizione->casella.pos_hold=NULL;
    player->posizione=player->posizione->west;
    player->posizione->casella.stato=tipo;
    player->posizione->casella.pos_hold=player;
    refreshmap(map);
    if(player->tipo==1)    
    printstats(player);
    return 0;
}

int HitCheck(struct colonne* pos,entity player){
    int chance=rand()%4;
    if (pos==NULL)
        return 0;
    if(pos->casella.stato==1 || pos->casella.stato==2){
        if(chance!=1){
            pos->casella.pos_hold->vite-=player->dmg;
            printf("\n\t\e[1;32mGIOCATORE \e[1;37m----->\e[1;31m NEMICO\e[1;37m");
            printf("\t*COLPITO*\n");
            printf("\e[0;37m");
            fflush(stdout);
            sleep(1);
            if(pos->casella.pos_hold->vite<=0){
                pos->casella.stato=0;
                player->uccisioni+=1;
            }
            return 1;
        }
        else{
            printf("\n\t\e[1;32mGIOCATORE \e[1;37m----->\e[1;31m NEMICO\e[1;37m");
            printf("\t*MANCATO*\n");
            printf("\e[0;37m");
            fflush(stdout);
            sleep(1);
        }
    }    
    else 
        return 0;
}

int shootup(mappa map, entity player){
    if(player->ammo<1){
        printf("\e[1;31m");
        printf("\t\t*MUNIZIONI ESAURITE*");
        printf("\e[1;37m");
         return 1;
    }
    player->ammo-=1;
    if(player->posizione->north->casella.stato==0){
        player->posizione->north->casella.stato=3;
        refreshmap(map);
        printstats(player);
        sleep(1);
        player->posizione->north->casella.stato=0;
        refreshmap(map);
        printstats(player);
        if(player->posizione->north->north->casella.stato==0){
            player->posizione->north->north->casella.stato=3;
            HitCheck(player->posizione->north->north,player);
            refreshmap(map);
            printstats(player);
            sleep(1);
            player->posizione->north->north->casella.stato=0;
        }   
    }
    HitCheck(player->posizione->north,player);
    HitCheck(player->posizione->north->north,player);
    return 0;
}


int shootdown(mappa map, entity player){
    if(player->ammo<1){
        printf("*MUNIZIONI ESAURITE");
         return 1;
    }
    player->ammo-=1;
    if(player->posizione->south->casella.stato==0){
        player->posizione->south->casella.stato=3;
        refreshmap(map);
        printstats(player);
        sleep(1);
        player->posizione->south->casella.stato=0;
        refreshmap(map);
        printstats(player);
        if(player->posizione->south->south->casella.stato==0){
            player->posizione->south->south->casella.stato=3;
            refreshmap(map);
            printstats(player);
            sleep(1);
            player->posizione->south->south->casella.stato=0;
        }
    }
    HitCheck(player->posizione->south,player);
    HitCheck(player->posizione->south->south,player);
    return 0;
}

int shootright(mappa map, entity player){
    if(player->ammo<1){
        printf("*MUNIZIONI ESAURITE");
         return 1;
    }
    player->ammo-=1;
    if(player->posizione->east->casella.stato==0){
        player->posizione->east->casella.stato=3;
        refreshmap(map);
        printstats(player);
        sleep(1);
        player->posizione->east->casella.stato=0;
        refreshmap(map);
        printstats(player);
        if(player->posizione->east->east->casella.stato==0){
            player->posizione->east->east->casella.stato=3;
            HitCheck(player->posizione->east->east,player);
            refreshmap(map);
            printstats(player);
            sleep(1);
            player->posizione->east->east->casella.stato=0;
        }
    }
    HitCheck(player->posizione->east,player);
    HitCheck(player->posizione->east->east,player);
    return 0;
}

int shootleft(mappa map, entity player){
    if(player->ammo<1){
        printf("\t*MUNIZIONI ESAURITE*");
         return 1;
    }
    player->ammo-=1;
    if(player->posizione->west->casella.stato==0){
        player->posizione->west->casella.stato=3;
        HitCheck(player->posizione->west,player);
        refreshmap(map);
        printstats(player);
        sleep(1);
        player->posizione->west->casella.stato=0;
        refreshmap(map);
        printstats(player);
        if(player->posizione->west->west->casella.stato==0){
            player->posizione->west->west->casella.stato=3;
            HitCheck(player->posizione->west->west,player);
            refreshmap(map);
            printstats(player);
            sleep(1);
            player->posizione->west->west->casella.stato=0;
        }
    }
    HitCheck(player->posizione->west,player);
    HitCheck(player->posizione->west->west,player);
    return 0;
}

float DistanzaEntity(struct colonne* pos1,struct colonne* pos2){
    float x1=pos1->casella.x;
    float y1=pos1->casella.y;
    float x2=pos2->casella.x;
    float y2=pos2->casella.y;
    float power=pow(x2-x1,2)+pow(y2-y1,2);
    float distanza=sqrt(power);
    return distanza;
}

int ValutaDistanze(float dist, struct colonne* pos1, struct colonne* pos2){
    if(DistanzaEntity(pos1,pos2)<=dist)
        return 1;
    else 
        return 0;
}

int InsEnemySlot(enemyslot* head){
    enemyslot p=*head;
    enemyslot  newslot=(enemyslot)malloc(sizeof(struct enemyslot)); 
    if(newslot==NULL)
        return 1;
    newslot->stato=0;
    newslot->nemico=NULL;
    if(*head==NULL){
        *head=newslot;
        newslot->prossimo=NULL;
        return 0;
    }
    while(p->prossimo!=NULL){
        p=p->prossimo;
    }
    p->prossimo=newslot;
    newslot->prossimo=NULL;
}

void CreateEnemySlots(enemyslot* head, int n){
    int i=0;
    for(i=0;i<n;i++){
        InsEnemySlot(head);
    }
}

int SlotNum(enemyslot head){
    int n=0;
    while (head!=NULL){
        n+=1;
        head=head->prossimo;
    }
    return n;
}

int EnemyCount(enemyslot head){
    int n=0;
    while (head!=NULL){
        if(head->stato==1)
            n+=1;
        head=head->prossimo;
    }
    return n;
}

int CheckStato(enemyslot slot){
    if(slot->stato==1){
        if(slot->nemico->vite<=0){
            slot->stato=0;
            free(slot->nemico);
            slot->nemico=NULL;
        }
    }
}

int IsFull(enemyslot slot){
    if (SlotNum(slot)==EnemyCount(slot))
        return 1;
    else
        return 0;
}

void FreeSlots(enemyslot slot){
    while(slot!=NULL){
        free(slot->nemico);
        slot->nemico=NULL;
        slot->stato=0;
        slot=slot->prossimo;
    }
}

enemyslot FirstAvaibleSlot(enemyslot head){
    while(head->stato==1 && head!=NULL){
        head=head->prossimo;   
    }
    
    if(head==NULL)
        return NULL;
    else{
        head->stato=1;
        return head;
    }
        
}


void wander(mappa griglia,entity enemy){
    int mossa;
    mossa=rand()%4;
    switch(mossa){
            case 0: moveup(griglia,enemy);break;
            case 1: movedown(griglia,enemy);break;
            case 2: moveright(griglia,enemy);break;
            case 3: moveleft(griglia,enemy);break;
    }
}
    
void follow(mappa griglia, entity enemy, entity player){
    int check=1;
    while(check==1){
        int mossa=rand()%4;
        switch(mossa){
            case 0: if(ValutaDistanze(DistanzaEntity(enemy->posizione,player->posizione),enemy->posizione->north,player->posizione)==1){
                        moveup(griglia,enemy);
                        check=0;
                    }
                    break; 
            case 1: if(ValutaDistanze(DistanzaEntity(enemy->posizione,player->posizione),enemy->posizione->south,player->posizione)==1){
                        movedown(griglia,enemy);
                        check=0;
                    }
                    break; 
            case 2: if(ValutaDistanze(DistanzaEntity(enemy->posizione,player->posizione),enemy->posizione->east,player->posizione)==1){
                        moveright(griglia,enemy);
                        check=0;
                    }
                    break; 
            case 3: if(ValutaDistanze(DistanzaEntity(enemy->posizione,player->posizione),enemy->posizione->west,player->posizione)==1){
                        moveleft(griglia,enemy);
                        check=0;
                    }
                    break;    
        }
    }
    printstats(player);
}    


void attack(entity player){
    int chance=rand()%2;
    if(chance==1){
       player->vite-=1;
       printf("\n\n\t\e[1;31mNEMICO \e[0;37m----->\e[1;32m GIOCATORE\e[1;37m");
       printf("\t*COLPITO*\n");
       ItemDeactivation(player);
       fflush(stdout);
       sleep(1);
   }
   else{
       printf("\n\n\t\e[1;31mNEMICO\e[0;37m ----->\e[1;32m GIOCATORE\e[1;37m");    
       printf("\t*MANCATO*\n");
       fflush(stdout);
       sleep(1);
    }
}


int EnemyMovement(mappa griglia, entity enemy,entity player){
    if(enemy->vite<=0){
        printstats(player);
        return 1;
    }
    
    if(DistanzaEntity(enemy->posizione,player->posizione)<=1){
        attack(player);
        return 0;
    }
    
    if(DistanzaEntity(enemy->posizione,player->posizione)>=3.5){
        wander(griglia, enemy);
        refreshmap(griglia);
        printstats(player);
        return 0;
    }
    else{
        follow(griglia,enemy,player);
        return 0;
    } 
}

void EnemiesMovement(mappa griglia, enemyslot head, entity player){
    while(head!=NULL){
        CheckStato(head);
        if(head->stato==1){
            EnemyMovement(griglia,head->nemico,player);
        }
        head=head->prossimo;
    }
}

void SpawnEnemy(mappa griglia,enemyslot head,entity player){
    if(IsFull(head)==0){
        int i = (rand() % (RowCount(griglia) - 2)) + 2;
        int j = (rand() % (ColCount(griglia) - 2)) + 2;
        enemyslot slot = FirstAvaibleSlot(head);
        slot->nemico = SpawnEntity(griglia,i,j,2,2,999);
        if(slot->nemico==NULL)
            slot->stato=0;
        printstats(player);
    }
}

void SpawnEnemies(mappa griglia, enemyslot head,entity player, int n){
    int i;
    for(i=0;i<n;i++){
        refreshmap(griglia);
        SpawnEnemy(griglia, head,player);
        
        
    }
}


void PlayerMovement(mappa griglia, entity player){
    char mossa;
    int check=1;
    while(check==1){
        printf("\n\n Comando: ");
        scanf("%c",&mossa);
        while ( getchar() != '\n' );
        switch(mossa){
            case 'w': check=moveup(griglia,player); break;
            case 'W': check=moveup(griglia,player); break;
            case 's': check=movedown(griglia,player); break;
            case 'S': check=movedown(griglia,player); break;
            case 'a': check=moveleft(griglia,player); break;
            case 'A': check=moveleft(griglia,player); break; 
            case 'd': check=moveright(griglia,player); break;
            case 'D': check=moveright(griglia,player); break;
            case 't': check=shootup(griglia,player); break;
            case 'T': check=shootup(griglia,player); break;
            case 'g': check=shootdown(griglia,player); break;
            case 'G': check=shootdown(griglia,player); break; 
            case 'f': check=shootleft(griglia,player); break;
            case 'F': check=shootleft(griglia,player); break; 
            case 'h': check=shootright(griglia,player); break;
            case 'H': check=shootright(griglia,player); break;
            default : printf(" Comando non riconosciuto"); break; 
        }
    }
     refreshmap(griglia);
     printstats(player);
}

void SpawnItem(mappa griglia,int i,int j, int tipo){ //1 vita,//2 ammo, //3 dmg
     mappa tmp=griglia;
     struct colonne* tmp2;
     while(tmp->i!=i){
        tmp=tmp->next;
    }
    tmp2=tmp->colonne;
    while(tmp2->j!=j){
        tmp2=tmp2->east;
    }
    switch(tipo){
        case 0: tmp2->casella.stato=6;break;
        case 1: tmp2->casella.stato=7; break;
        case 2: tmp2->casella.stato=8; break;
    }
}

void ItemEffect(int tipo, entity player){
    switch(tipo){
        case 6: printf("\n  *Vita Extra*\n"); fflush(stdout); sleep(1); player->vite+=1; break;
        case 7: printf("\n  *+10 Munizioni*\n"); fflush(stdout); sleep(1); player->ammo+=10; break;
        case 8: printf("\n  *Danno Aumentato*\n"); fflush(stdout); sleep(1); player->dmg=2; break;
    }
}


void ItemDeactivation(entity player){
    player->dmg=1;
}

void checkturn(int *turni,mappa griglia,entity player, enemyslot enslots){
    if(*turni > 0 && *turni <= 30 && *turni % 8 == 0 ){
        SpawnEnemy(griglia, enslots,player);
    }
    if(*turni>30 && *turni < 60 && *turni % 13 == 0){
        SpawnEnemies(griglia, enslots,player, 2);
    }
    if(*turni > 60 && *turni< 120 && *turni % 13 == 0 ){
        SpawnEnemies(griglia, enslots,player, 3);
    }
    if(*turni > 120 && *turni % 16 == 0){
        SpawnEnemies(griglia, enslots,player, 4);
    }
    if(*turni % 5 == 0 && (rand() % 3) == 1){
            SpawnItem(griglia, (rand() % (RowCount(griglia) - 2)) + 2  , (rand() % (ColCount(griglia) - 2)) + 2, rand() % 3);
    }
    *turni=*turni+1;
}

