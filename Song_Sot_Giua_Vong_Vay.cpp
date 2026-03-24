#include <graphics.h>
#include "GameDraw.h" 

int main() {
    initwindow(800, 600, "Undead Survivor: Classic Vector - Chuong 1");
    
    setbkcolor(BLACK);
    cleardevice();

    veBanDo();
    
    veNhanVat(400, 300);
    
    veZombie(300, 150);
    veZombie(450, 450);
    veZombie(100, 250);

    veGiaoDien();

    getch();
    closegraph();
    return 0;
}
