# include <string.h>
# include <stdio.h>
# include <time.h>
# include "game.h"
# include "raylib.h"


ColdGameData *initColdGameData() {  
    const float shipSpeeds[] = {300.0f, 450.0f};
    const float shipDelaysToFire[] = {0.5f, 0.1f};
    const float screenLimits[] = {250.0f, 1670.0f};

    ColdGameData *gameData = (ColdGameData *)malloc(sizeof(ColdGameData));
    gameData->enemyShipDelayToFire = 0.25f;
    gameData->enemyShipSpeed = 450.0f;
    gameData->projectileSpeed = 600.0f;
    gameData->powerupDuration = 60.0f;
    gameData->animationDuration = 0.2f;
    gameData->hordeSpeedIncrease = 45.0f;
    gameData->hordeStepY = 50.0f;
    gameData->enemyShipSleepTime = 4.0f;
    
    memcpy(
        &gameData->shipSpeeds,
        shipSpeeds,
        2*sizeof(float)
    );
    memcpy(
        &gameData->shipDelaysToFire,
        shipDelaysToFire,
        2*sizeof(float)
    );
    memcpy(&gameData->screenLimits, screenLimits, 2*sizeof(float));

    return gameData;
}

HotGameData *initHotGameData() {
    HotGameData *gameData = (HotGameData *)malloc(sizeof(HotGameData));
    gameData->lastFrameTime = GetTime();
    gameData->hordeSpeed = 150.0f;
    gameData->gameState = MENU;
    gameData->menuButton = START;
    gameData->enemyShipGoingLeft = true;
    gameData->enemyShipActive = false;
    gameData->enemyShipDefeated = false;
    gameData->fastMoveActive = false;
    gameData->fastShotActive = false;
    gameData->shipLastShotTime = 0.0;
    gameData->remainingTimeEnemyShipAlarm = 4.0f;
    gameData->enemyShipLastShotTime = 0.0;

    return gameData;
}

void cleanupGameData(ColdGameData *cold, HotGameData *hot) {
    free(cold);
    free(hot);
}

Sounds *initSounds() {
    Sounds *sounds = (Sounds *)malloc(sizeof(Sounds));
    sounds->background = LoadMusicStream("assets/sounds/background.ogg");
    sounds->enemyShip = LoadMusicStream("assets/sounds/enemyShip.ogg");
    sounds->shipFire = LoadSound("assets/sounds/shipFire.ogg");
    sounds->enemyFire = LoadSound("assets/sounds/alienFire.ogg");
    sounds->shipExplosion = LoadSound("assets/sounds/shipExplosion.ogg");
    sounds->enemyExplosion = LoadSound("assets/sounds/alienExplosion.ogg");
    sounds->powerup = LoadSound("assets/sounds/powerup.ogg");
    sounds->lose = LoadSound("assets/sounds/lose.ogg");
    sounds->victory = LoadSound("assets/sounds/victory.ogg");
    sounds->menu = LoadSound("assets/sounds/menu.ogg");

    return sounds;
}

void cleanupSounds(Sounds *sounds) {
    UnloadMusicStream(sounds->background);
    UnloadMusicStream(sounds->enemyShip);
    UnloadSound(sounds->shipFire);
    UnloadSound(sounds->enemyFire);
    UnloadSound(sounds->shipExplosion);
    UnloadSound(sounds->enemyExplosion);
    UnloadSound(sounds->powerup);
    UnloadSound(sounds->lose);
    UnloadSound(sounds->victory);
    UnloadSound(sounds->menu);
    free(sounds);
}

Textures *initTextures() {
    Textures *textures = (Textures *)malloc(sizeof(Textures));
    textures->ship = LoadTexture("assets/textures/ship.png");
    textures->enemyShip = LoadTexture("assets/textures/enemyShip.png");
    textures->alienSlow = LoadTexture("assets/textures/alienSlow.png");
    textures->alienFast = LoadTexture("assets/textures/alienFast.png");
    textures->alienFaster = LoadTexture("assets/textures/alienFaster.png");
    textures->bullet = LoadTexture("assets/textures/bullet.png");
    textures->shotPowerup = LoadTexture("assets/textures/shotPowerup.png");
    textures->movePowerup = LoadTexture("assets/textures/movePowerup.png");

    return textures;
}

void cleanupTextures(Textures *textures) {
    UnloadTexture(textures->ship);
    UnloadTexture(textures->enemyShip);
    UnloadTexture(textures->alienSlow);
    UnloadTexture(textures->alienFast);
    UnloadTexture(textures->alienFaster);
    UnloadTexture(textures->bullet);
    UnloadTexture(textures->shotPowerup);
    UnloadTexture(textures->movePowerup);
    free(textures);
}

