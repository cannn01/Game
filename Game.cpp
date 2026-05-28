#include <graphics.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Config.h"
#include "Types.h"
#include "Globals.h"
#include "Draw.h"
#include "Game.h"
#include "Sound.h"

#define C_BG_DARK       DARKGRAY
#define C_BG_ICE        BLUE
#define C_BG_LAVA       BLACK

#define C_OUTLINE       BLACK
#define C_SHADOW        DARKGRAY

#define C_ZOMBIE        GREEN
#define C_ZOMBIE_DARK   DARKGRAY
#define C_ZOMBIE_EYE    RED

#define C_PLAYER        BLUE
#define C_PLAYER_ARMOR  LIGHTGRAY
#define C_GUN           DARKGRAY
#define C_GUN_LIGHT     LIGHTGRAY

#define C_WOOD          BROWN
#define C_WOOD_DARK     DARKGRAY
#define C_TREE_1        GREEN
#define C_TREE_2        LIGHTGREEN
#define C_TREE_3        GREEN

#define C_BLOOD         RED
#define C_ICE           CYAN
#define C_FIRE          LIGHTRED

void drawShadow(int x,int y,int rx,int ry);
void spawnBuffGlow(float x,float y);
void spawnBlood(float x,float y);
float dist2(float x1,float y1,float x2,float y2){

    float dx = x1 - x2;
    float dy = y1 - y2;

    return sqrt(dx*dx + dy*dy);
}

float difficultyHpMul(){

    if(difficulty == DIFF_EASY)
        return 0.8f;

    if(difficulty == DIFF_HARD)
        return 1.65f;

    return 1.0f;
}

float difficultySpeedMul(){

    if(difficulty == DIFF_EASY)
        return 0.9f;

    if(difficulty == DIFF_HARD)
        return 1.25f;

    return 1.0f;
}void loadHighScore(){

    FILE *f = fopen("highscore.txt","r");

    if(f){

        fscanf(f,"%d",&highScore);

        fclose(f);
    }
}

void saveHighScore(){

    if(p.score > highScore){

        FILE *f = fopen("highscore.txt","w");

        if(f){

            fprintf(f,"%d",p.score);

            fclose(f);
        }

        highScore = p.score;
    }
}

void clearArrays(){

    for(int i=0;i<MAX_BULLETS;i++)
        bullets[i].active = 0;

    for(int i=0;i<MAX_ENEMIES;i++)
        enemies[i].active = 0;
        
	for(int i=0;i<MAX_EFFECTS;i++)
    	effects[i].active = 0;
    boxItem.active = 0;

    portal.active = 0;

    obsCount = 0;

    bossSpawned = 0;
}

void addObs(int x,int y,int w,int h,int type){

    if(obsCount < MAX_OBS){

        obs[obsCount].x = x;
        obs[obsCount].y = y;

        obs[obsCount].w = w;
        obs[obsCount].h = h;

        obs[obsCount].type = type;

        obsCount++;
    }
}

void setupMap(int map){

    clearArrays();

    p.map = map;

    p.x = W/2;
    p.y = H/2;

    spawnTimer = 15;

    boxTimer = 500;

    if(map == 1){

        addObs(90,140,190,80,1);

        addObs(450,120,80,150,2);

        addObs(850,240,150,95,3);

        addObs(300,510,220,85,1);

        addObs(650,500,120,70,3);
    }

    if(map == 2){

        addObs(80,160,180,70,4);

        addObs(470,330,150,100,4);

        addObs(780,130,190,90,5);

        addObs(330,540,190,70,5);

        addObs(900,520,130,70,4);
    }

    if(map == 3){

        addObs(150,130,160,110,6);

        addObs(510,210,150,120,7);

        addObs(820,420,210,90,7);

        addObs(250,540,180,80,6);

        addObs(650,110,120,80,7);
    }

    if(map == 4){

        addObs(90,140,150,75,1);

        addObs(330,190,170,90,4);

        addObs(610,340,180,90,7);

        addObs(850,170,150,105,6);

        addObs(285,540,190,70,5);

        addObs(760,540,160,80,3);
    }
    playMapMusic(map);
}



int hitObs(float x,float y,int r){

    for(int i=0;i<obsCount;i++){

        if(
            x+r > obs[i].x &&
            x-r < obs[i].x + obs[i].w &&
            y+r > obs[i].y &&
            y-r < obs[i].y + obs[i].h
        )
            return 1;
    }

    return 0;
}

void resetGame(){

    p.maxHp = 100;

    p.hp = 100;

    p.speed = 4.2f;

    p.fireCd = 12;

    p.fireTimer = 0;

    p.bulletCount = 1;

    p.score = 0;

    p.alive = 1;

    p.slowTimer = 0;

    p.burnTimer = 0;

    p.dashCd = 0;

    p.dashTimer = 0;
    
    p.dashKeyOld = 0;

	p.shootKeyOld = 0;

	p.mouseShootOld = 0;
    setupMap(1);
}

