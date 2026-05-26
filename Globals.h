#ifndef GLOBALS_H
#define GLOBALS_H

#include "Config.h"
#include "Types.h"

extern int W, H;
extern int page;
extern int highScore;
extern int musicOn;
extern int difficulty;
extern GameState gameState;

extern Player p;
extern Bullet bullets[MAX_BULLETS];
extern Enemy enemies[MAX_ENEMIES];
extern Obstacle obs[MAX_OBS];
extern BoxItem boxItem;
extern Portal portal;

extern int obsCount;
extern int bossSpawned;
extern float spawnTimer;
extern float boxTimer;
extern Effect effects[MAX_EFFECTS];
extern int gameTick;
#endif
