#ifndef DRAW_H
#define DRAW_H

void myLine(int x1,int y1,int x2,int y2,int color);

void myCircle(int xc,int yc,int r,int color);

void myRect(int x,int y,int w,int h,int color);

void myFillRect(int x,int y,int w,int h,int color);

void drawKoch(int n,float x,float y,float len,float dir,int color);

void drawTextCenter(int x,int y,const char *s,int color);

void drawButton(int x,int y,int w,int h,const char *text,int hover);

#endif
