#include <graphics.h>
#include <math.h>
#include <string.h>

#include "Config.h"
#include "Draw.h"

void myLine(int x1,int y1,int x2,int y2,int color){

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(true){

        putpixel(x1,y1,color);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy){
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx){
            err += dx;
            y1 += sy;
        }
    }
}

void circle8(int xc,int yc,int x,int y,int color){

    putpixel(xc+x,yc+y,color);
    putpixel(xc-x,yc+y,color);

    putpixel(xc+x,yc-y,color);
    putpixel(xc-x,yc-y,color);

    putpixel(xc+y,yc+x,color);
    putpixel(xc-y,yc+x,color);

    putpixel(xc+y,yc-x,color);
    putpixel(xc-y,yc-x,color);
}

void myCircle(int xc,int yc,int r,int color){

    int x = 0;
    int y = r;

    int p = 1 - r;

    circle8(xc,yc,x,y,color);

    while(x < y){

        x++;

        if(p < 0)
            p += 2*x + 1;
        else{
            y--;
            p += 2*(x-y) + 1;
        }

        circle8(xc,yc,x,y,color);
    }
}

void myRect(int x,int y,int w,int h,int color){

    myLine(x,y,x+w,y,color);

    myLine(x+w,y,x+w,y+h,color);

    myLine(x+w,y+h,x,y+h,color);

    myLine(x,y+h,x,y,color);
}

void myFillRect(int x,int y,int w,int h,int color){

    for(int i=y;i<=y+h;i++){

        myLine(x,i,x+w,i,color);
    }
}

void drawKoch(int n,float x,float y,float len,float dir,int color){

    if(n == 0){

        float x2 = x + len*cos(dir*PI/180);
        float y2 = y + len*sin(dir*PI/180);

        myLine((int)x,(int)y,(int)x2,(int)y2,color);

        return;
    }

    float l = len/3;

    drawKoch(n-1,x,y,l,dir,color);

    x += l*cos(dir*PI/180);
    y += l*sin(dir*PI/180);

    drawKoch(n-1,x,y,l,dir-60,color);

    x += l*cos((dir-60)*PI/180);
    y += l*sin((dir-60)*PI/180);

    drawKoch(n-1,x,y,l,dir+60,color);

    x += l*cos((dir+60)*PI/180);
    y += l*sin((dir+60)*PI/180);

    drawKoch(n-1,x,y,l,dir,color);
}

void drawTextCenter(int x,int y,const char *s,int color){

    setcolor(color);

    outtextxy(
        x - textwidth((char*)s)/2,
        y,
        (char*)s
    );
}

void drawButton(int x,int y,int w,int h,const char *text,int hover){

    int border = hover ? YELLOW : WHITE;
    int fill = hover ? BLUE : BLACK;

    myFillRect(x,y,w,h,fill);

    myRect(x,y,w,h,border);
    myRect(x+3,y+3,w-6,h-6,border);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    drawTextCenter(
        x+w/2,
        y+h/2-8,
        text,
        border
    );

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}
