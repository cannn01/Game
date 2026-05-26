#ifndef TYPES_H
#define TYPES_H

struct Bullet {
    int active;
    float x, y, vx, vy, life;
};

struct Enemy {
    int active;
    int type;
    int boss;
    int hp, maxHp;
    float x, y;
    float speed;
    float atkCd;
    float skillCd;
};

struct Obstacle {
    int x, y, w, h;
    int type;
};

struct BoxItem {
    int active;
    int hp;
    int x, y;
    int buff;
};

struct Portal {
    int active;
    int x, y;
};

struct Player {
    float x, y;
    float angle;
    float speed;
    float hp, maxHp;

    float fireCd;
    float fireTimer;
    int bulletCount;

    int score;
    int map;
    int alive;

    float slowTimer;
    float burnTimer;


    float dashCd;
    float dashTimer;
    int dashKeyOld;
    
    int shootKeyOld;
	int mouseShootOld;
};
struct Effect {
    int active;
    int type;
    float x, y;
    float vx, vy;
    float life, maxLife;
    int color;
    int size;
};
#endif