void initGameSystem(){

    srand((unsigned)time(NULL));

    W = GetSystemMetrics(SM_CXSCREEN);

    H = GetSystemMetrics(SM_CYSCREEN);

    initwindow(
        W,
        H,
        (char*)"LAST SURVIVOR: ZOMBIE CHAOS"
    );

	
    loadHighScore();
}


void spawnEnemy(int boss){

    for(int i=0;i<MAX_ENEMIES;i++){

        if(!enemies[i].active){

            Enemy *e = &enemies[i];

            e->active = 1;

            e->boss = boss;

            e->type = 1;

            if(p.map == 4)
                e->type = 1 + rand()%3;
            else
                e->type = p.map;

            int side = rand()%4;

            if(side == 0){
                e->x = 20;
                e->y = rand()%H;
            }

            if(side == 1){
                e->x = W - 20;
                e->y = rand()%H;
            }

            if(side == 2){
                e->x = rand()%W;
                e->y = 20;
            }

            if(side == 3){
                e->x = rand()%W;
                e->y = H - 20;
            }

            if(boss){

                e->hp = e->maxHp =
                    (int)((420 + p.map*180) * difficultyHpMul());

                e->speed = 1.05f * difficultySpeedMul();

                e->skillCd = 120;
            }
            else{

                int special = rand()%4 == 0;

                int baseHp = special ? 80 : 38;

                e->hp = e->maxHp =
                    (int)(baseHp * difficultyHpMul());

                e->speed =
                    (special ? 2.05f : 1.25f) * difficultySpeedMul();

                if(special)
                    e->type += 10;
            }

            e->atkCd = 0;

            return;
        }
    }
}


void shoot(){

    if(p.fireTimer > 0)
        return;

    p.fireTimer = p.fireCd;

    int mx = mousex();
    int my = mousey();

    float ang = atan2(
        (float)my - p.y,
        (float)mx - p.x
    );

    for(int b=0;b<p.bulletCount;b++){

        float spread =
            (b - (p.bulletCount-1)/2.0f) * 0.18f;

        for(int i=0;i<MAX_BULLETS;i++){

            if(!bullets[i].active){

                bullets[i].active = 1;

                bullets[i].x = p.x + cos(ang) * 28;
                bullets[i].y = p.y + sin(ang) * 28;

                bullets[i].vx = cos(ang + spread) * 12;
                bullets[i].vy = sin(ang + spread) * 12;

                bullets[i].life = 75;

                break;
            }
        }
    }
}


void spawnBox(){

    boxItem.active = 1;

    boxItem.hp = 2;

    boxItem.x = 80 + rand()%(W-160);

    boxItem.y = 80 + rand()%(H-160);

    boxItem.buff = rand()%4;
}

void applyBuff(int type){

    if(type == 0){

        p.maxHp += 20;

        p.hp += 25;

        if(p.hp > p.maxHp)
            p.hp = p.maxHp;
    }

    if(type == 1){

        p.fireCd -= 2;

        if(p.fireCd < 6)
            p.fireCd = 6;
    }

    if(type == 2){

        p.bulletCount++;

        if(p.bulletCount > 3)
            p.bulletCount = 3;
    }

    if(type == 3){

        p.speed += 0.35f;

        if(p.speed > 6.2f)
            p.speed = 6.2f;
    }
    spawnBuffGlow(p.x,p.y);
}


void dash(){

    if(p.dashCd > 0)
        return;

    float a = atan2((float)mousey() - p.y, (float)mousex() - p.x);

    float dashDistance = 420;
    float step = 25;
    float moved = 0;

    while(moved < dashDistance){

        float nx = p.x + cos(a) * step;
        float ny = p.y + sin(a) * step;

        if(nx < 35 || nx > W-35 || ny < 35 || ny > H-35)
            break;

        if(hitObs(nx,ny,18))
            break;

        p.x = nx;
        p.y = ny;

        moved += step;
    }

    p.dashCd = 180;
    p.dashTimer = 28;
}

void handleShootInput(){

    int mouseNow = 0;
    int spaceNow = 0;

    if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        mouseNow = 1;

    if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
        mouseNow = 1;

    if(GetAsyncKeyState(VK_SPACE) & 0x8000)
        spaceNow = 1;

    if(mouseNow || spaceNow){
        shoot();
    }

    p.mouseShootOld = mouseNow;
    p.shootKeyOld = spaceNow;
}
void addEffect(int type,float x,float y,int color,int size){

    for(int i=0;i<MAX_EFFECTS;i++){

        if(!effects[i].active){

            effects[i].active = 1;
            effects[i].type = type;
            effects[i].x = x;
            effects[i].y = y;

            effects[i].vx = (rand()%100 - 50) / 25.0f;
            effects[i].vy = (rand()%100 - 50) / 25.0f;

            effects[i].life = 35 + rand()%25;
            effects[i].maxLife = effects[i].life;

            effects[i].color = color;
            effects[i].size = size;

            return;
        }
    }
}

