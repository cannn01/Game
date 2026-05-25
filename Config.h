#ifndef CONFIG_H
#define CONFIG_H

#define MAX_BULLETS 260
#define MAX_ENEMIES 120
#define MAX_OBS 40

#define PI 3.14159265f

enum GameState {
    STATE_MENU,
    STATE_PLAY,
    STATE_PAUSE,
    STATE_GAMEOVER
};

enum Difficulty {
    DIFF_EASY = 0,
    DIFF_NORMAL = 1,
    DIFF_HARD = 2
};

#endif
