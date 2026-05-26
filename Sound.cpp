#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#include "Globals.h"
#include "Sound.h"

char MUSIC_FOLDER[] = "E:\\225\\THDoHoaMayTinh\\Game\\";

void stopMusic(){

    mciSendString("stop bgm", NULL, 0, NULL);
    mciSendString("close bgm", NULL, 0, NULL);
}

void playFile(const char *name){

    if(!musicOn)
        return;

    stopMusic();

    char fullPath[300];
    char cmd[400];

    sprintf(fullPath, "%s%s", MUSIC_FOLDER, name);

    DWORD attr = GetFileAttributes(fullPath);

    if(attr == INVALID_FILE_ATTRIBUTES){
        MessageBox(NULL, fullPath, "KHONG TIM THAY FILE NHAC", MB_OK);
        return;
    }

    sprintf(cmd, "open \"%s\" type mpegvideo alias bgm", fullPath);
    mciSendString(cmd, NULL, 0, NULL);

    mciSendString("play bgm repeat", NULL, 0, NULL);
}

void playMenuMusic(){

    playFile("menu.mp3");
}

void playMapMusic(int map){

    if(map == 1) playFile("map1.mp3");
    if(map == 2) playFile("map2.mp3");
    if(map == 3) playFile("map3.mp3");
    if(map == 4) playFile("map4.mp3");
}