void spawnBlood(float x,float y){

    for(int i=0;i<12;i++)
        addEffect(1,x,y,RED,3 + rand()%4);
}

void spawnBuffGlow(float x,float y){

    for(int i=0;i<18;i++)
        addEffect(2,x,y,LIGHTMAGENTA,4 + rand()%5);
}

void updateEffects(){

    for(int i=0;i<MAX_EFFECTS;i++){

        if(!effects[i].active)
            continue;

        effects[i].x += effects[i].vx;
        effects[i].y += effects[i].vy;

        effects[i].life--;

        if(effects[i].life <= 0)
            effects[i].active = 0;
    }
}

void drawEffects(){

    for(int i=0;i<MAX_EFFECTS;i++){

        if(!effects[i].active)
            continue;

        int s = effects[i].size;

        if(effects[i].type == 1){

            setfillstyle(SOLID_FILL,effects[i].color);
            fillellipse(
                (int)effects[i].x,
                (int)effects[i].y,
                s,
                s
            );
        }

        if(effects[i].type == 2){

            myCircle(
                (int)effects[i].x,
                (int)effects[i].y,
                s,
                effects[i].color
            );
        }
    }
}

void updatePlayer(){

    float sp = p.speed;

    if(p.slowTimer > 0){
        sp *= 0.55f;
        p.slowTimer--;
    }

    if(p.burnTimer > 0){
        p.hp -= 0.08f;
        p.burnTimer--;
    }

    if(p.dashCd > 0)
        p.dashCd--;

    if(p.dashTimer > 0)
        p.dashTimer--;

    p.angle = atan2(
        (float)mousey() - p.y,
        (float)mousex() - p.x
    );

    handleShootInput();

    int eNow = GetAsyncKeyState('E') & 0x8000;

    if(eNow && !p.dashKeyOld){
        dash();
    }

    p.dashKeyOld = eNow;

    float nx = p.x;
    float ny = p.y;

    if(GetAsyncKeyState('W') & 0x8000)
        ny -= sp;

    if(GetAsyncKeyState('S') & 0x8000)
        ny += sp;

    if(GetAsyncKeyState('A') & 0x8000)
        nx -= sp;

    if(GetAsyncKeyState('D') & 0x8000)
        nx += sp;

    if(nx < 20)
        nx = 20;

    if(nx > W-20)
        nx = W-20;

    if(ny < 20)
        ny = 20;

    if(ny > H-20)
        ny = H-20;

    if(!hitObs(nx,ny,15)){
        p.x = nx;
        p.y = ny;
    }

    if(p.fireTimer > 0)
        p.fireTimer--;

    if(p.hp <= 0){
        p.alive = 0;
        saveHighScore();
        gameState = STATE_GAMEOVER;
    }
}


void updateBullets(){

    for(int i=0;i<MAX_BULLETS;i++){

        if(!bullets[i].active)
            continue;

        bullets[i].x += bullets[i].vx;
        bullets[i].y += bullets[i].vy;

        bullets[i].life--;

        if(
            bullets[i].x < 0 ||
            bullets[i].x > W ||
            bullets[i].y < 0 ||
            bullets[i].y > H ||
            bullets[i].life <= 0 ||
            hitObs(bullets[i].x,bullets[i].y,3)
        ){
            bullets[i].active = 0;
            continue;
        }

        if(
            boxItem.active &&
            dist2(
                bullets[i].x,
                bullets[i].y,
                boxItem.x,
                boxItem.y
            ) < 25
        ){
            bullets[i].active = 0;

            boxItem.hp--;

            if(boxItem.hp <= 0){

                applyBuff(boxItem.buff);

                boxItem.active = 0;
            }

            continue;
        }

        for(int j=0;j<MAX_ENEMIES;j++){

            if(
                enemies[j].active &&
                dist2(
                    bullets[i].x,
                    bullets[i].y,
                    enemies[j].x,
                    enemies[j].y
                ) < (enemies[j].boss ? 42 : 20)
            ){

                bullets[i].active = 0;

                enemies[j].hp -= 25;
				spawnBlood(enemies[j].x,enemies[j].y);
                if(enemies[j].hp <= 0){

                    if(enemies[j].boss){

                        p.score += 100;

                        portal.active = 1;

                        portal.x = W/2;
                        portal.y = 85;
                    }
                    else{

                        if(enemies[j].type >= 10)
                            p.score += 25;
                        else
                            p.score += 10;
                    }

                    enemies[j].active = 0;
                }

                break;
            }
        }
    }
}


