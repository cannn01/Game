#include <graphics.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "Config.h"
#include "Globals.h"
#include "Draw.h"
#include "Game.h"
#include "Menu.h"

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

    int panelW = 520;
    int panelH = 520;
    int px = W/2 - panelW/2;
    int py = H/2 - panelH/2;

    myFillRect(px,py,panelW,panelH,DARKGRAY);
    myRect(px,py,panelW,panelH,YELLOW);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);

    drawTextCenter(
        W/2,
        py + 45,
        "LAST SURVIVOR",
        YELLOW
    );

    drawTextCenter(
        W/2,
        py + 85,
        "ZOMBIE CHAOS",
        LIGHTRED
    );

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    int bx = W/2 - 160;
    int by = py + 155;

    drawButton(bx,by,320,55,"START",mouseIn(bx,by,320,55));

    char d[80];

    if(difficulty == DIFF_EASY)
        strcpy(d,"DIFFICULTY: EASY");
    else if(difficulty == DIFF_NORMAL)
        strcpy(d,"DIFFICULTY: NORMAL");
    else
        strcpy(d,"DIFFICULTY: HARD");

    drawButton(bx,by+80,320,55,d,mouseIn(bx,by+80,320,55));

    char m[80];

    sprintf(m,"MUSIC: %s",musicOn ? "ON" : "OFF");

    drawButton(bx,by+160,320,55,m,mouseIn(bx,by+160,320,55));

    drawButton(bx,by+240,320,55,"EXIT",mouseIn(bx,by+240,320,55));

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    drawTextCenter(
        W/2,
        py + panelH - 45,
        "WASD MOVE | RIGHT MOUSE SHOOT | E DASH | ESC PAUSE",
        WHITE
    );

    setvisualpage(page);
    page = 1-page;
}

void updateMenu(){

    int panelH = 520;
    int py = H/2 - panelH/2;

    int bx = W/2 - 160;
    int by = py + 155;

    if(leftClick()){

        if(mouseIn(bx,by,320,55)){

            resetGame();

            gameState = STATE_PLAY;
        }

        else if(mouseIn(bx,by+80,320,55)){

            difficulty++;

            if(difficulty > DIFF_HARD)
                difficulty = DIFF_EASY;
        }

        else if(mouseIn(bx,by+160,320,55)){

            musicOn = !musicOn;
        }

        else if(mouseIn(bx,by+240,320,55)){

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
