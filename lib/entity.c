# include <stdlib.h>
# include <stdio.h>
# include "entity.h"
# include "raylib.h"


Entity *createPlayerShip() {
    const float height = 72.0f;
    const float width = 96.0f;
    const float x = 912.0f;
    const float y = 900.0f;

    Entity *ship = (Entity *)malloc(sizeof(Entity));
    ship->type = PLAYER_SHIP;
    ship->bounds = (Rectangle){.height=height, .width=width, .x=x, .y=y};
    return ship;
}

Entity *createEnemyShip() {
    const float height = 40.0f;
    const float width = 64.0f;
    const float x = 1920.0f;
    const float y = 50.0f;

    Entity *enemyShip = (Entity *)malloc(sizeof(Entity));
    enemyShip->type = ENEMY_SHIP;
    enemyShip->bounds = (Rectangle){.height=height, .width=width, .x=x, .y=y};
    return enemyShip;
}

Entity *createListEntities() {
    Entity *leftSentinel = (Entity *)malloc(sizeof(Entity));
    Entity *rightSentinel = (Entity *)malloc(sizeof(Entity));
    leftSentinel->next = rightSentinel;
    rightSentinel->prev = leftSentinel;
    leftSentinel->type = LIST_SENTINEL;
    rightSentinel->type = LIST_SENTINEL;

    return leftSentinel;
}

Entity *createHorde(Entity *lastAlive) {
    const int sizeHorde = 55;
    const int rows = 5;
    const int columns = 11;
    const float height = 32.0f;
    const float width = 32.0f;
    const float gapX = 15.0f;
    const float gapY = 20.0f;
    const float offSetX = 1920.0f/2.0f - (width*(float)columns + gapX*((float)columns - 1.0f))/2.0f;
    const float offSetY = height*3.0f;
    float x, y;

    Entity *leftSentinel = createListEntities();

    Entity *current = leftSentinel;
    for (int i = 0; i < sizeHorde; ++i) {
        float x = offSetX + ((i % columns)*(width + gapX));
        float y = offSetY + ((i / columns)*(height + gapY));

        Entity *temp = (Entity *)malloc(sizeof(Entity));
        temp->bounds = (Rectangle){.height=height, .width=width, .x=x, .y=y};
        if (i / columns < 2) temp->alienType = TYPE1;
        else if (i / columns < 3) temp->alienType = TYPE2;
        else temp->alienType = TYPE3;

        temp->type = ALIEN;

        temp->next = current->next;
        current->next->prev = temp;
        current->next = temp;
        temp->prev = current;
        current = temp;
    }
    lastAlive = current;

    return leftSentinel;
}

void killEnemy(Entity *enemy) {
    enemy->next->prev = enemy->prev;
    enemy->prev->next = enemy->next;
    free(enemy);
}

void generateProjectile(
    Entity *list, float height, float width, float x, float y, bool up
) {
    Entity *projectile = (Entity *)malloc(sizeof(Entity));
    projectile->bounds = (Rectangle){.height=height, .width=width, .x=x, .y=y};
    projectile->up = up;

    projectile->next = list->next;
    projectile->prev = list;
    projectile->next->prev = projectile;
    projectile->prev->next = projectile;
}

void killProjectile(Entity *projectile) {
    projectile->next->prev = projectile->prev;
    projectile->prev->next = projectile->next;
    free(projectile);
}

Entity *createBulletsList() {
    return createListEntities();
}

void generateBullet(Entity *list, float x, float y, bool up) {
    const float height = 32.0f;
    const float width = 4.0f;

    generateProjectile(list, height, width, x - width/2.0f, y, up);
    list->next->type = BULLET;
}

void killBullet(Entity *bullet) {
    killProjectile(bullet);
}

Entity *createPowerupsList() {
    return createListEntities();
}

void generatePowerup(Entity *list, float x, float y) {
    const float width = 25.0f;
    int dropCheck = rand() % 100;

    generateProjectile(list, width, width, x - width/2.0f, y, false);
    if (dropCheck < 50) list->next->type = FAST_MOVE;
    else list->next->type = FAST_SHOT;
}

void killPowerup(Entity *powerup) {
    killProjectile(powerup);
}

void freeEntities(Entity *entity) {
    Entity *current = entity;
    Entity *next = current->next;

    while (next->type != LIST_SENTINEL) {
        free(current);
        current = next;
        next = current->next;
    }
    free(current);
    free(next);
}

void freeHorde(Entity *entity) {
    freeEntities(entity);
}

void freeBullets(Entity *entity) {
    freeEntities(entity);
}

void freePowerups(Entity *entity) {
    freeEntities(entity);
}

void freeShip(Entity *ship) {
    free(ship);
}

void freeEnemyShip(Entity *enemyShip) {
    free(enemyShip);
}