void bossSkill(Enemy *e){

    if(e->skillCd > 0){

        e->skillCd--;

        return;
    }

    if(p.map == 1){

        if(dist2(e->x,e->y,p.x,p.y) < 95){

            p.hp -= 18;

            p.slowTimer = 120;

            float a = atan2(
                p.y - e->y,
                p.x - e->x
            );

            p.x += cos(a) * 30;
            p.y += sin(a) * 30;
        }

        e->skillCd = 480;
    }

    if(p.map == 2){

        if(dist2(e->x,e->y,p.x,p.y) < 160){

            p.hp -= 13;

            p.slowTimer = 190;
        }

        e->skillCd = 600;
    }

    if(p.map == 3){

        if(rand()%2 == 0){

            p.hp -= 14;

            p.burnTimer = 180;
        }

        e->skillCd = 720;
    }

    if(p.map == 4){

        if(rand()%2 == 0)
            p.slowTimer = 150;
        else
            p.burnTimer = 150;

        p.hp -= 12;

        e->skillCd = 540;
    }
}

void updateEnemies(){

    for(int i=0;i<MAX_ENEMIES;i++){

        Enemy *e = &enemies[i];

        if(!e->active)
            continue;

        float dx = p.x - e->x;
        float dy = p.y - e->y;

        float d = sqrt(dx*dx + dy*dy);

        if(d > 0){

            float nx = e->x + dx/d * e->speed;
            float ny = e->y + dy/d * e->speed;

            if(hitObs(nx,ny,e->boss ? 34 : 15)){

                nx = e->x - dy/d * e->speed * 0.7f;
                ny = e->y + dx/d * e->speed * 0.7f;
            }

            if(!hitObs(nx,ny,e->boss ? 34 : 15)){

                e->x = nx;

                e->y = ny;
            }
        }

        if(e->boss)
            bossSkill(e);

        if(d < (e->boss ? 50 : 25)){

            if(e->atkCd <= 0){

                if(e->boss)
                    p.hp -= 18;
                else
                    p.hp -= 8;

                if(e->type == 2 || e->type == 12)
                    p.slowTimer = 120;

                if(e->type == 3 || e->type == 13)
                    p.burnTimer = 150;

                e->atkCd = 42;
            }
        }

        if(e->atkCd > 0)
            e->atkCd--;
    }
}


void updateSpawn(){

    spawnTimer--;

    if(spawnTimer <= 0){

        int count = 1;

        if(p.map >= 2)
            count = 2;

        if(p.map == 4)
            count = 3 + p.score/400;

        for(int i=0;i<count;i++)
            spawnEnemy(0);

        spawnTimer = 58 - (p.map * 8);

        if(difficulty == DIFF_HARD)
            spawnTimer -= 10;

        if(spawnTimer < 18)
            spawnTimer = 18;
    }

    boxTimer--;

    if(boxTimer <= 0 && !boxItem.active){

        spawnBox();

        boxTimer = 900 + rand()%500;
    }

    int need = 100;

    if(p.map == 2)
        need = 150;

    if(p.map == 3)
        need = 200;

    if(p.map < 4 && p.score >= need && !bossSpawned){

        spawnEnemy(1);

        bossSpawned = 1;
    }

    if(
        p.map == 4 &&
        p.score > 0 &&
        p.score % 700 < 15 &&
        rand()%50 == 0
    ){
        spawnEnemy(1);
    }

    if(
        portal.active &&
        dist2(p.x,p.y,portal.x,portal.y) < 40
    ){
        if(p.map < 4)
            setupMap(p.map + 1);
    }
}

void updateGame(){

    gameTick++;

    updatePlayer();

    updateBullets();

    updateEnemies();

    updateSpawn();

    updateEffects();
}


