#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H
#include <SDL2/SDL.h>

#define D_UP 0
#define D_RIGHT 1
#define D_DOWN 2
#define D_LEFT 3

#define P_NORMAL 0
#define P_DYING 1

#define E_NORMAL 0
#define E_WEAK 1
#define E_DEAD 2

#define GRAPE 0
#define ORANGE 1
#define BANANA 2
#define APPLE 3

typedef struct TD_LOC {
  int i;
  int j;
} loc;

typedef struct SPRITYPE {
  SDL_Surface* textures[4][4];
  int direction;
  int x, y;
  int frame;
  int state;
  int frameOrder[6]; //="0,1,2,3,2,1";
} sprite;

typedef struct FRUITT {
  SDL_Surface* fruitTextures[4];
  int direction;
  int x, y;
  int frame;
  int fruitScore[4];
  int hasGoal;
  int hasLTGoal;
  int LTgx, LTgy;
  int state;
  int gx, gy;
  int fruiti;
  int valid;
} FRUIT;

typedef struct ENEMY {
  SDL_Surface* textures[3][4];
  int direction;
  int type;
  int x, y;
  int frame;
  int frameOrder[6];
  int hasGoal;
  int hasLTGoal;
  int LTgx, LTgy;
  int state;
  int gx, gy;

  //="0,1,2,3,2,1";
} enemy;

typedef struct FLOATING_SCORE {
  int score;
  int digits[10];
  int digitCount;
  int x, y;
  int framesLeft;
  //="0,1,2,3,2,1";
} fscore;

typedef struct TILE_TYPE {
  int valid;
  char ul;
  char ur;
  char bl;
  char br;
} tile;

typedef struct QUEUE_TYPE {
  int start, end;
  loc data[500];
  int size;
} nqueue;

#define G_WIDTH 28
#define G_HEIGHT 22

#define Y_OFFSET 20
#define X_OFFSET 40

#define UL_IF 0
#define UL_OF 1
#define UR_IF 2
#define UR_OF 3
#define BL_IF 4
#define BL_OF 5
#define BR_IF 6
#define BR_OF 7
#define H_UF 8
#define H_BF 9
#define V_LF 10
#define V_RF 11
#define FILLER 12
#define BIGFOOD 13
#define SMALLFOOD 14
#define DOOR_UL 15
#define DOOR_UR 16
#define DOOR_BL 17
#define DOOR_BR 18

#define NUM_TILES 19

#define SCREEN_WIDTH 640  // 96//640 // size of screen
#define SCREEN_HEIGHT 480 // 160//480

#endif
