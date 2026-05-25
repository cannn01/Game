#include <graphics.h>
#include <windows.h>

#include "Config.h"
#include "Globals.h"
#include "Game.h"
#include "Menu.h"

int main(){

    initGameSystem();

    static int oldEsc = 0;

    while(true){

        if(gameState == STATE_MENU){

            updateMenu();

            renderMenu();
        }

        else if(gameState == STATE_PLAY){

            int esc =
                GetAsyncKeyState(VK_ESCAPE)
                & 0x8000;

            if(esc && !oldEsc)
                gameState = STATE_PAUSE;

            oldEsc = esc;

            updateGame();

            renderGame();
        }

        else if(gameState == STATE_PAUSE){

            updatePause();

            renderPause();

            oldEsc =
                GetAsyncKeyState(VK_ESCAPE)
                & 0x8000;
        }

        else if(gameState == STATE_GAMEOVER){

            updateGameOver();

            renderGameOver();
        }

        delay(16);
    }

    closegraph();

    return 0;
}