Animation *initAnimation() {
    Animation *animation = (Animation *)malloc(sizeof(Animation));
    animation->aliensFrame = (Rectangle){.height=16.0f, .width=16.0f, .x=0.0f, .y=0.0f};
    animation->shipFrame = (Rectangle){.height=12.0f, .width=16.0f, .x=0.0f, .y=0.0f};
    animation->bulletFrame = (Rectangle){.height=8.0f, .width=4.0f, .x=0.0f, .y=0.0f};
    animation->enemyShipFrame = (Rectangle){.height=10.0f, .width=16.0f, .x=0.0f, .y=0.0f};
    animation->powerupFrame = (Rectangle){.height=18.0f, .width=18.0f, .x=0.0f, .y=0.0f};
    animation->enemyCurrentFrame = 0;

    return animation;
}

void cleanupAnimation(Animation *animation) {
    free(animation);
}

Game initGame() {
    srand(time(NULL));
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1920.0f, 1080.0f, "Space Invaders Clone");
    InitAudioDevice();
    SetExitKey(KEY_NULL);
    DisableCursor();

    Game game = {
        .ship=createPlayerShip(),
        .enemyShip=createEnemyShip(),
        .bullets=createBulletsList(),
        .powerups=createPowerupsList(),
        .coldData=initColdGameData(),
        .hotData=initHotGameData(),
        .sounds=initSounds(),
        .textures=initTextures(),
        .animation=initAnimation(),
    };
    game.horde = createHorde(game.hordeLastAlive);

    game.sounds->background.looping = true;
    game.sounds->enemyShip.looping = true;
    PlayMusicStream(game.sounds->background);

    return game;
}

void cleanupGame(Game *game) {
    cleanupGameData(game->coldData, game->hotData);
    cleanupSounds(game->sounds);
    cleanupTextures(game->textures);
    cleanupAnimation(game->animation);
    freeShip(game->ship);
    freeEnemyShip(game->enemyShip);
    freeHorde(game->horde);
    freeBullets(game->bullets);
    freePowerups(game->powerups);
    CloseAudioDevice();
    CloseWindow();
}

void fire(Game *game, Entity *entity) {
    double now = GetTime();
    if (entity->type == PLAYER_SHIP) {
        float delayToFire;
        if (game->hotData->fastShotActive) {
            delayToFire = game->coldData->shipDelaysToFire[BUFFED];
        } else {
            delayToFire = game->coldData->shipDelaysToFire[REGULAR];
        }

        if (now - game->hotData->shipLastShotTime > delayToFire) {
            generateBullet(game->bullets, entity->bounds.x + entity->bounds.width/2.0f, entity->bounds.y, true);
            game->hotData->shipLastShotTime = now;
        }
    } else if (entity->type == ENEMY_SHIP) {
        if (now - game->hotData->enemyShipLastShotTime > game->coldData->enemyShipDelayToFire) {
            generateBullet(game->bullets, entity->bounds.x + entity->bounds.width/2.0f, entity->bounds.y + entity->bounds.height, false);
            game->hotData->enemyShipLastShotTime = now;
        }
    } else {
        generateBullet(game->bullets, entity->bounds.x + entity->bounds.width/2.0f, entity->bounds.y + entity->bounds.height, false);
        PlaySound(game->sounds->enemyFire);
    }
}

