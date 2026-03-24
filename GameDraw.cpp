#include <graphics.h>
#include "GameDraw.h"

// ---  Ve môi truong / Chuong ngai vat ---
void veBanDo() {
    // 1. Ve Tang dá lon 
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillellipse(650, 120, 60, 40);
    // Thêm chi tiet vân dá
    setcolor(LIGHTGRAY);
    arc(650, 120, 45, 135, 50); 
    arc(630, 110, 90, 180, 20);

    // 2. Ve Cây khô 
    setcolor(BROWN);
    setfillstyle(SOLID_FILL, BROWN);
    rectangle(600, 400, 620, 520); // Goc cây
    floodfill(610, 450, BROWN);
    // Ve các cành cây khô tua ra bang hàm line
    setcolor(BROWN);
    line(610, 430, 560, 380);
    line(610, 460, 660, 410);
    line(610, 410, 630, 350);

    // 3. Ve Cum Thùng go tiep te 
    // Thùng 1
    setcolor(BROWN);
    setfillstyle(CLOSE_DOT_FILL, BROWN); // Dùng pattern cham bi gia vân go
    rectangle(100, 450, 160, 510);
    floodfill(105, 455, BROWN);
    // Ve thanh chéo chu X trên thùng go
    setcolor(BLACK);
    line(100, 450, 160, 510);
    line(160, 450, 100, 510);
    rectangle(100, 450, 160, 510); // Vien thùng

    // Thùng 2 (Nam dè lên phía sau)
    setcolor(BROWN);
    setfillstyle(CLOSE_DOT_FILL, BROWN);
    rectangle(140, 420, 190, 470);
    floodfill(145, 425, BROWN);
    setcolor(BLACK);
    line(140, 420, 190, 470);
    line(190, 420, 140, 470);
    rectangle(140, 420, 190, 470);
}
// Hàm ve Nhân vat 
void veNhanVat(int x, int y) {
    // 1. Ve Balo (Sau lung nhân vat)
    setcolor(BROWN);
    setfillstyle(SOLID_FILL, BROWN);
    rectangle(x - 20, y - 15, x - 5, y + 15);
    floodfill(x - 10, y, BROWN);

    // 2. Ve Vai/Thân
    setcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    fillellipse(x, y, 15, 22);

    // 3. Ve Súng 
    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    rectangle(x + 10, y + 5, x + 45, y + 12);
    floodfill(x + 20, y + 8, LIGHTGRAY);

    // 4. Ve Tay cam súng (2 bàn tay)
    setcolor(YELLOW); // Màu da
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(x + 10, y - 10, 6, 6); // Tay trái
    fillellipse(x + 25, y + 8, 6, 6);  // Tay phai dat trên súng

    // 5. Ve Ðau 
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(x, y, 12, 12);

    // 6. Ðiem nhan: Bang rôn do trên dau
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    rectangle(x - 10, y - 4, x + 10, y + 2);
    floodfill(x, y - 1, RED);
}

// Hàm ve Zombie 
void veZombie(int x, int y) {
    // 1. Ve hai tay vuon ra phía truoc dòi can
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(x + 15, y - 12, 12, 5); // Tay trái
    fillellipse(x + 18, y + 12, 12, 5); // Tay phai 

    // 2. Ve Thân/Vai mac áo rách màu xám toi
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillellipse(x, y, 14, 20);

    // 3. Ve Ðau
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(x, y, 14, 14);

    // 4. Ve Mat (1 con mat do to, 1 con loi xech)
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x + 8, y - 5, 4, 4); // Mat trái to
    fillellipse(x + 6, y + 6, 2, 2); // Mat phai nho

    // 5. Thêm chi tiet bang Arc và Line 
    setcolor(LIGHTRED);
    arc(x - 4, y, 90, 270, 6);        // Mot vet can khuyet bên trái dau
    line(x - 2, y - 8, x + 4, y - 2); // Mot vet seo vat ngang dau
}

// Hàm ve Giao dien 
void veGiaoDien() {
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
    setcolor(YELLOW);
    outtextxy(250, 20, "UNDEAD SURVIVOR");   
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(WHITE);
    outtextxy(20, 550, "Di chuyen: WASD | Ban: Chuot trai");
    
    setcolor(LIGHTCYAN);
    rectangle(10, 10, 150, 80);
    outtextxy(20, 20, "SCORE: 0000");
    outtextxy(20, 45, "HP: [||||||||||]");
}
