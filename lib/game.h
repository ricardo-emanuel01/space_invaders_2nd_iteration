# ifndef _GAME_H_
# define _GAME_H_

# include <stdlib.h>
# include "entity.h"
# include "raylib.h"
# include "raymath.h"


typedef enum GameState {
    MENU,
    PLAYING,
    PAUSED,
    LOSE,
    WIN,
    CLOSE,
} GameState;

typedef enum Ship {
    REGULAR,
    BUFFED,
} Ship;

typedef enum MenuButton {
    QUIT,
    START,
    RESTART,
} MenuButton;

typedef struct Input {
    bool left;
    bool right;
    bool fire;
    bool select;
    bool up;
    bool down;
    bool pause;
} Input;

typedef struct ColdGameData {
    float shipSpeeds[2];
    float shipDelaysToFire[2];
    float screenLimits[2];
    float enemyShipDelayToFire;
    float enemyShipSpeed;
    float projectileSpeed;
    float powerupDuration;
    float animationDuration;
    float hordeSpeedIncrease;
    float hordeStepY;
    float enemyShipSleepTime;
} ColdGameData;

typedef struct HotGameData {
    double fastShotRemainingTime;
    double fastMoveRemainingTime;
    double remainingTimeEnemyShipAlarm;
    double lastFrameTime;
    double shipLastShotTime;
    double enemyShipLastShotTime;
    float hordeSpeed;
    GameState gameState;
    MenuButton menuButton;
    Input input;
    bool fastShotActive;
    bool fastMoveActive;
    bool enemyShipGoingLeft;
    bool enemyShipDefeated;
    bool enemyShipActive;
} HotGameData;

typedef struct Sounds {
    Music background;
    Music enemyShip;
    Sound shipFire;
    Sound enemyFire;
    Sound shipExplosion;
    Sound enemyExplosion;
    Sound powerup;
    Sound lose;
    Sound victory;
    Sound menu;
} Sounds;

typedef struct Textures {
    Texture2D ship;
    Texture2D enemyShip;
    Texture2D alienSlow;
    Texture2D alienFast;
    Texture2D alienFaster;
    Texture2D bullet;
    Texture2D shotPowerup;
    Texture2D movePowerup;
} Textures;

typedef struct Animation {
    Rectangle aliensFrame;
    Rectangle shipFrame;
    Rectangle bulletFrame;
    Rectangle enemyShipFrame;
    Rectangle powerupFrame;
    int enemyCurrentFrame;
} Animation;

typedef struct Game {
    Entity *ship;
    Entity *enemyShip;
    Entity *horde;
    Entity *hordeLastAlive;
    Entity *bullets;
    Entity *powerups;
    ColdGameData *coldData;
    HotGameData *hotData;
    Sounds *sounds;
    Textures *textures;
    Animation *animation;
} Game;

ColdGameData *initColdGameData();

HotGameData *initHotGameData();

void cleanupGameData(ColdGameData *, HotGameData *);

Sounds *initSounds();

void cleanupSounds(Sounds *);

Textures *initTextures();

void cleanupTextures(Textures *);

Animation *initAnimation();

void cleanupAnimation(Animation *);

Game initGame();

void fire(Game *, Entity *);

void detectCollisions(Game *);

void mainLoop();

# endif