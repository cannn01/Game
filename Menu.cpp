#include <graphics.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "Config.h"
#include "Globals.h"
#include "Draw.h"
#include "Game.h"
#include "Menu.h"
#include "Sound.h"
int mouseIn(int x,int y,int w,int h){

    int mx = mousex();
    int my = mousey();

    return (
        mx >= x &&
        mx <= x+w &&
        my >= y &&
        my <= y+h
    );
}

int leftClick(){

    static int old = 0;

    int now =
        GetAsyncKeyState(VK_LBUTTON) & 0x8000;

    int clicked = now && !old;

    old = now;

    return clicked;
}

void renderMenu(){

    setactivepage(page);
    setbkcolor(BLACK);
    cleardevice();

    int panelW = 820;
    int panelH = 620;
    int px = W/2 - panelW/2;
    int py = H/2 - panelH/2;

    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(px,py,px+panelW,py+panelH);

    setcolor(YELLOW);
    rectangle(px,py,px+panelW,py+panelH);
    rectangle(px+8,py+8,px+panelW-8,py+panelH-8);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    drawTextCenter(W/2,py+55,"LAST SURVIVOR",YELLOW);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    drawTextCenter(W/2,py+105,"ZOMBIE CHAOS",LIGHTRED);

    int bx = W/2 - 230;
    int by = py + 180;

    drawButton(bx,by,460,65,"START",mouseIn(bx,by,460,65));

    char diffText[100];

    if(difficulty == DIFF_EASY)
        sprintf(diffText,"DIFFICULTY EASY");
    else if(difficulty == DIFF_NORMAL)
        sprintf(diffText,"DIFFICULTY NORMAL");
    else
        sprintf(diffText,"DIFFICULTY HARD");

    drawButton(bx,by+90,460,65,diffText,mouseIn(bx,by+90,460,65));

    char musicText[100];
    sprintf(musicText,"MUSIC %s",musicOn ? "ON" : "OFF");

    drawButton(bx,by+180,460,65,musicText,mouseIn(bx,by+180,460,65));

    drawButton(bx,by+270,460,65,"EXIT",mouseIn(bx,by+270,460,65));

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    drawTextCenter(W/2,py+panelH-45,"WASD MOVE | RIGHT MOUSE SHOOT | E DASH | ESC PAUSE",WHITE);

    setvisualpage(page);
    page = 1-page;
}

void updateMenu(){

    int panelH = 620;
    int py = H/2 - panelH/2;

    int bx = W/2 - 230;
    int by = py + 180;

    if(leftClick()){

        if(mouseIn(bx,by,460,65)){
            resetGame();
            playMapMusic(1);
            gameState = STATE_PLAY;
        }

        else if(mouseIn(bx,by+90,460,65)){
            difficulty++;
            if(difficulty > DIFF_HARD)
                difficulty = DIFF_EASY;
        }

        else if(mouseIn(bx,by+180,460,65)){
            musicOn = !musicOn;

            if(!musicOn)
                stopMusic();
            else
                playMenuMusic();
        }

        else if(mouseIn(bx,by+270,460,65)){
            closegraph();
            exit(0);
        }
    }
}

void renderPause(){

    setactivepage(page);

    setbkcolor(BLACK);

    cleardevice();

    drawTextCenter(
        W/2,
        130,
        "GAME PAUSED",
        YELLOW
    );

    int bx = W/2 - 130;
    int by = 230;

    drawButton(
        bx,
        by,
        260,
        55,
        "CONTINUE",
        mouseIn(bx,by,260,55)
    );

    drawButton(
        bx,
        by+80,
        260,
        55,
        "EXIT TO MENU",
        mouseIn(bx,by+80,260,55)
    );

    setvisualpage(page);

    page = 1-page;
}

void updatePause(){

    static int oldEsc = 0;

    int esc =
        GetAsyncKeyState(VK_ESCAPE) & 0x8000;

    if(esc && !oldEsc)
        gameState = STATE_PLAY;

    oldEsc = esc;

    int bx = W/2 - 130;
    int by = 230;

    if(leftClick()){

        if(mouseIn(bx,by,260,55))
            gameState = STATE_PLAY;

        if(mouseIn(bx,by+80,260,55))
            gameState = STATE_MENU;
    }
}

void renderGameOver(){

    setactivepage(page);

    setbkcolor(BLACK);

    cleardevice();

    char s[100];

    drawTextCenter(
        W/2,
        150,
        "GAME OVER",
        RED
    );

    sprintf(s,"Score: %d",p.score);

    drawTextCenter(
        W/2,
        205,
        s,
        WHITE
    );

    sprintf(s,"High Score: %d",highScore);

    drawTextCenter(
        W/2,
        235,
        s,
        YELLOW
    );

    int bx = W/2 - 130;
    int by = 310;

    drawButton(
        bx,
        by,
        260,
        55,
        "BACK TO MENU",
        mouseIn(bx,by,260,55)
    );

    drawButton(
        bx,
        by+80,
        260,
        55,
        "EXIT",
        mouseIn(bx,by+80,260,55)
    );

    setvisualpage(page);

    page = 1-page;
}

void updateGameOver(){

    int bx = W/2 - 130;
    int by = 310;

    if(leftClick()){

        if(mouseIn(bx,by,260,55))
            gameState = STATE_MENU;

        if(mouseIn(bx,by+80,260,55)){

            closegraph();

            exit(0);
        }
    }
}