void drawBackground(){

    if(p.map == 1)
        setbkcolor(DARKGRAY);
    if(p.map == 2)
        setbkcolor(BLUE);
    if(p.map == 3)
        setbkcolor(BLACK);
    if(p.map == 4)
        setbkcolor(DARKGRAY);

    cleardevice();

    int i;

    /* MAP 1: thanh pho do nat */
    if(p.map == 1){

        setfillstyle(SOLID_FILL,DARKGRAY);
        bar(0,0,W,H);

        /* vach duong cu */
        for(i=0;i<W;i+=180){
            setcolor(LIGHTGRAY);
            line(i, H/2, i+70, H/2);
        }

        /* vet nut nen */
        for(i=0;i<25;i++){

            int x = (i*137)%W;
            int y = (i*83)%H;

            setcolor(BLACK);

            line(x,y,x+25,y+8);
            line(x+25,y+8,x+45,y-6);
            line(x+45,y-6,x+65,y+14);
        }

        /* da vu nho */
        for(i=0;i<70;i++){

            int x = (i*91)%W;
            int y = (i*61)%H;

            setcolor(LIGHTGRAY);

            putpixel(x,y,LIGHTGRAY);
            putpixel(x+2,y+1,LIGHTGRAY);
            putpixel(x-1,y+2,LIGHTGRAY);
        }

        /* vet mau */
        for(i=0;i<8;i++){

            int x = (i*211)%W;
            int y = (i*97)%H;

            setfillstyle(SOLID_FILL,RED);

            fillellipse(x,y,16,6);
            fillellipse(x+12,y+4,7,4);
        }

        /* tuong gach do nat */
        for(i=0;i<7;i++){

            int x = (i*240)%W;
            int y = (i*130)%H;

            setfillstyle(SOLID_FILL,BROWN);
            bar(x,y,x+95,y+26);

            setcolor(BLACK);
            rectangle(x,y,x+95,y+26);

            line(x,y+13,x+95,y+13);
            line(x+20,y,x+20,y+26);
            line(x+50,y,x+50,y+26);
            line(x+75,y,x+75,y+26);
        }

        drawKoch(2,30,H-80,150,0,GREEN);
    }
	/* MAP 2: bang gia */
    if(p.map == 2){

        // Ð?i BLUE thành LIGHTBLUE d? màu n?n d?u và l?nh hon
        setfillstyle(SOLID_FILL, LIGHTBLUE); 
        bar(0,0,W,H);

        for(i=0;i<35;i++){

            int x = (i*101)%W;
            int y = (i*67)%H;

            setcolor(WHITE);
            line(x-6,y,x+6,y);
            line(x,y-6,x,y+6);
        }

        for(i=0;i<12;i++){

            int x = (i*233)%W;
            int y = (i*119)%H;

            setcolor(LIGHTCYAN);
            ellipse(x,y,0,360,45,18);

            setcolor(WHITE);
            ellipse(x,y,0,360,30,10);
        }

        drawKoch(3,40,H-70,170,0,WHITE);
    }
    

    /* MAP 3: nui lua */
    if(p.map == 3){

        setfillstyle(SOLID_FILL,BLACK);
        bar(0,0,W,H);

        for(i=0;i<35;i++){

            int x = (i*163)%W;
            int y = (i*71)%H;

            setcolor(DARKGRAY);
            line(x,y,x+30,y-45);
            line(x+30,y-45,x+60,y);
            line(x,y,x+60,y);

            setcolor(RED);
            line(x+30,y-25,x+30,y-5);
        }

        for(i=0;i<20;i++){

            int x = (i*187)%W;
            int y = (i*97)%H;

            setfillstyle(SOLID_FILL,RED);
            fillellipse(x,y,22,8);

            setcolor(YELLOW);
            ellipse(x,y,0,360,30,12);
        }

        drawKoch(2,30,H-70,170,0,RED);
    }

    /* MAP 4: hon loan */
    if(p.map == 4){

        setfillstyle(SOLID_FILL,DARKGRAY);
        bar(0,0,W,H);

        for(i=0;i<20;i++){

            int x = (i*157)%W;
            int y = (i*93)%H;

            setcolor(BLACK);
            line(x,y,x+30,y+10);
            line(x+30,y+10,x+60,y-5);
        }

        for(i=0;i<12;i++){

            int x = (i*203)%W;
            int y = (i*113)%H;

            setcolor(CYAN);
            ellipse(x,y,0,360,35,12);

            setcolor(RED);
            ellipse(x+60,y+30,0,360,30,10);
        }

        drawKoch(2,30,H-70,130,0,GREEN);
        drawKoch(2,230,H-70,130,0,LIGHTCYAN);
        drawKoch(2,430,H-70,130,0,RED);
    }
}