void processInput(Input *input) {
    const float stickDeadzone = 0.1f;
    float stickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
    if (stickX < stickDeadzone && stickX > -stickDeadzone) stickX = 0.0f;
    float stickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
    if (stickY < stickDeadzone && stickY > -stickDeadzone) stickY = 0.0f;

    input->up = IsKeyPressed(KEY_UP) || stickY > 0.0f;
    input->down = IsKeyPressed(KEY_DOWN) || stickY < 0.0f;
    input->left = IsKeyDown(KEY_LEFT) || stickX < 0.0f;
    input->right = IsKeyDown(KEY_RIGHT) || stickX > 0.0f;
    input->fire = IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    input->select = IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
    input->pause = IsKeyPressed(KEY_ESCAPE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
}

void updateShip(Game *game) {
    if (game->hotData->gameState == PLAYING) {
        Entity *ship = game->ship;
        Input input = game->hotData->input;
        double now = GetTime();
        double delta = now - game->hotData->lastFrameTime;

        if (game->hotData->fastShotActive) {
            game->hotData->fastShotRemainingTime -= delta;
            if (game->hotData->fastShotRemainingTime <= 0.0) game->hotData->fastShotActive = false;
        }

        if (game->hotData->fastMoveActive) {
            game->hotData->fastMoveRemainingTime -= delta;
            if (game->hotData->fastMoveRemainingTime <= 0.0) game->hotData->fastMoveActive = false;
        }

        if (input.right) {
            if (game->hotData->fastMoveActive) {
                if (ship->bounds.x + ship->bounds.width + game->coldData->shipSpeeds[BUFFED]*delta >= game->coldData->screenLimits[1]) {
                    ship->bounds.x = game->coldData->screenLimits[1] - ship->bounds.width;
                } else {
                    ship->bounds.x += game->coldData->shipSpeeds[BUFFED]*delta;
                }
            }
            else {
                if (ship->bounds.x + ship->bounds.width + game->coldData->shipSpeeds[REGULAR]*delta >= game->coldData->screenLimits[1]) {
                    ship->bounds.x = game->coldData->screenLimits[1] - ship->bounds.width;
                } else {
                    ship->bounds.x += game->coldData->shipSpeeds[REGULAR]*delta;
                }
            }
        }
        if (input.left) {
            if (game->hotData->fastMoveActive) {
                if (ship->bounds.x - game->coldData->shipSpeeds[BUFFED]*delta <= game->coldData->screenLimits[0]) {
                    ship->bounds.x = game->coldData->screenLimits[0];
                } else {
                    ship->bounds.x -= game->coldData->shipSpeeds[BUFFED]*delta;
                }
            } else {
                if (ship->bounds.x - game->coldData->shipSpeeds[REGULAR]*delta <= game->coldData->screenLimits[0]) {
                    ship->bounds.x = game->coldData->screenLimits[0];
                } else {
                    ship->bounds.x -= game->coldData->shipSpeeds[REGULAR]*delta;
                }
            }
        }

        if (input.fire) {
            fire(game, ship);
        }
    }
}

void updateHorde(Game *game) {
    if (game->hotData->gameState == PLAYING) {
        Entity *current = game->horde->next;
        double delta = GetTime() - game->hotData->lastFrameTime;
        bool changeDirection = false;
        float maxMovement;
    
        if (game->hotData->hordeSpeed > 0.0f) {
            float maxPositionX = current->bounds.x;
            while (current->type != LIST_SENTINEL) {
                maxPositionX = current->bounds.x > maxPositionX ? current->bounds.x : maxPositionX;
                current = current->next;
            }

            if (maxPositionX + game->hotData->hordeSpeed*delta + game->horde->next->bounds.width >= game->coldData->screenLimits[1]) {
                maxMovement = 2.0f*game->hotData->hordeSpeed*delta - game->coldData->screenLimits[1] + maxPositionX + game->horde->next->bounds.width;
                changeDirection = true;
                game->hotData->hordeSpeed *= -1;
                game->hotData->hordeSpeed -= game->coldData->hordeSpeedIncrease;
            } else {
                maxMovement = game->hotData->hordeSpeed*delta;
            }
        } else {
            float minPositionX = current->bounds.x;
            while (current->type != LIST_SENTINEL) {
                minPositionX = current->bounds.x < minPositionX ? current->bounds.x : minPositionX;
                current = current->next;
            }
            if (minPositionX + game->hotData->hordeSpeed*delta <= game->coldData->screenLimits[0]) {
                maxMovement = minPositionX - game->coldData->screenLimits[0];
                changeDirection = true;
                game->hotData->hordeSpeed *= -1;
                game->hotData->hordeSpeed += game->coldData->hordeSpeedIncrease;
            } else {
                maxMovement = game->hotData->hordeSpeed*delta;
            }
        }
    
        for (Entity *current = game->horde->next; current->type != LIST_SENTINEL; current = current->next) {
            int dropCheck = rand() % 100000;
            if (dropCheck < 10) fire(game, current);
            current->bounds.x += maxMovement;
            
            if (changeDirection) {
                current->bounds.y += game->coldData->hordeStepY;
            }
        }
    }
}

void updateEnemyShip(Game *game) {
    if (!game->hotData->enemyShipDefeated && !game->hotData->enemyShipActive) {
        game->hotData->remainingTimeEnemyShipAlarm -= GetTime() - game->hotData->lastFrameTime;
        if (game->hotData->remainingTimeEnemyShipAlarm <= 0.0) {
            game->hotData->enemyShipActive = true;
            PlayMusicStream(game->sounds->enemyShip);
        }
    } else if (game->hotData->enemyShipActive && !game->hotData->enemyShipDefeated) {
        Entity *enemyShip = game->enemyShip;
        const float enemyShipMove = game->coldData->enemyShipSpeed*(GetTime() - game->hotData->lastFrameTime);
        fire(game, game->enemyShip);
    
        UpdateMusicStream(game->sounds->enemyShip);
        if (game->hotData->enemyShipGoingLeft) {
            if (enemyShip->bounds.x - enemyShipMove <= game->coldData->screenLimits[0]) {
                enemyShip->bounds.x = game->coldData->screenLimits[0];
                game->hotData->enemyShipGoingLeft = false;
            } else {
                enemyShip->bounds.x -= enemyShipMove;
            }
        } else {
            if (enemyShip->bounds.x + enemyShipMove >= 1920.0f) {
                enemyShip->bounds.x = 1920.0f;;
                game->hotData->enemyShipGoingLeft = true;
                game->hotData->remainingTimeEnemyShipAlarm = game->coldData->enemyShipSleepTime;
                game->hotData->enemyShipActive = false;
                StopMusicStream(game->sounds->enemyShip);
            } else {
                enemyShip->bounds.x += enemyShipMove;
            }
        }
    }
}

void updateProjectiles(Game *game, Entity *projectiles) {
    double delta = GetTime() - game->hotData->lastFrameTime;
    Entity *current = projectiles->next;

    while (current->type != LIST_SENTINEL) {
        if (current->up) {
            current->bounds.y -= game->coldData->projectileSpeed * delta;
        } else {
            current->bounds.y += game->coldData->projectileSpeed * delta;
        }

        if ((current->bounds.y > 1080.0f) || (current->bounds.y - current->bounds.height < 0.0f)) {
            Entity *toKill = current;
            current = current->prev;
            killProjectile(toKill);
        }

        current = current->next;
    }
}

void updateMenu(Game *game) {
    if (game->hotData->gameState == MENU) {
        if (game->hotData->input.up || game->hotData->input.down) {
            PlaySound(game->sounds->menu);
            if (game->hotData->menuButton == START)
                game->hotData->menuButton = QUIT;
            else
                game->hotData->menuButton = START;
        }
    } else if (game->hotData->gameState == WIN || game->hotData->gameState == LOSE) {
        if (game->hotData->input.up || game->hotData->input.down) {
            PlaySound(game->sounds->menu);
            if (game->hotData->menuButton == RESTART)
                game->hotData->menuButton = QUIT;
            else
                game->hotData->menuButton = RESTART;
        }
    }
}

void updateGameState(Game *game) {
    GameState gameState = game->hotData->gameState;
    if (gameState == PLAYING) {
        if (game->hotData->input.pause) {
            game->hotData->gameState = MENU;
        }
    }

    if (gameState == MENU) {
        if (game->hotData->input.select) {
            if (game->hotData->menuButton == START) {
                game->hotData->gameState = PLAYING;
            } else if (game->hotData->gameState == QUIT) {
                game->hotData->gameState = CLOSE;
            }
        }
    }

    if (gameState == WIN || gameState == LOSE) {
        if (game->hotData->input.select) {
            if (game->hotData->menuButton == RESTART) {
                //rebootGame(game);
            } else if (game->hotData->menuButton == QUIT) {
                game->hotData->gameState = CLOSE;
            }
        }
    }
}

void updateGame(Game *game) {
    updateGameState(game);
    UpdateMusicStream(game->sounds->background);

    if (game->hotData->gameState == PLAYING) {
        detectCollisions(game);
        updateShip(game);
        updateHorde(game);
        updateEnemyShip(game);
        updateProjectiles(game, game->bullets);
        updateProjectiles(game, game->powerups);
    } else if (game->hotData->gameState != CLOSE)
        updateMenu(game);
    
    game->hotData->lastFrameTime = GetTime();
}

bool detectCollision(Entity *entity, Entity *otherEntity) {
    if (
        entity->bounds.x <= otherEntity->bounds.x + otherEntity->bounds.width &&
        entity->bounds.x + entity->bounds.width >= otherEntity->bounds.x &&
        entity->bounds.y <= otherEntity->bounds.y + otherEntity->bounds.width &&
        entity->bounds.y + entity->bounds.height >= otherEntity->bounds.y
    ) {
        return true;
    }

    return false;
}

void detectCollisions(Game *game) {
    Entity *currentBullet = game->bullets->next;
    bool hit;
    int dropCheck;
    while (currentBullet->type != LIST_SENTINEL) {
        hit = false;
        if (currentBullet->up) {
            Entity *currentEnemy = game->horde->next;
            while (currentEnemy->type != LIST_SENTINEL) {
                if (detectCollision(currentBullet, currentEnemy)) {
                    hit = true;
                    dropCheck = rand() % 100;
                    break;
                }
                currentEnemy = currentEnemy->next;
            }

            if (hit) {
                if (currentEnemy == game->hordeLastAlive) {
                    // Will be LIST_SENTINEL when the player win
                    game->hordeLastAlive = currentEnemy->prev;
                    if (game->hordeLastAlive->type == LIST_SENTINEL) {
                        game->hotData->gameState = WIN;
                        game->hotData->menuButton = RESTART;
                        PlaySound(game->sounds->victory);
                        return;
                    }
                }
                if (dropCheck < 100) {
                    generatePowerup(game->powerups, currentEnemy->bounds.x + currentEnemy->bounds.width/2.0f, currentEnemy->bounds.y + currentEnemy->bounds.height);
                }
                Entity *temp = currentBullet->prev;
                killBullet(currentBullet);
                killEnemy(currentEnemy);
                currentBullet = temp;
                PlaySound(game->sounds->enemyExplosion);
            } else {
                if (detectCollision(currentBullet, game->enemyShip)) {
                    dropCheck = rand() % 100;
                    if (dropCheck < 15) {
                        generatePowerup(game->powerups, game->enemyShip->bounds.x + game->enemyShip->bounds.width/2.0f, game->enemyShip->bounds.y + game->enemyShip->bounds.height);
                    }
                    Entity *temp = currentBullet->prev;
                    game->hotData->enemyShipActive = false;
                    game->hotData->enemyShipDefeated = true;
                    killBullet(currentBullet);
                    currentBullet = temp;
                    PlaySound(game->sounds->shipExplosion);
                }
            }
            currentBullet = currentBullet->next;
        } else {
            if (detectCollision(game->ship, currentBullet)) {
                game->hotData->gameState = LOSE;
                game->hotData->menuButton = RESTART;
                StopMusicStream(game->sounds->background);
                killBullet(currentBullet);
                PlaySound(game->sounds->shipExplosion);
                PlaySound(game->sounds->lose);
                return;
            } else {
                currentBullet = currentBullet->next;
            }
        }
    }

    Entity *currentPowerup = game->powerups->next;
    while (currentPowerup->type != LIST_SENTINEL) {
        if (detectCollision(game->ship, currentPowerup)) {
            if (currentPowerup->type == FAST_SHOT) {
                game->hotData->fastShotActive = true;
                game->hotData->fastShotRemainingTime = game->coldData->powerupDuration;
            } else {
                game->hotData->fastMoveActive = true;
                game->hotData->fastMoveRemainingTime = game->coldData->powerupDuration;
            }

            Entity *temp = currentPowerup;
            currentPowerup = currentPowerup->prev;
            killPowerup(temp);
            PlaySound(game->sounds->powerup);
        }
        currentPowerup = currentPowerup->next;
    }
}

void drawShip(Game *game) {
    if (game->hotData->gameState != LOSE) {
        Vector2 origin = {0.0f, 0.0f};
        DrawTexturePro(
            game->textures->ship,
            game->animation->shipFrame,
            game->ship->bounds,
            origin,
            0.0f,
            WHITE
        );
    }
}

void drawEnemyShip(Game *game) {
    if (!game->hotData->enemyShipDefeated && game->hotData->enemyShipActive) {
        Vector2 origin = {0.0f, 0.0f};
        DrawTexturePro(
            game->textures->enemyShip,
            game->animation->enemyShipFrame,
            game->enemyShip->bounds,
            origin,
            0.0f,
            WHITE
        );
    }
}

void drawAlien(Game *game, Entity *alien) {
    Vector2 origin = {0.0f, 0.0f};
    Texture2D currentTex;
    if (alien->alienType == TYPE1) currentTex = game->textures->alienFaster;
    else if (alien->alienType == TYPE2) currentTex = game->textures->alienFast;
    else currentTex = game->textures->alienSlow;

    DrawTexturePro(
        currentTex,
        game->animation->aliensFrame,
        alien->bounds,
        origin,
        0.0f,
        WHITE
    );
}

void drawHorde(Game *game) {
    Entity *currentAlien = game->horde->next;
    while (currentAlien->type != LIST_SENTINEL) {
        drawAlien(game, currentAlien);
        currentAlien = currentAlien->next;
    }
}

void drawBullet(Game *game, Entity *bullet) {
    Vector2 origin = {0.0f, 0.0f};
    DrawTexturePro(
        game->textures->bullet,
        game->animation->bulletFrame,
        bullet->bounds,
        origin,
        0.0f,
        WHITE
    );
}

void drawBullets(Game *game) {
    Entity *currentBullet = game->bullets->next;
    while (currentBullet->type != LIST_SENTINEL) {
        drawBullet(game, currentBullet);
        currentBullet = currentBullet->next;
    }
}

void drawPowerup(Game *game, Entity *powerup) {
    Vector2 origin = {0.0f, 0.0f};
    Texture2D currentTex;
    if (powerup->type == FAST_SHOT) currentTex = game->textures->shotPowerup;
    else currentTex = game->textures->movePowerup;
    DrawTexturePro(
        currentTex,
        game->animation->bulletFrame,
        powerup->bounds,
        origin,
        0.0f,
        WHITE
    );
}

void drawPowerups(Game *game) {
    Entity *currentPowerup = game->powerups->next;
    while (currentPowerup->type != LIST_SENTINEL) {
        drawPowerup(game, currentPowerup);
        currentPowerup = currentPowerup->next;
    }
}

void drawMenuBanner(Rectangle *banner) {
    Vector2 origin = {0.0f, 0.0f};

    DrawRectanglePro(
        *banner,
        origin,
        0.0f,
        WHITE
    );
}

void drawMenuButtons(Game *game, Rectangle *banner) {
    float sizeQuit = 80.0f, sizeStart = 80.0f, sizeRestart = 80.0f;
    float spacing = 5.0f;
    Font defaultFont = GetFontDefault();
    float topX = banner->x + banner->width/2.0f;
    float topY = banner->y + 50.0f;
    float bottomX = topX;
    float bottomY = banner->y + banner->height - 50.0f;

    if (game->hotData->menuButton == QUIT) {
        sizeQuit = 100.0f;
    } else if (game->hotData->menuButton == START) {
        sizeStart = 100.0f;
    } else {
        sizeRestart = 100.0f;
    }


    switch (game->hotData->gameState) {
        case MENU:
        {
            Vector2 dimensionsStart = MeasureTextEx(defaultFont, "START", sizeStart, spacing);
            Vector2 dimensionsQuit = MeasureTextEx(defaultFont, "QUIT", sizeQuit, spacing);
            topX -= dimensionsStart.x/2.0f;
            bottomX -= dimensionsQuit.x/2.0f;
            bottomY -= dimensionsQuit.y;
            DrawText("START", topX, topY, sizeStart, BLACK);
            DrawText("QUIT", bottomX, bottomY, sizeQuit, BLACK);
        } break;
        case WIN:
        case LOSE:
        {
            Vector2 dimensionsRestart = MeasureTextEx(defaultFont, "RESTART", sizeRestart, spacing);
            Vector2 dimensionsQuit = MeasureTextEx(defaultFont, "QUIT", sizeQuit, spacing);
            topX -= dimensionsRestart.x/2.0f;
            bottomX -= dimensionsQuit.x/2.0f;
            bottomY -= dimensionsQuit.y;
            DrawText("RESTART", topX, topY, sizeRestart, BLACK);
            DrawText("QUIT", bottomX, bottomY, sizeQuit, BLACK);
        } break;
        default: break;
    }
}

void drawMenu(Game *game) {
    const float height = 400.0f;
    const float width = 600.0f;
    const float x = (1920.0f - width)/2.0f;
    const float y = (1080.0f - height)/2.0f;
    Rectangle banner = {.height=height, .width=width, .x=x, .y=y};

    drawMenuBanner(&banner);
    drawMenuButtons(game, &banner);
}

void drawGame(Game *game) {
    ClearBackground(BLACK);
    DrawFPS(10, 10);
    drawShip(game);
    drawEnemyShip(game);
    drawHorde(game);
    drawBullets(game);
    drawPowerups(game);

    if (game->hotData->gameState != PLAYING) {
        drawMenu(game);
    }
}

void mainLoop() {
    Game game = initGame();

    while (game.hotData->gameState != CLOSE) {
        processInput(&game.hotData->input);
        updateGame(&game);
        BeginDrawing();
            drawGame(&game);
        EndDrawing();
    }

    cleanupGame(&game);
}
