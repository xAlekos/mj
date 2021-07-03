#include "mappa.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


struct entity{
    int tipo;
    int vite;
    int ammo;
    int dmg;
    int uccisioni;
    struct colonne* posizione;
};

typedef struct entity* entity;

struct enemyslot{
    entity nemico;
    int stato; 
    struct enemyslot* prossimo;
};

typedef struct enemyslot* enemyslot;

entity CreateEntity(mappa map, int i, int j,int tipo);

void setstats(entity entity,int vite, int ammo);

void printstats(entity entity);

entity SpawnEntity(mappa map,int i,int j,int tipo, int vite, int ammo);

int IsLegal(struct colonne* posizione);

int moveup(mappa map, entity player);

int movedown(mappa map, entity player);

int moveright(mappa map, entity player);

int moveleft(mappa map, entity player);

int HitCheck(struct colonne* pos,entity player);

int shootup(mappa map, entity player);

int shootdown(mappa map, entity player);

int shootright(mappa map, entity player);

int shootleft(mappa map, entity player);

float DistanzaEntity(struct colonne* pos1,struct colonne* pos2);

int ValutaDistanze(float dist, struct colonne* pos1, struct colonne* pos2);

int InsEnemySlot(enemyslot* head);

void CreateEnemySlots(enemyslot* head, int n);

int SlotNum(enemyslot head);

int EnemyCount(enemyslot head);

int CheckStato(enemyslot slot);

int IsFull(enemyslot slot);

void FreeSlots(enemyslot slot);

enemyslot FirstAvaibleSlot(enemyslot head);

void wander(mappa griglia,entity enemy);

void follow(mappa griglia, entity enemy, entity player);

void attack(entity player);

int EnemyMovement(mappa griglia, entity enemy,entity player);

void EnemiesMovement(mappa griglia, enemyslot head, entity player);

void SpawnEnemy(mappa griglia,enemyslot head,entity player);

void SpawnEnemies(mappa griglia, enemyslot head,entity player, int n);

void PlayerMovement(mappa griglia, entity player);

void SpawnItem(mappa griglia,int i,int j, int tipo);

void ItemEffect(int tipo, entity player);

void ItemDeactivation(entity player);

void checkturn(int *turni,mappa griglia,entity player, enemyslot enslots);