void drawObs(){

    for(int i=0;i<obsCount;i++){

        int x = obs[i].x;
        int y = obs[i].y;
        int w = obs[i].w;
        int h = obs[i].h;
        int type = obs[i].type;

        /* type 1: thung kim loai */
        if(type == 1){

            drawShadow(x+w/2,y+h+5,w/2,7);

            setfillstyle(SOLID_FILL,LIGHTGRAY);
            bar(x,y,x+w,y+h);

            setcolor(BLACK);
            rectangle(x,y,x+w,y+h);
            rectangle(x+6,y+6,x+w-6,y+h-6);

            line(x,y,x+w,y+h);
            line(x+w,y,x,y+h);

            setcolor(WHITE);
            line(x+8,y+8,x+w-8,y+8);
        }

        /* type 2: thung go / container nho */
        else if(type == 2){

            drawShadow(x+w/2,y+h+5,w/2,7);

            setfillstyle(SOLID_FILL,BROWN);
            bar(x,y,x+w,y+h);

            setcolor(BLACK);
            rectangle(x,y,x+w,y+h);

            setcolor(YELLOW);
            rectangle(x+8,y+8,x+w-8,y+h-8);

            line(x+8,y+8,x+w-8,y+h-8);
            line(x+w-8,y+8,x+8,y+h-8);
        }

        /* type 3: cay */
        else if(type == 3){

            int trunkX = x + w/2;
            int trunkTop = y + h/2;
            int trunkBottom = y + h + 35;

            /* chi co 1 bong duoi goc cay */
            drawShadow(trunkX,trunkBottom+4,42,8);

            /* than cay */
            setfillstyle(SOLID_FILL,BROWN);
            bar(trunkX-8,trunkTop,trunkX+8,trunkBottom);

            setcolor(BLACK);
            rectangle(trunkX-8,trunkTop,trunkX+8,trunkBottom);

            /* tan la nhieu lop */
            setfillstyle(SOLID_FILL,GREEN);
            fillellipse(trunkX-22,y+h/2,32,24);

            setfillstyle(SOLID_FILL,LIGHTGREEN);
            fillellipse(trunkX+18,y+h/2-8,32,24);

            setfillstyle(SOLID_FILL,GREEN);
            fillellipse(trunkX,y+h/2-25,36,28);

            setcolor(BLACK);
            ellipse(trunkX,y+h/2-10,0,360,45,35);
        }

        /* type 4/5: bang */
        else if(type == 4 || type == 5){

            drawShadow(x+w/2,y+h+5,w/2,7);

            setfillstyle(SOLID_FILL,CYAN);
            bar(x,y,x+w,y+h);

            setcolor(BLACK);
            rectangle(x,y,x+w,y+h);

            setcolor(WHITE);
            line(x,y+h,x+w/2,y);
            line(x+w/2,y,x+w,y+h);
            line(x,y,x+w,y+h);

            setcolor(BLUE);
            rectangle(x+6,y+6,x+w-6,y+h-6);
        }

        /* type 6: da nui lua */
        else if(type == 6){

            drawShadow(x+w/2,y+h+5,w/2,7);

            setfillstyle(SOLID_FILL,DARKGRAY);
            bar(x,y,x+w,y+h);

            setcolor(BLACK);
            rectangle(x,y,x+w,y+h);

            setcolor(LIGHTGRAY);
            line(x,y+h,x+w/2,y);
            line(x+w/2,y,x+w,y+h);

            setcolor(RED);
            line(x+15,y+h-15,x+w/2,y+10);
        }

        /* type 7: dung nham */
        else if(type == 7){

            drawShadow(x+w/2,y+h/2+12,w/2,6);

            setfillstyle(SOLID_FILL,RED);
            fillellipse(x+w/2,y+h/2,w/2,h/2);

            setfillstyle(SOLID_FILL,LIGHTRED);
            fillellipse(x+w/2,y+h/2,w/2-14,h/2-10);

            setcolor(YELLOW);
            ellipse(x+w/2,y+h/2,0,360,w/2-10,h/2-10);

            setcolor(BLACK);
            ellipse(x+w/2,y+h/2,0,360,w/2,h/2);
        }
    }
}

void drawShadow(int x,int y,int rx,int ry){

    setfillstyle(SOLID_FILL,BLACK);

    fillellipse(x,y,rx,ry);
}

void drawPlayer(){

    float a = p.angle;

    float fx = cos(a);
    float fy = sin(a);

    float rx = cos(a + PI/2);
    float ry = sin(a + PI/2);

    int cx = (int)p.x;
    int cy = (int)p.y;

    /* bong */
    drawShadow(cx,cy+22,18,6);

    /* body */
    setfillstyle(SOLID_FILL,BLUE);

    fillellipse(cx,cy,13,16);

    setcolor(BLACK);

    ellipse(cx,cy,0,360,13,16);

    /* giap */
    setfillstyle(SOLID_FILL,DARKGRAY);

    fillellipse(cx,cy+1,8,10);

    /* balo */
    setfillstyle(SOLID_FILL,BROWN);

    fillellipse(
        cx - fx*10,
        cy - fy*10,
        7,
        9
    );

    /* dau */
    int headX = cx;
    int headY = cy-18;

    setfillstyle(SOLID_FILL,BLUE);

    fillellipse(
        headX,
        headY,
        11,
        11
    );

    setcolor(BLACK);

    ellipse(
        headX,
        headY,
        0,
        360,
        11,
        11
    );

    /* visor */
    setfillstyle(SOLID_FILL,LIGHTGRAY);

    bar(
        headX-8,
        headY-2,
        headX+8,
        headY+5
    );

    setcolor(BLACK);

    rectangle(
        headX-8,
        headY-2,
        headX+8,
        headY+5
    );

    /* mat */
    setfillstyle(SOLID_FILL,WHITE);

    fillellipse(headX-4,headY+1,2,2);
    fillellipse(headX+4,headY+1,2,2);

    setfillstyle(SOLID_FILL,BLACK);

    fillellipse(headX-4,headY+1,1,1);
    fillellipse(headX+4,headY+1,1,1);

    /* chan */
    setcolor(BLACK);

    setlinestyle(SOLID_LINE,0,THICK_WIDTH);

    line(cx-5,cy+12,cx-8,cy+22);
    line(cx+5,cy+12,cx+8,cy+22);

    /* vai */
    int shoulderX = cx + fx*5;
    int shoulderY = cy + fy*5;

    /* tay */
    int handX = cx + fx*18;
    int handY = cy + fy*18;

    line(
        cx-rx*5,
        cy-ry*5,
        handX-rx*4,
        handY-ry*4
    );

    line(
        cx+rx*5,
        cy+ry*5,
        handX+rx*4,
        handY+ry*4
    );

    /* sung */
    int gunX1 = cx + fx*14;
    int gunY1 = cy + fy*14;

    int gunX2 = cx + fx*38;
    int gunY2 = cy + fy*38;

    int sx = rx*3;
    int sy = ry*3;

    int poly[10];

    poly[0]=gunX1+sx;
    poly[1]=gunY1+sy;

    poly[2]=gunX2+sx;
    poly[3]=gunY2+sy;

    poly[4]=gunX2-sx;
    poly[5]=gunY2-sy;

    poly[6]=gunX1-sx;
    poly[7]=gunY1-sy;

    poly[8]=poly[0];
    poly[9]=poly[1];

    setfillstyle(SOLID_FILL,DARKGRAY);

    fillpoly(5,poly);

    setcolor(BLACK);

    drawpoly(5,poly);

    /* nong */
    setcolor(LIGHTGRAY);

    line(
        gunX2,
        gunY2,
        gunX2+fx*8,
        gunY2+fy*8
    );

    /* flash */
    if(p.fireTimer > p.fireCd-3){

        setfillstyle(SOLID_FILL,YELLOW);

        fillellipse(
            gunX2+fx*10,
            gunY2+fy*10,
            4,
            4
        );
    }

    setlinestyle(SOLID_LINE,0,NORM_WIDTH);
}

