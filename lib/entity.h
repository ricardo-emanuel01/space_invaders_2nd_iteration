# ifndef _ENTITY_H_
# define _ENTITY_H_

# include <stdlib.h>
# include <string.h>
# include "raylib.h"


typedef enum EntityType {
    PLAYER_SHIP,
    ALIEN,
    FAST_SHOT,
    ENEMY_SHIP,
    LIST_SENTINEL,
    BULLET,
    FAST_MOVE,
} EntityType;

typedef enum AlienTexture {
    TYPE1,
    TYPE2,
    TYPE3,
} AlienTexture;

typedef struct Entity {
    Rectangle bounds;
    struct Entity *next;
    struct Entity *prev;
    EntityType type;
    // Could be a union
    AlienTexture alienType;
    bool up;
} Entity;

Entity *createPlayerShip();

Entity *createEnemyShip();

Entity *createListEntities();

Entity *createHorde();

Entity *findLastAlive(Entity *);

void killEnemy(Entity *enemy);

Entity *createBulletsList();

void killProjectile(Entity *);

void generateBullet(Entity *list, float x, float y, bool up);

void killBullet(Entity *);

Entity *createPowerupsList();

void generatePowerup(Entity *list, float x, float y);

void killPowerup(Entity *);

void freeHorde(Entity *);

void freeBullets(Entity *);

void freePowerups(Entity *);

void freeShip(Entity *);

void freeEnemyShip(Entity *);

# endif
