#include "Globals.h"

int W = 1280;
int H = 720;
int page = 0;

int highScore = 0;
int musicOn = 1;
int difficulty = DIFF_NORMAL;

GameState gameState = STATE_MENU;

Player p;
Bullet bullets[MAX_BULLETS];
Enemy enemies[MAX_ENEMIES];
Obstacle obs[MAX_OBS];
BoxItem boxItem;
Portal portal;

int obsCount = 0;
int bossSpawned = 0;
float spawnTimer = 0;
float boxTimer = 0;