void drawBullets(){

    for(int i=0;i<MAX_BULLETS;i++){

        if(bullets[i].active){

            myLine(
                (int)(bullets[i].x - bullets[i].vx*0.8f),
                (int)(bullets[i].y - bullets[i].vy*0.8f),
                (int)bullets[i].x,
                (int)bullets[i].y,
                YELLOW
            );

            myCircle(
                (int)bullets[i].x,
                (int)bullets[i].y,
                3,
                WHITE
            );
        }
    }
}

void drawEnemies(){

    for(int i=0;i<MAX_ENEMIES;i++){

        if(!enemies[i].active)
            continue;

        Enemy *e = &enemies[i];

        int cx = (int)e->x;
        int cy = (int)e->y;

        int boss = e->boss;

        int r = boss ? 48 : 20;

        int skin = GREEN;
        int cloth = DARKGRAY;

        if(e->type == 2 || e->type == 12){

            skin = LIGHTCYAN;
            cloth = BLUE;
        }

        if(e->type == 3 || e->type == 13){

            skin = LIGHTRED;
            cloth = BROWN;
        }

        if(e->type >= 10 && !boss){

            skin = MAGENTA;
            cloth = DARKGRAY;
        }

        if(boss){

            skin = BROWN;
            cloth = DARKGRAY;
        }

        int walk =
            (int)(sin((gameTick+i)*0.25f)*6);

        drawShadow(
            cx,
            cy+r+8,
            boss ? 40 : 20,
            boss ? 12 : 6
        );

        /* chân */
        setcolor(BLACK);

        setlinestyle(SOLID_LINE,0,THICK_WIDTH);

        line(cx-7,cy+r-5,cx-18,cy+r+20+walk);
        line(cx+7,cy+r-5,cx+18,cy+r+20-walk);

        /* tay */
        line(cx-r+5,cy,cx-r-18,cy+12);
        line(cx+r-5,cy,cx+r+18,cy+12);

        /* thân */
        setfillstyle(SOLID_FILL,cloth);

        fillellipse(cx,cy,r,r+10);

        setcolor(BLACK);

        ellipse(cx,cy,0,360,r,r+10);

        /* d?u */
        setfillstyle(SOLID_FILL,skin);

        fillellipse(cx,cy-r/2-8,r-3,r-1);

        setcolor(BLACK);

        ellipse(cx,cy-r/2-8,0,360,r-3,r-1);

        /* m?t */
        setfillstyle(SOLID_FILL,WHITE);

        fillellipse(cx-6,cy-r/2-12,3,3);
        fillellipse(cx+6,cy-r/2-12,3,3);

        setfillstyle(SOLID_FILL,RED);

        fillellipse(cx-6,cy-r/2-12,1,1);
        fillellipse(cx+6,cy-r/2-12,1,1);

        /* mi?ng */
        setcolor(BLACK);

        line(
            cx-8,
            cy-r/2,
            cx+8,
            cy-r/2
        );

        /* s?o */
        setcolor(RED);

        line(
            cx-10,
            cy-r/2-18,
            cx-2,
            cy-r/2-8
        );

        /* aura */
        if(e->type == 2 || e->type == 12)
            myCircle(cx,cy,r+8,LIGHTCYAN);

        if(e->type == 3 || e->type == 13)
            myCircle(cx,cy,r+8,LIGHTRED);

        if(e->type >= 10 && !boss)
            myCircle(cx,cy,r+10,YELLOW);

        if(boss)
            myCircle(cx,cy,r+16,YELLOW);

        setlinestyle(SOLID_LINE,0,NORM_WIDTH);

        /* máu */
        int barW = boss ? 150 : 50;

        setfillstyle(SOLID_FILL,BLACK);

        bar(
            cx-barW/2,
            cy-r-42,
            cx+barW/2,
            cy-r-30
        );

        setfillstyle(SOLID_FILL,RED);

        bar(
            cx-barW/2,
            cy-r-42,
            cx-barW/2 +
            (int)(barW*(float)e->hp/e->maxHp),
            cy-r-30
        );

        setcolor(WHITE);

        rectangle(
            cx-barW/2,
            cy-r-42,
            cx+barW/2,
            cy-r-30
        );
    }
}

void drawBoxPortal(){

    if(boxItem.active){

        int glow = 28 + (gameTick%20);

        myCircle(boxItem.x,boxItem.y,glow,LIGHTMAGENTA);

        setfillstyle(SOLID_FILL,BROWN);
        bar(boxItem.x-22,boxItem.y-22,boxItem.x+22,boxItem.y+22);

        setcolor(YELLOW);
        rectangle(boxItem.x-22,boxItem.y-22,boxItem.x+22,boxItem.y+22);

        line(boxItem.x-22,boxItem.y,boxItem.x+22,boxItem.y);
        line(boxItem.x,boxItem.y-22,boxItem.x,boxItem.y+22);
    }

    if(portal.active){

        int r = 35 + (gameTick%15);

        myCircle(portal.x,portal.y,r,LIGHTMAGENTA);
        myCircle(portal.x,portal.y,r-10,MAGENTA);
        myCircle(portal.x,portal.y,r-20,WHITE);

        settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
        outtextxy(portal.x-45,portal.y-70,(char*)"PORTAL");
        settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
    }
}


void drawHUD(){

    char s[160];

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);

    setfillstyle(SOLID_FILL, BLACK);
    bar(15,15,390,150);

    setcolor(WHITE);
    rectangle(15,15,390,150);

    setfillstyle(SOLID_FILL, RED);
    bar(35,35,35 + (int)(300*p.hp/p.maxHp),65);

    setcolor(WHITE);
    rectangle(35,35,335,65);

    sprintf(s,"HP %.0f / %.0f",p.hp,p.maxHp);
    outtextxy(35,80,s);

    setfillstyle(SOLID_FILL, BLACK);
    bar(W-390,15,W-20,145);

    setcolor(WHITE);
    rectangle(W-390,15,W-20,145);

    sprintf(s,"SCORE %d",p.score);
    outtextxy(W-360,35,s);

    sprintf(s,"MAP %d",p.map);
    outtextxy(W-360,80,s);

    setfillstyle(SOLID_FILL, BLACK);
    bar(15,H-160,430,H-20);

    setcolor(WHITE);
    rectangle(15,H-160,430,H-20);

    sprintf(s,"BULLET %d",p.bulletCount);
    outtextxy(35,H-135,s);

    if(p.dashCd <= 0)
        sprintf(s,"E DASH READY");
    else
        sprintf(s,"E DASH %.1fs",p.dashCd/60.0f);

    outtextxy(35,H-90,s);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

void drawBossSkillWarning(){

    for(int i=0;i<MAX_ENEMIES;i++){

        if(!enemies[i].active || !enemies[i].boss)
            continue;

        Enemy *e = &enemies[i];

        if(e->skillCd < 60){

            if(p.map == 1){

                myCircle((int)e->x,(int)e->y,95,RED);
                myCircle((int)e->x,(int)e->y,75,LIGHTRED);
            }

            if(p.map == 2){

                myCircle((int)e->x,(int)e->y,160,LIGHTCYAN);
                myCircle((int)e->x,(int)e->y,120,WHITE);
            }

            if(p.map == 3){

                myCircle((int)p.x,(int)p.y,60,RED);
                myLine(p.x-60,p.y,p.x+60,p.y,RED);
                myLine(p.x,p.y-60,p.x,p.y+60,RED);
            }

            if(p.map == 4){

                myCircle((int)e->x,(int)e->y,130,MAGENTA);
            }
        }
    }
}

void renderGame(){

    setactivepage(page);

    drawBackground();

    drawObs();

    drawBoxPortal();

    drawEffects();

    drawBullets();

    drawEnemies();

    drawBossSkillWarning();

    drawPlayer();

    drawHUD();

    setvisualpage(page);

    page = 1 - page;
}
