// pacman.cpp : Defines the entry point for the application.
//

#include "MyTimer.h"
#include "compat.h"
#include "globals.h"
#include "level.h"
#include "sound.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <stdio.h>
#include <unordered_map>

FILE* debug;

SDL_Window* window = nullptr;
SDL_Surface* surface = nullptr; // dd primary surface
SDL_Renderer* renderer = nullptr;
std::unordered_map<int, bool> keys;

SDL_Surface* test = NULL;
SDL_Surface* tright = NULL;
SDL_Surface* digits[10];
SDL_Surface* bigDigits[10];
SDL_Surface* pacmanTitle = NULL;
SDL_Surface* exitItem = NULL;
SDL_Surface* startItem = NULL;
SDL_Surface* returnItem = NULL;
SDL_Surface* menuBack = NULL;
SDL_Surface* hiScoreItem = NULL;

SDL_Surface* font[26];

char** levelNames;
char** levelPaths;

char levelName[40];
int levelNumber = 0;
int levelCount = 0;

int (*Game_Main_Func)(void);
int Enemy_Init(int ei);
void DrawFPS();

int gcount = 0;
int weak;
int scores[2];
int lives[] = {2, 2};
int multiple = 1;

MyTimer timer;
Level* level;

FRUIT fruit;
sprite pacman;
enemy enemies[8];
int numEnemies = 4;
fscore score;

int Level_Intro();
int Level_Finished();
int Game_Over();
int Game_Main();
void fillFloatingScore(int);

void fetchLevel() {
  FILE* file = NULL;
  char temp[300];

  // DSound_Stop_All_Sounds();
  fruit.valid = 0;

  sprintf(temp, "%s/level.txt", levelPaths[levelNumber]);
  level->loadLevel(temp);
  sprintf(temp, "%s/walls.bmp", levelPaths[levelNumber]);
  if ((file = fopen(temp, "r")) != NULL) {
    std::cout << "Loading walls:" << temp << std::endl;
    test = SDL_LoadBMP(temp);
    level->fillWallTextures(test);
    SDL_FreeSurface(test);
  }
}

void placePacman() {
  pacman.x = 40;
  pacman.y = 20;
}

void placeEnemies() {
  int i = 0;
  while (i < numEnemies) {
    Enemy_Init(i);
    i++;
  }
  enemies[0].x = level->safe[0].j * 20;
  enemies[0].y = level->safe[0].i * 20;
  enemies[1].x = level->safe[1].j * 20;
  enemies[1].y = level->safe[1].i * 20;
  enemies[2].x = level->safe[2].j * 20;
  enemies[2].y = level->safe[2].i * 20;
  enemies[3].x = level->safe[3].j * 20;
  enemies[3].y = level->safe[3].i * 20;
}

int moveFruit() {
  /*
  int dir;
  if(fruit.valid)
  {
          if(fruit.hasGoal)
          {
                  if(fruit.x==fruit.gx
                          && fruit.y==fruit.gy)
                  {
                          fruit.hasGoal=0;
                  }
          }
          if(!fruit.hasGoal)
          {
                  dir=level->findPath(fruit.y/20,fruit.x/20,
                          level->fruitEnd.i,level->fruitEnd.j);
  
                  if(dir==-1)
                          return 1;
                  fruit.hasGoal=1;
                  fruit.direction = dir;
                  switch(dir)
                  {
                  case D_UP:
                          fruit.gy= (fruit.y/20 - 1)*20;
                          fruit.gx = fruit.x;
                          break;
                  case D_DOWN:
                          fruit.gy = (fruit.y/20 + 1)*20;
                          fruit.gx = fruit.x;
                          break;
                  case D_RIGHT:
                          fruit.gy = fruit.y;
                          fruit.gx = (fruit.x/20 + 1)*20;
                          break;
                  case D_LEFT:
                          fruit.gy= fruit.y;
                          fruit.gx = (fruit.x/20 - 1)*20;
                          break;
                  }
          }
          if(fruit.hasGoal)
          switch(fruit.direction)
                  {
                  case D_UP:
                          fruit.y--;
                          break;
                  case D_DOWN:
                          fruit.y++;
                          break;
                  case D_RIGHT:
                          fruit.x++;
                          break;
                  case D_LEFT:
                          fruit.x--;
                          break;
                  }
  }
  
  return 1;
  */
  int dir;
  static int index = 0;
  static int steps = 0;
  if (fruit.valid) {
    if (fruit.hasLTGoal) {
      if (fruit.x == fruit.LTgx && fruit.y == fruit.LTgy) {
        fruit.hasLTGoal = 0;
        fruit.valid = 0;
      }
    }
    if (!fruit.hasLTGoal) {
      index = 0;
      steps = 0;

      if (-1 != level->findPath(fruit.y / 20, fruit.x / 20, level->fruitEnd.i,
                                level->fruitEnd.j)) {
        fruit.hasLTGoal = 1;
        fruit.LTgx = level->fruitEnd.j * 20;
        fruit.LTgy = level->fruitEnd.i * 20;
      }
    }
    if (fruit.hasLTGoal) {
      dir = level->path[index];
      steps++;
      if (steps >= 20) {
        steps = 0;
        index++;
      }
      if (dir == -1)
        return 1;
      fruit.hasGoal = 1;
      fruit.direction = dir;
      switch (dir) {
      case D_UP:
        fruit.gy = (fruit.y / 20 - 1) * 20;
        fruit.gx = fruit.x;
        break;
      case D_DOWN:
        fruit.gy = (fruit.y / 20 + 1) * 20;
        fruit.gx = fruit.x;
        break;
      case D_RIGHT:
        fruit.gy = fruit.y;
        fruit.gx = (fruit.x / 20 + 1) * 20;
        break;
      case D_LEFT:
        fruit.gy = fruit.y;
        fruit.gx = (fruit.x / 20 - 1) * 20;
        break;
      }
    }
    if (fruit.hasGoal)
      switch (fruit.direction) {
      case D_UP:
        fruit.y--;
        break;
      case D_DOWN:
        fruit.y++;
        break;
      case D_RIGHT:
        fruit.x++;
        break;
      case D_LEFT:
        fruit.x--;
        break;
      }
  }

  return 1;
}

inline int moveEnemy(
    int ei) { /*
                     int dir=level->findPath(enemies[0].y/20,enemies[0].x/20,
                                     pacman.y/20,pacman.x/20);
                     switch(dir)
                     {
                             case D_UP:
                                     enemies[ei].y--;
                                     break;
                             case D_DOWN:
                                     enemies[ei].y++;
                                     break;
                             case D_RIGHT:
                                     enemies[ei].x++;
                                     break;
                             case D_LEFT:
                                     enemies[ei].x--;
                                     break;
                             default:
                                     break;
                     }*/
              ///	level->findPath(1,1,3,10);

  int dir;
  if (enemies[ei].state == E_NORMAL && ei == 1) {
    if (enemies[ei].hasLTGoal) {
      if (enemies[ei].x == enemies[ei].LTgx &&
          enemies[ei].y == enemies[ei].LTgy) {
        enemies[ei].hasLTGoal = 0;
      }
    }

    if (!enemies[ei].hasLTGoal) {
      int ii = 0;
      int index = rand() % level->foodLocCount;
      enemies[ei].hasLTGoal = 1;
      enemies[ei].LTgx = level->foodLocJs[index] * 20;
      enemies[ei].LTgy = level->foodLocIs[index] * 20;
      fprintf(debug, "Getting LTGOAL, it is %d,%d, level->foodLocCount = %d\n",
              enemies[ei].LTgx, enemies[ei].LTgy, level->foodLocCount);
      level->foodLocI++;

      if (level->foodLocI > level->foodLocCount) {
        level->foodLocI = 0;
      }
    }
    if (enemies[ei].hasLTGoal) {
      if (enemies[ei].hasGoal) {
        if (enemies[ei].x == enemies[ei].gx &&
            enemies[ei].y == enemies[ei].gy) {
          enemies[ei].hasGoal = 0;
        }
      }
      if (!enemies[ei].hasGoal) {
        dir = level->findPath2(enemies[ei].y / 20, enemies[ei].x / 20,
                               enemies[ei].LTgy / 20, enemies[ei].LTgx / 20);

        if (dir == -1)
          return 1;
        enemies[ei].hasGoal = 1;
        enemies[ei].direction = dir;
        switch (dir) {
        case D_UP:
          enemies[ei].gy = (enemies[ei].y / 20 - 1) * 20;
          enemies[ei].gx = enemies[ei].x;
          break;
        case D_DOWN:
          enemies[ei].gy = (enemies[ei].y / 20 + 1) * 20;
          enemies[ei].gx = enemies[ei].x;
          break;
        case D_RIGHT:
          enemies[ei].gy = enemies[ei].y;
          enemies[ei].gx = (enemies[ei].x / 20 + 1) * 20;
          break;
        case D_LEFT:
          enemies[ei].gy = enemies[ei].y;
          enemies[ei].gx = (enemies[ei].x / 20 - 1) * 20;
          break;
        }
      }
      if (enemies[ei].hasGoal) {
        switch (enemies[ei].direction) {
        case D_UP:
          enemies[ei].y--;
          break;
        case D_DOWN:
          enemies[ei].y++;
          break;
        case D_RIGHT:
          enemies[ei].x++;
          break;
        case D_LEFT:
          enemies[ei].x--;
          break;
        }
      }
    }
    return 1;
  }
  if (enemies[ei].state == E_NORMAL) {

    if (enemies[ei].hasGoal) {
      if (enemies[ei].x == enemies[ei].gx && enemies[ei].y == enemies[ei].gy) {
        enemies[ei].hasGoal = 0;
      }
    }
    if (!enemies[ei].hasGoal) {
      dir = level->findPath2(enemies[ei].y / 20, enemies[ei].x / 20,
                             pacman.y / 20, pacman.x / 20);

      if (dir == -1)
        return 1;
      enemies[ei].hasGoal = 1;
      enemies[ei].direction = dir;
      switch (dir) {
      case D_UP:
        enemies[ei].gy = (enemies[ei].y / 20 - 1) * 20;
        enemies[ei].gx = enemies[ei].x;
        break;
      case D_DOWN:
        enemies[ei].gy = (enemies[ei].y / 20 + 1) * 20;
        enemies[ei].gx = enemies[ei].x;
        break;
      case D_RIGHT:
        enemies[ei].gy = enemies[ei].y;
        enemies[ei].gx = (enemies[ei].x / 20 + 1) * 20;
        break;
      case D_LEFT:
        enemies[ei].gy = enemies[ei].y;
        enemies[ei].gx = (enemies[ei].x / 20 - 1) * 20;
        break;
      }
    }
    if (enemies[ei].hasGoal)
      switch (enemies[ei].direction) {
      case D_UP:
        enemies[ei].y--;
        break;
      case D_DOWN:
        enemies[ei].y++;
        break;
      case D_RIGHT:
        enemies[ei].x++;
        break;
      case D_LEFT:
        enemies[ei].x--;
        break;
      }
  } else if (enemies[ei].state == E_WEAK) {
    if (enemies[ei].hasLTGoal) {
      if (enemies[ei].x == enemies[ei].LTgx &&
          enemies[ei].y == enemies[ei].LTgy) {
        enemies[ei].hasLTGoal = 0;
      }
    }

    if (!enemies[ei].hasLTGoal) {
      int rj = rand() % (G_WIDTH);
      int ri = rand() % (G_HEIGHT);
      /*
                      Find a random free square
      */
      int done = 0;
      while (!done) {
        if (level->checkGrid(ri, rj) != 1) {
          if (-1 !=
              level->findPath2(enemies[ei].y / 20, enemies[ei].x / 20, ri, rj))
            break;
        }
        rj = rand() % (G_WIDTH);
        ri = rand() % (G_HEIGHT);
      }

      enemies[ei].hasLTGoal = 1;
      enemies[ei].LTgx = rj * 20;
      enemies[ei].LTgy = ri * 20;
      fprintf(debug, "Enemy[%d] new LTGoal=[%d,%d]\n", ei, enemies[ei].LTgx,
              enemies[ei].LTgy);
    }
    if (enemies[ei].hasLTGoal) {
      if (enemies[ei].hasGoal) {
        if (enemies[ei].x == enemies[ei].gx &&
            enemies[ei].y == enemies[ei].gy) {
          enemies[ei].hasGoal = 0;
        }
      }
      if (!enemies[ei].hasGoal) {
        dir = level->findPath2(enemies[ei].y / 20, enemies[ei].x / 20,
                               enemies[ei].LTgy / 20, enemies[ei].LTgx / 20);

        if (dir == -1)
          return 1;
        enemies[ei].hasGoal = 1;
        enemies[ei].direction = dir;
        switch (dir) {
        case D_UP:
          enemies[ei].gy = (enemies[ei].y / 20 - 1) * 20;
          enemies[ei].gx = enemies[ei].x;
          break;
        case D_DOWN:
          enemies[ei].gy = (enemies[ei].y / 20 + 1) * 20;
          enemies[ei].gx = enemies[ei].x;
          break;
        case D_RIGHT:
          enemies[ei].gy = enemies[ei].y;
          enemies[ei].gx = (enemies[ei].x / 20 + 1) * 20;
          break;
        case D_LEFT:
          enemies[ei].gy = enemies[ei].y;
          enemies[ei].gx = (enemies[ei].x / 20 - 1) * 20;
          break;
        }
      }
      if (enemies[ei].hasGoal) {
        switch (enemies[ei].direction) {
        case D_UP:
          enemies[ei].y--;
          break;
        case D_DOWN:
          enemies[ei].y++;
          break;
        case D_RIGHT:
          enemies[ei].x++;
          break;
        case D_LEFT:
          enemies[ei].x--;
          break;
        }
      }
    }
  } else if (enemies[ei].state == E_DEAD) {
    if (enemies[ei].hasLTGoal) {
      if (enemies[ei].x == enemies[ei].LTgx &&
          enemies[ei].y == enemies[ei].LTgy) {
        enemies[ei].hasLTGoal = 0;
        enemies[ei].state = E_NORMAL;
      }
    }

    if (!enemies[ei].hasLTGoal) {
      int ri, rj;

      ri = level->safe[ei].i;
      rj = level->safe[ei].j;

      enemies[ei].hasLTGoal = 1;
      enemies[ei].LTgx = rj * 20;
      enemies[ei].LTgy = ri * 20;
      fprintf(debug, "Enemy[%d] new LTGoal=[%d,%d]\n", ei, enemies[ei].LTgx,
              enemies[ei].LTgy);
    }
    if (enemies[ei].hasLTGoal) {
      if (enemies[ei].hasGoal) {
        if (enemies[ei].x == enemies[ei].gx &&
            enemies[ei].y == enemies[ei].gy) {
          enemies[ei].hasGoal = 0;
        }
      }
      if (!enemies[ei].hasGoal) {
        dir = level->findPath2(enemies[ei].y / 20, enemies[ei].x / 20,
                               enemies[ei].LTgy / 20, enemies[ei].LTgx / 20);

        if (dir == -1)
          return 1;
        enemies[ei].hasGoal = 1;
        enemies[ei].direction = dir;
        switch (dir) {
        case D_UP:
          enemies[ei].gy = (enemies[ei].y / 20 - 1) * 20;
          enemies[ei].gx = enemies[ei].x;
          break;
        case D_DOWN:
          enemies[ei].gy = (enemies[ei].y / 20 + 1) * 20;
          enemies[ei].gx = enemies[ei].x;
          break;
        case D_RIGHT:
          enemies[ei].gy = enemies[ei].y;
          enemies[ei].gx = (enemies[ei].x / 20 + 1) * 20;
          break;
        case D_LEFT:
          enemies[ei].gy = enemies[ei].y;
          enemies[ei].gx = (enemies[ei].x / 20 - 1) * 20;
          break;
        }
      }
      if (enemies[ei].hasGoal) {
        switch (enemies[ei].direction) {
        case D_UP:
          enemies[ei].y--;
          break;
        case D_DOWN:
          enemies[ei].y++;
          break;
        case D_RIGHT:
          enemies[ei].x++;
          break;
        case D_LEFT:
          enemies[ei].x--;
          break;
        }
      }
    }
  }
  return 1;
}

inline void drawEnemy(int ei) {
  int offset = 0;
  if (enemies[ei].state == E_WEAK) {
    if (weak < 40)
      offset = weak % 2;
  }
  DDraw_Draw_Surface(enemies[ei].textures[enemies[ei].state - offset]
                                         [pacman.frameOrder[pacman.frame]],
                     X_OFFSET + enemies[ei].x - 2, Y_OFFSET + enemies[ei].y - 2,
                     24, 24, surface, 1);
}

inline void drawFruit() {
  if (fruit.valid)
    DDraw_Draw_Surface(fruit.fruitTextures[fruit.fruiti],
                       X_OFFSET + fruit.x - 2, Y_OFFSET + fruit.y - 2, 24, 24,
                       surface, 1);
}

inline void drawEnemies() {
  int i = 0;
  while (i < numEnemies) {
    drawEnemy(i);
    i++;
  }
}

inline void movePacman() {
  int i = 0;
  if (pacman.state == P_NORMAL) {
    /*
    Draw man
    */
    if (keys[SDLK_DOWN]) {
      int mod = pacman.x % 20;
      pacman.direction = D_DOWN;
      if (mod >= 10)
        pacman.x += (20 - mod);
      else
        pacman.x -= mod;
      if (!level->checkGrid((pacman.y + 20) / 20, pacman.x / 20)) {
        pacman.y += 2;
      }
      gcount--;
    } else if (keys[SDLK_UP]) {
      int mod = pacman.x % 20;
      pacman.direction = D_UP;
      if (mod >= 10)
        pacman.x += (20 - mod);
      else
        pacman.x -= mod;
      if (!level->checkGrid((pacman.y - 2) / 20, pacman.x / 20)) {
        pacman.y -= 2;
      }
      gcount--;
    } else if (keys[SDLK_LEFT]) {
      int mod = pacman.y % 20;
      pacman.direction = D_LEFT;
      if (mod >= 10)
        pacman.y += (20 - mod);
      else
        pacman.y -= (mod);
      if (!level->checkGrid(pacman.y / 20, (pacman.x - 2) / 20)) {
        pacman.x -= 2;
      }
      gcount--;
    } else if (keys[SDLK_RIGHT]) {
      int mod = pacman.y % 20;
      pacman.direction = D_RIGHT;
      if (mod >= 10)
        pacman.y += (20 - mod);
      else
        pacman.y -= (mod);
      if (!level->checkGrid(pacman.y / 20, (pacman.x + 20) / 20)) {
        pacman.x += 2;
      }
      gcount--;
    }

    /*EAT THE SHIT*/
    if (keys[SDLK_SPACE]) {
      switch (pacman.direction) {
      case D_RIGHT:
        if (level->checkGrid(pacman.y / 20, (pacman.x + 20) / 20)) {
          level->tiles[pacman.y / 20][(pacman.x + 20) / 20].valid = 0;
          level->updateAllGrid();
        }
        break;
      case D_LEFT:
        if (level->checkGrid(pacman.y / 20, (pacman.x - 20) / 20)) {
          level->tiles[pacman.y / 20][(pacman.x - 20) / 20].valid = 0;
          level->updateAllGrid();
        }
        break;
      case D_UP:
        if (level->checkGrid((pacman.y - 20) / 20, (pacman.x) / 20)) {
          level->tiles[(pacman.y - 20) / 20][(pacman.x) / 20].valid = 0;
          level->updateAllGrid();
        }
        break;
      case D_DOWN:
        if (level->checkGrid((pacman.y + 20) / 20, (pacman.x) / 20)) {
          level->tiles[(pacman.y + 20) / 20][(pacman.x) / 20].valid = 0;
          level->updateAllGrid();
        }
        break;
      }
    }

    i = 0;
    while (i < numEnemies) {
      // if(pacman.x/20==enemies[i].x/20 &&
      //	pacman.y/20 == enemies[i].y/20)
      if (abs(pacman.x - enemies[i].x) < 20 &&
          abs(pacman.y - enemies[i].y) < 20) {
        if (enemies[i].state == E_WEAK) {
          enemies[i].state = E_DEAD;
          enemies[i].hasLTGoal = 0;
          DSound_Play(eatGhostID);
          // PlaySound("sounds/eatghost.wav",NULL,SND_ASYNC);
          fillFloatingScore(multiple * 150);
          multiple++;
        } else if (enemies[i].state == E_NORMAL) {
          weak = 60;
          pacman.state = P_DYING;
          DSound_Play(dieID);
        }
      }
      i++;
    }

    if (fruit.valid) {
      // if(pacman.x/20==fruit.x/20 &&
      //	pacman.y/20 == fruit.y/20)
      //
      if (abs(pacman.x - fruit.x) < 20 && abs(pacman.y - fruit.y) < 20) {

        fruit.valid = 0;
        DSound_Play(eatFruitID);
        fillFloatingScore(150);
      }
    }

    if (level->tiles[(pacman.y + 10) / 20][(pacman.x + 10) / 20].valid == 3) {
      scores[0] += 10;

      DSound_Play(chompID);

      level->tiles[(pacman.y + 10) / 20][(pacman.x + 10) / 20].valid = 0;
      level->foodCount--;
      if (rand() % 40 == 1) {
        if (!fruit.valid) {
          fruit.hasGoal = 0;
          fruit.hasLTGoal = 0;
          fruit.valid = 1;
          fruit.fruiti = (fruit.fruiti + 1) % 4;
          fruit.x = level->fruitStart.j * 20;
          fruit.y = level->fruitStart.i * 20;
        }
      }

      if (level->foodCount == 0)
        Game_Main_Func = Level_Finished;
    }
    if (level->tiles[(pacman.y + 10) / 20][(pacman.x + 10) / 20].valid == 2) {
      int ei = 0;
      weak = 400;
      scores[0] += 50;
      DSound_Play(weakID);
      while (ei < numEnemies) {
        if (enemies[ei].state == E_NORMAL) {
          enemies[ei].state = E_WEAK;
          enemies[ei].hasLTGoal = 0;
        }
        ei++;
      }

      level->tiles[(pacman.y + 10) / 20][(pacman.x + 10) / 20].valid = 0;
      level->foodCount--;
      if (level->foodCount == 0)
        Game_Main_Func = Level_Finished;
    }
    if (weak == 20)
      // PlaySound("sounds/return.wav",NULL,SND_ASYNC);
      DSound_Play(returnID);
    if (weak > 0)
      weak--;
    if (weak == 0) {
      weak = -1;
      multiple = 1;
      if (enemies[0].state == E_WEAK) {
        enemies[0].state = E_NORMAL;
        enemies[0].hasLTGoal = 0;
      }
      if (enemies[1].state == E_WEAK) {
        enemies[1].state = E_NORMAL;
        enemies[1].hasLTGoal = 0;
      }
      if (enemies[2].state == E_WEAK) {
        enemies[2].state = E_NORMAL;
        enemies[2].hasLTGoal = 0;
      }
      if (enemies[3].state == E_WEAK) {
        enemies[3].state = E_NORMAL;
        enemies[3].hasLTGoal = 0;
      }
    }
    if (gcount <= 0) {
      pacman.frame++;
      pacman.frame %= 6;
      gcount = 8;
    }
    if (pacman.x >= 540)
      pacman.x = 0;
    if (pacman.y >= 440)
      pacman.y = 0;
    if (pacman.x < 0)
      pacman.x = 540;
    if (pacman.y < 0)
      pacman.y = 440;
  } else {
    weak--;
    gcount--;
    if (gcount <= 0) {
      pacman.direction++;
      pacman.direction %= 4;
      gcount = 8;
    }
    if (weak <= 0) {
      pacman.state = P_NORMAL;
      placeEnemies();
      placePacman();
      lives[0]--;
      if (lives[0] == -1) {
        Game_Main_Func = Game_Over;
      }
    }
  }
}

inline void drawPacman() {
  DDraw_Draw_Surface(
      pacman.textures[pacman.direction][pacman.frameOrder[pacman.frame]],
      X_OFFSET + pacman.x - 2, Y_OFFSET + pacman.y - 2, 24, 24, surface, 1);
}

inline void drawFloatingScore() {
  if (score.framesLeft > 0) {
    int i = score.digitCount - 1;
    int offset = 0 + X_OFFSET;
    int dec = 0;
    if (score.framesLeft % 4 == 0)
      dec = 1;
    while (i >= 0) {
      DDraw_Draw_Surface(digits[score.digits[i]], score.x + offset,
                         score.y + Y_OFFSET, 16, 14, surface, 1);
      score.y -= dec;
      offset += 12;
      i--;
    }
    score.framesLeft--;
  }
}

void fillFloatingScore(int s) {
  int ndigits = 0;

  score.score = s;
  score.digitCount = 0;

  score.x = pacman.x;
  score.y = pacman.y;

  while (s > 0) {
    score.digits[score.digitCount] = s % 10;
    s /= 10;
    score.digitCount++;
  }
  score.framesLeft = 30;
  scores[0] += score.score;
}

inline int drawScoreBoard() {
  int i = 0;
  int x = 100;
  int modulus = 10;
  int scoreDigits[15];
  int numDigits = 0;
  int tscore = scores[0];

  while (tscore > 0) {
    scoreDigits[numDigits] = tscore % modulus;
    tscore /= modulus;
    numDigits++;
  }
  i = numDigits - 1;
  while (i >= 0) {
    DDraw_Draw_Surface(bigDigits[scoreDigits[i]], x, 0, 18, 20, surface, 1);
    i--;
    x += 16;
  }
  i = 0;
  x = 10;
  while (i < lives[0]) {
    DDraw_Draw_Surface(pacman.textures[D_LEFT][3], x, 456, 24, 24, surface, 0);
    i++;
    x += 30;
  }
  DrawFPS();
  return 1;
}

int Game_Intro() {
  static int y = -200;
  static int wait = 0;
  static int selected = 0;
  static int menuStartY = 300;
  static int yspace = 40;
  static int ex = 0;
  static int ey = 440; // 983-7980
  static int aframe = 0;
  static int delay = 0;
  static int estate = 0;

  timer.start();
  SDL_FillRect(surface, 0, 0);

  DDraw_Draw_Surface(pacmanTitle, 135, y, 400, 200, surface, 1);

  DDraw_Draw_Surface(startItem, 255, menuStartY - y + 100, 120, yspace, surface,
                     1);
  DDraw_Draw_Surface(hiScoreItem, 255, menuStartY + yspace - y + 100, 120,
                     yspace, surface, 1);
  DDraw_Draw_Surface(exitItem, 255, menuStartY + 2 * yspace - y + 100, 120,
                     yspace, surface, 1);

  // aw_Text_GDI("Unavailable",270,menuStartY+yspace-y+110,_RGB16BIT565(255,255,255),surface);
  // Draw_Line(255,menuStartY+yspace-y+100,275,menuStartY+yspace-y+100,_RGB16BIT565(255,255,255),0,0);

  DDraw_Draw_Surface(enemies[0].textures[estate][aframe], ex, ey, 24, 24,
                     surface, 1);
  DDraw_Draw_Surface(enemies[1].textures[E_NORMAL][aframe], ex + 30, ey + 10,
                     24, 24, surface, 1);

  DDraw_Draw_Surface(enemies[2].textures[estate][aframe], 640 - ex - 24, ey, 24,
                     24, surface, 1);
  DDraw_Draw_Surface(enemies[3].textures[E_NORMAL][aframe], 640 - ex - 54,
                     ey + 10, 24, 24, surface, 1);

  DDraw_Draw_Surface(fruit.fruitTextures[3], ex, y + 150, 24, 24, surface, 1);
  DDraw_Draw_Surface(fruit.fruitTextures[2], ex + 30, y + 160, 24, 24, surface,
                     1);

  DDraw_Draw_Surface(fruit.fruitTextures[0], 640 - ex - 24, y + 150, 24, 24,
                     surface, 1);
  DDraw_Draw_Surface(fruit.fruitTextures[1], 640 - ex - 54, y + 160, 24, 24,
                     surface, 1);

  if (ex == 100)
    estate = E_WEAK;
  if (ex == 200)
    estate = E_DEAD;
  if (ex == 260)
    estate = E_NORMAL;

  if (y == 0)
    DSound_Play(startID);

  if (ex < 260) {
    delay++;
    ex += 2;
    if (delay >= 4) {
      delay = 0;
      aframe++;
    }
    if (ex > 150) {
      ey--;
    }
  }
  if (aframe >= 4)
    aframe = 0;

  if (keys[SDLK_ESCAPE]) {
    y = 100;
    ex = 260;
  }
  if (y < 100) {
    y += 2;
  } else {

    DDraw_Draw_Surface(pacman.textures[D_RIGHT][3], 255,
                       menuStartY + selected * yspace + 8, 24, 24, surface, 1);
    if (keys[SDLK_DOWN]) {
      keys[SDLK_DOWN] = 0;
      selected++;
      if (selected > 2)
        selected = 2;
    }
    if (keys[SDLK_UP]) {
      keys[SDLK_UP] = 0;
      selected--;
      if (selected < 0)
        selected = 0;
    }

    if (keys[SDLK_RETURN]) {
      keys[SDLK_RETURN] = 0;
      if (selected == 0) {
        levelNumber = 0;
        scores[0] = 0;
        lives[0] = 2;
        fetchLevel();
        Game_Main_Func = Level_Intro; // Game_Main;
      } else if (selected == 2) {
        exit(1);
      }
    }
  }

  timer.wait(40);
  return 1;
}

int Game_Paused() {
  static int selected = 0;
  int menuStartY = 100;
  int yspace = 40;
  timer.start();
  SDL_FillRect(surface, 0, 0);
  level->drawLevel(surface);
  drawPacman();
  drawScoreBoard();

  DDraw_Draw_Surface(menuBack, 250, 60, 140, 140, surface, 1);

  DDraw_Draw_Surface(pacman.textures[D_RIGHT][3], 260,
                     menuStartY + selected * yspace + 8, 24, 24, surface, 1);
  DDraw_Draw_Surface(returnItem, 260, menuStartY, 120, yspace, surface, 1);
  DDraw_Draw_Surface(exitItem, 260, menuStartY + yspace, 120, yspace, surface,
                     1);

  if (keys[SDLK_DOWN]) {
    keys[SDLK_DOWN] = 0;
    selected++;
    if (selected > 1)
      selected = 1;
  }
  if (keys[SDLK_UP]) {
    keys[SDLK_UP] = 0;
    selected--;
    if (selected < 0)
      selected = 0;
  }
  if (keys[SDLK_RETURN]) {

    keys[SDLK_RETURN] = 0;
    if (selected == 0)
      Game_Main_Func = Game_Main;
    else if (selected == 1) {
      Game_Main_Func = Game_Intro;
    }
  }

  // while (FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));

  timer.wait(50);
  return 1;
}

int Level_Intro() {
  int i = 0;
  int j = 0;
  static int count = 640;
  static int accel = 10;
  static int halflen = -1;
  timer.start();
  SDL_FillRect(surface, 0, 0);

  level->drawLevel(surface);

  drawEnemies();
  drawPacman();
  drawFloatingScore();
  drawScoreBoard();

  while (i < strlen(levelNames[levelNumber])) {
    if (levelNames[levelNumber][i] != ' ')
      DDraw_Draw_Surface(font[levelNames[levelNumber][i] - 65], count + 28 * i,
                         210, 32, 32, surface, 1);
    i++;
  }
  if (halflen == -1) {
    placePacman();
    placeEnemies();
    halflen = (28 * i) / 2;
  }
  count -= accel;
  if ((count + halflen) < 400 && (count + halflen) > 340) {
    if (accel > 1)
      accel--;
  } else if ((count + halflen) > 240 && (count + halflen) < 300) {
    if (accel < 10)
      accel++;
  }
  if (count + 28 * i < 0) {
    count = 640;
    halflen = -1;
    Game_Main_Func = Game_Main;
  }
  // while (FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));

  timer.wait(12);
  return 1;
}

int Level_Finished() {
  int i = 0;
  int j = 0;
  static int count = 640;
  static int accel = 10;
  static int halflen = -1;
  static char message[50];
  timer.start();
  SDL_FillRect(surface, 0, 0);
  level->drawLevel(surface);

  // drawEnemies();
  drawPacman();
  drawFloatingScore();
  drawScoreBoard();

  if (halflen == -1) {
    sprintf(message, "%s COMPLETE", levelNames[levelNumber]);
  }

  while (i < strlen(message)) {
    if (message[i] != ' ')
      DDraw_Draw_Surface(font[message[i] - 65], count + 28 * i, 210, 32, 32,
                         surface, 1);
    i++;
  }
  if (halflen == -1) {
    halflen = (28 * i) / 2;
  }
  count -= accel;
  if ((count + halflen) < 400 && (count + halflen) > 340) {
    if (accel > 1)
      accel--;
  } else if ((count + halflen) > 240 && (count + halflen) < 300) {
    if (accel < 10)
      accel++;
  }
  if (count + 28 * i < 0) {
    count = 640;
    halflen = -1;

    levelNumber++;
    fetchLevel();
    // placeEnemies();

    Game_Main_Func = Level_Intro;
  }
  // while (FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));

  timer.wait(12);
  return 1;
}

int Game_Over() {
  int i = 0;
  int j = 0;
  static int count = 640;
  static int accel = 10;
  static int halflen = -1;
  static char message[50];
  timer.start();
  SDL_FillRect(surface, 0, 0);
  level->drawLevel(surface);

  drawEnemies();
  // drawPacman();
  drawFloatingScore();
  drawScoreBoard();

  if (halflen == -1) {
    sprintf(message, "GAME OVER");
  }

  while (i < strlen(message)) {
    if (message[i] != ' ')
      DDraw_Draw_Surface(font[message[i] - 65], count + 28 * i, 210, 32, 32,
                         surface, 1);
    i++;
  }
  if (halflen == -1) {
    halflen = (28 * i) / 2;
  }
  count -= accel;
  if ((count + halflen) < 400 && (count + halflen) > 340) {
    if (accel > 1)
      accel--;
  } else if ((count + halflen) > 240 && (count + halflen) < 300) {
    if (accel < 10)
      accel++;
  }
  if (count + 28 * i < 0) {
    count = 640;
    halflen = -1;
    levelNumber = 0;
    scores[0] = 0;
    lives[0] = 2;
    fetchLevel();
    Game_Main_Func = Game_Intro;
  }
  // while (FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));

  timer.wait(12);
  return 1;
}

inline void DrawFPS() {
  static int fcount = 0;
  static DWORD stime = 0;
  static char timeStr[15];
  if (stime == 0)
    stime = GetTickCount();
  fcount++;
  /*if((stime-GetTickCount())>1000)
  {
          
          sprintf(timeStr,"fps:%3d",1000*fcount/(GetTickCount()-stime));
          stime = GetTickCount();
          fcount=0;
  }*/
  if (fcount == 10) {
    sprintf(timeStr, "fps:%3d", 10000 / (GetTickCount() - stime));
    stime = GetTickCount();
    fcount = 0;
  }
  // Draw_Text_GDI(timeStr,200,5,_RGB16BIT565(100,50,100),surface);
}

int Game_Main() {
  int i = 0;
  int j = 0;
  timer.start();

  SDL_FillRect(surface, 0, 0);
  level->drawLevel(surface);

  movePacman();
  moveEnemy(0);
  moveEnemy(1);
  moveEnemy(2);
  moveEnemy(3);

  moveFruit();

  drawEnemies();
  drawFruit();
  drawPacman();
  drawFloatingScore();
  drawScoreBoard();

  timer.wait(12);
  return 1;
}

int Level_Init() {
  level = new Level();
  return 1;
}

int Enemy_Init(int ei) {
  enemies[ei].hasGoal = 0;
  enemies[ei].state = E_NORMAL;
  enemies[ei].x = 0;
  enemies[ei].y = 0;
  enemies[ei].gx = 0;
  enemies[ei].gy = 0;
  enemies[ei].LTgx = 0;
  enemies[ei].LTgx = 0;
  enemies[ei].hasLTGoal = 0;
  return 0;
}

int Game_Init() {
  SDL_Surface* temp;
  debug = fopen("endebug.txt", "w");

  /*
    Set the level Count
  */
  {
    FILE* ld = fopen("levels/leveldat.txt", "r");
    FILE* ldebug = fopen("levels/leveldebug.txt", "w");

    int li = 0;
    if (ld == NULL) {
      std::cerr << "Cant find level data\n";
      return 0;
    }
    fscanf(ld, "%d\n", &levelCount);
    if (levelCount <= 0) {
      std::cerr << "Invalid level data\n";
      return 0;
    }
    levelNames = (char**)malloc(sizeof(char*) * levelCount);
    levelPaths = (char**)malloc(sizeof(char*) * levelCount);
    fprintf(ldebug, "LevelCount is %d\n", levelCount);
    while (li < levelCount) {
      int j = 0;
      char temp[100];
      levelNames[li] = (char*)malloc(sizeof(char) * 35);
      levelPaths[li] = (char*)malloc(sizeof(char) * 100);

      fgets(levelNames[li], 35, ld);
      fgets(temp, 100, ld);

      if (levelNames[li][strlen(levelNames[li]) - 1] == '\n') {
        levelNames[li][strlen(levelNames[li]) - 1] = '\0';
      }

      if (temp[strlen(temp) - 1] == '\n') {
        temp[strlen(temp) - 1] = '\0';
      }
      // fscanf(ld,"[A-Z, ,\"]%s %s",levelNames[li],temp);

      /*
        strcpy(levelNames[li],levelNames[li]+1);
        levelNames[li][strlen(levelNames[li])-1]='\0';

        strcpy(temp,temp+1);
        temp[strlen(temp)-1]='\0';
      */
      sprintf(levelPaths[li], "levels/");
      strcat(levelPaths[li], temp);
      fprintf(ldebug, "Name %s, Path %s\n", levelNames[li], levelPaths[li]);
      li++;
    }
    fclose(ld);
    fclose(ldebug);
  }
#ifdef ENABLE_SOUND
  DSound_Init();
#endif
  int rendererFlags = SDL_RENDERER_SOFTWARE, windowFlags = 0;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, windowFlags);

  if (!window) {
    printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT,
           SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  renderer = SDL_CreateRenderer(window, -1, rendererFlags);

  if (!renderer) {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    exit(1);
  }

  Level_Init();

  test = SDL_LoadBMP("textures/food.bmp");
  level->fillFoodTextures(test);
  SDL_FreeSurface(test);

  test = SDL_LoadBMP("textures/walls.bmp");
  level->fillWallTextures(test);
  SDL_FreeSurface(test);

  fetchLevel();

  test = SDL_LoadBMP("textures/pac.bmp");
  SDL_SetColorKey(test, 1, 0);

  pacman.direction = 0;
  pacman.frame = 0;
  pacman.state = P_NORMAL;
  pacman.frameOrder[0] = 0;
  pacman.frameOrder[1] = 1;
  pacman.frameOrder[2] = 2;
  pacman.frameOrder[3] = 3;
  pacman.frameOrder[4] = 2;
  pacman.frameOrder[5] = 1;

  scores[0] = 0;

  pacman.textures[D_UP][0] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_UP][1] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_UP][2] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_UP][3] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  pacman.textures[D_RIGHT][0] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_RIGHT][1] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_RIGHT][2] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_RIGHT][3] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  pacman.textures[D_DOWN][0] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_DOWN][1] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_DOWN][2] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_DOWN][3] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  pacman.textures[D_LEFT][0] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_LEFT][1] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_LEFT][2] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  pacman.textures[D_LEFT][3] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 0, 24, 24, pacman.textures[D_UP][0], 0);
  NDDraw_Draw_Surface(test, 24, 0, 24, 24, pacman.textures[D_UP][1], 0);
  NDDraw_Draw_Surface(test, 48, 0, 24, 24, pacman.textures[D_UP][2], 0);
  NDDraw_Draw_Surface(test, 72, 0, 24, 24, pacman.textures[D_UP][3], 0);

  NDDraw_Draw_Surface(test, 0, 24, 24, 24, pacman.textures[D_RIGHT][0], 0);
  NDDraw_Draw_Surface(test, 24, 24, 24, 24, pacman.textures[D_RIGHT][1], 0);
  NDDraw_Draw_Surface(test, 48, 24, 24, 24, pacman.textures[D_RIGHT][2], 0);
  NDDraw_Draw_Surface(test, 72, 24, 24, 24, pacman.textures[D_RIGHT][3], 0);

  NDDraw_Draw_Surface(test, 0, 48, 24, 24, pacman.textures[D_DOWN][0], 0);
  NDDraw_Draw_Surface(test, 24, 48, 24, 24, pacman.textures[D_DOWN][1], 0);
  NDDraw_Draw_Surface(test, 48, 48, 24, 24, pacman.textures[D_DOWN][2], 0);
  NDDraw_Draw_Surface(test, 72, 48, 24, 24, pacman.textures[D_DOWN][3], 0);

  NDDraw_Draw_Surface(test, 0, 72, 24, 24, pacman.textures[D_LEFT][0], 0);
  NDDraw_Draw_Surface(test, 24, 72, 24, 24, pacman.textures[D_LEFT][1], 0);
  NDDraw_Draw_Surface(test, 48, 72, 24, 24, pacman.textures[D_LEFT][2], 0);
  NDDraw_Draw_Surface(test, 72, 72, 24, 24, pacman.textures[D_LEFT][3], 0);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      SDL_SetColorKey(pacman.textures[i][j], 1, 0);
    }
  }

  SDL_FreeSurface(test);
  test = SDL_LoadBMP("textures/enemy.bmp");

  Enemy_Init(0);
  Enemy_Init(1);

  /*
          Enemy 0's normal texture is loaded
  */
  enemies[0].textures[E_NORMAL][0] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_NORMAL][1] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_NORMAL][2] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_NORMAL][3] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 0, 24, 24, enemies[0].textures[E_NORMAL][0], 0);
  NDDraw_Draw_Surface(test, 24, 0, 24, 24, enemies[0].textures[E_NORMAL][1], 0);
  NDDraw_Draw_Surface(test, 48, 0, 24, 24, enemies[0].textures[E_NORMAL][2], 0);
  NDDraw_Draw_Surface(test, 72, 0, 24, 24, enemies[0].textures[E_NORMAL][3], 0);

  /*
          Enemy 0's weak texture is loaded, which is the same as the others weak
     texture
  */
  enemies[0].textures[E_WEAK][0] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_WEAK][1] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_WEAK][2] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_WEAK][3] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 96, 24, 24, enemies[0].textures[E_WEAK][0], 0);
  NDDraw_Draw_Surface(test, 24, 96, 24, 24, enemies[0].textures[E_WEAK][1], 0);
  NDDraw_Draw_Surface(test, 48, 96, 24, 24, enemies[0].textures[E_WEAK][2], 0);
  NDDraw_Draw_Surface(test, 72, 96, 24, 24, enemies[0].textures[E_WEAK][3], 0);

  /*
          Enemy 0's dead texture is loaded, which is the same as the others weak
     texture
  */
  enemies[0].textures[E_DEAD][0] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_DEAD][1] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_DEAD][2] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[0].textures[E_DEAD][3] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 120, 24, 24, enemies[0].textures[E_DEAD][0], 0);
  NDDraw_Draw_Surface(test, 24, 120, 24, 24, enemies[0].textures[E_DEAD][1], 0);
  NDDraw_Draw_Surface(test, 48, 120, 24, 24, enemies[0].textures[E_DEAD][2], 0);
  NDDraw_Draw_Surface(test, 72, 120, 24, 24, enemies[0].textures[E_DEAD][3], 0);

  enemies[1].textures[E_NORMAL][0] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[1].textures[E_NORMAL][1] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[1].textures[E_NORMAL][2] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[1].textures[E_NORMAL][3] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 24, 24, 24, enemies[1].textures[E_NORMAL][0], 0);
  NDDraw_Draw_Surface(test, 24, 24, 24, 24, enemies[1].textures[E_NORMAL][1],
                      0);
  NDDraw_Draw_Surface(test, 48, 24, 24, 24, enemies[1].textures[E_NORMAL][2],
                      0);
  NDDraw_Draw_Surface(test, 72, 24, 24, 24, enemies[1].textures[E_NORMAL][3],
                      0);

  enemies[2].textures[E_NORMAL][0] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[2].textures[E_NORMAL][1] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[2].textures[E_NORMAL][2] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[2].textures[E_NORMAL][3] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 48, 24, 24, enemies[2].textures[E_NORMAL][0], 0);
  NDDraw_Draw_Surface(test, 24, 48, 24, 24, enemies[2].textures[E_NORMAL][1],
                      0);
  NDDraw_Draw_Surface(test, 48, 48, 24, 24, enemies[2].textures[E_NORMAL][2],
                      0);
  NDDraw_Draw_Surface(test, 72, 48, 24, 24, enemies[2].textures[E_NORMAL][3],
                      0);

  enemies[3].textures[E_NORMAL][0] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[3].textures[E_NORMAL][1] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[3].textures[E_NORMAL][2] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  enemies[3].textures[E_NORMAL][3] =
      SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 72, 24, 24, enemies[3].textures[E_NORMAL][0], 0);
  NDDraw_Draw_Surface(test, 24, 72, 24, 24, enemies[3].textures[E_NORMAL][1],
                      0);
  NDDraw_Draw_Surface(test, 48, 72, 24, 24, enemies[3].textures[E_NORMAL][2],
                      0);
  NDDraw_Draw_Surface(test, 72, 72, 24, 24, enemies[3].textures[E_NORMAL][3],
                      0);

  enemies[1].textures[1][0] = enemies[0].textures[1][0];
  enemies[1].textures[1][1] = enemies[0].textures[1][1];
  enemies[1].textures[1][2] = enemies[0].textures[1][2];
  enemies[1].textures[1][3] = enemies[0].textures[1][3];
  enemies[1].textures[2][0] = enemies[0].textures[2][0];
  enemies[1].textures[2][1] = enemies[0].textures[2][1];
  enemies[1].textures[2][2] = enemies[0].textures[2][2];
  enemies[1].textures[2][3] = enemies[0].textures[2][3];

  enemies[2].textures[1][0] = enemies[0].textures[1][0];
  enemies[2].textures[1][1] = enemies[0].textures[1][1];
  enemies[2].textures[1][2] = enemies[0].textures[1][2];
  enemies[2].textures[1][3] = enemies[0].textures[1][3];
  enemies[2].textures[2][0] = enemies[0].textures[2][0];
  enemies[2].textures[2][1] = enemies[0].textures[2][1];
  enemies[2].textures[2][2] = enemies[0].textures[2][2];
  enemies[2].textures[2][3] = enemies[0].textures[2][3];

  enemies[3].textures[1][0] = enemies[0].textures[1][0];
  enemies[3].textures[1][1] = enemies[0].textures[1][1];
  enemies[3].textures[1][2] = enemies[0].textures[1][2];
  enemies[3].textures[1][3] = enemies[0].textures[1][3];
  enemies[3].textures[2][0] = enemies[0].textures[2][0];
  enemies[3].textures[2][1] = enemies[0].textures[2][1];
  enemies[3].textures[2][2] = enemies[0].textures[2][2];
  enemies[3].textures[2][3] = enemies[0].textures[2][3];

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 3; ++j) {
      for (int k = 0; k < 4; ++k) {
        SDL_SetColorKey(enemies[i].textures[j][k], 1, 0);
      }
    }
  }

  SDL_FreeSurface(test);
  test = SDL_LoadBMP("textures/smallnumbers.bmp");

  digits[0] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[1] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[2] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[3] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[4] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[5] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[6] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[7] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[8] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);
  digits[9] = SDL_CreateRGBSurface(0, 16, 14, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 0, 16, 14, digits[0], 0);
  NDDraw_Draw_Surface(test, 16, 0, 16, 14, digits[1], 0);
  NDDraw_Draw_Surface(test, 32, 0, 16, 14, digits[2], 0);
  NDDraw_Draw_Surface(test, 48, 0, 16, 14, digits[3], 0);
  NDDraw_Draw_Surface(test, 64, 0, 16, 14, digits[4], 0);
  NDDraw_Draw_Surface(test, 80, 0, 16, 14, digits[5], 0);
  NDDraw_Draw_Surface(test, 96, 0, 16, 14, digits[6], 0);
  NDDraw_Draw_Surface(test, 112, 0, 16, 14, digits[7], 0);
  NDDraw_Draw_Surface(test, 128, 0, 16, 14, digits[8], 0);
  NDDraw_Draw_Surface(test, 144, 0, 16, 14, digits[9], 0);
  for (int i = 0; i < 10; ++i) {
    SDL_SetColorKey(digits[i], 1, 0);
  }

  SDL_FreeSurface(test);
  test = SDL_LoadBMP("textures/bignumbers.bmp");

  bigDigits[0] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[1] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[2] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[3] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[4] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[5] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[6] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[7] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[8] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);
  bigDigits[9] = SDL_CreateRGBSurface(0, 18, 20, 32, 0, 0, 0, 0);

  NDDraw_Draw_Surface(test, 0, 0, 18, 20, bigDigits[0], 0);
  NDDraw_Draw_Surface(test, 18, 0, 18, 20, bigDigits[1], 0);
  NDDraw_Draw_Surface(test, 36, 0, 18, 20, bigDigits[2], 0);
  NDDraw_Draw_Surface(test, 54, 0, 18, 20, bigDigits[3], 0);
  NDDraw_Draw_Surface(test, 72, 0, 18, 20, bigDigits[4], 0);
  NDDraw_Draw_Surface(test, 90, 0, 18, 20, bigDigits[5], 0);
  NDDraw_Draw_Surface(test, 108, 0, 18, 20, bigDigits[6], 0);
  NDDraw_Draw_Surface(test, 126, 0, 18, 20, bigDigits[7], 0);
  NDDraw_Draw_Surface(test, 144, 0, 18, 20, bigDigits[8], 0);
  NDDraw_Draw_Surface(test, 162, 0, 18, 20, bigDigits[9], 0);

  for (int i = 0; i < 10; ++i) {
    SDL_SetColorKey(bigDigits[i], 1, 0);
  }

  SDL_FreeSurface(test);
  // Load_Bitmap_File(&bitFile,"pacmanTitle.bmp");
  /*
  Load_Bitmap_File(&bitFile,"test.bmp");
  test= SDL_CreateRGBSurface(0, 390,200,32, 0, 0, 0, 0);
  pacmanTitle = SDL_CreateRGBSurface(0, 390,200,32, 0, 0, 0, 0);
  Scan_Image_Bitmap16(&bitFile,test,0,0,390,200);
  Unload_Bitmap_File(&bitFile);
  
  DDraw_Draw_Surface(test,0,0,390,200,pacmanTitle,0);
  */

  test = SDL_LoadBMP("textures/fruit.bmp");

  fruit.fruitTextures[GRAPE] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  fruit.fruitTextures[ORANGE] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  fruit.fruitTextures[BANANA] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);
  fruit.fruitTextures[APPLE] = SDL_CreateRGBSurface(0, 24, 24, 32, 0, 0, 0, 0);

  fruit.x = 20;
  fruit.y = 20;
  fruit.hasGoal = 0;

  NDDraw_Draw_Surface(test, 0, 0, 24, 24, fruit.fruitTextures[0], 0);
  NDDraw_Draw_Surface(test, 24, 0, 24, 24, fruit.fruitTextures[1], 0);
  NDDraw_Draw_Surface(test, 48, 0, 24, 24, fruit.fruitTextures[2], 0);
  NDDraw_Draw_Surface(test, 72, 0, 24, 24, fruit.fruitTextures[3], 0);
  for (int i = 0; i < 4; ++i) {
    SDL_SetColorKey(fruit.fruitTextures[i], 1, 0);
  }
  SDL_FreeSurface(test);

  menuBack = SDL_LoadBMP("textures/menuback.bmp");
  pacmanTitle = SDL_LoadBMP("textures/pacmanTitle.bmp");
  startItem = SDL_LoadBMP("textures/start.bmp");
  exitItem = SDL_LoadBMP("textures/exit.bmp");
  returnItem = SDL_LoadBMP("textures/return.bmp");
  hiScoreItem = SDL_LoadBMP("textures/hiscores.bmp");

  memset(font, 0, sizeof(SDL_Surface*) * 26);

  {
    int x = 32;
    int i = 0;

    test = SDL_LoadBMP("textures/font.bmp");

    while (x < 512) {
      font[i] = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
      NDDraw_Draw_Surface(test, x, 0, 32, 32, font[i], 0);
      SDL_SetColorKey(font[i], 1, 0);
      x += 32;
      i++;
    }
    x = 0;

    while (i < 26) {
      font[i] = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
      NDDraw_Draw_Surface(test, x, 32, 32, 32, font[i], 0);
      SDL_SetColorKey(font[i], 1, 0);
      x += 32;
      i++;
    }

    SDL_FreeSurface(test);
    test = NULL;
  }
  // level->initTile();
  return 0;
}

int Game_Shutdown(void* parms = NULL, int num_parms = 0) {
  int i = 0;
  int j;
  if (debug)
    fclose(debug);
  if (level)
    free(level);
  return 0;
}

int HandleInput();

int main(int ac, char* av[]) {
  // initialize game here
  Game_Init();
  strcpy(levelName, "STAGE ONE");

  // enter main event loop
  Game_Main_Func = Game_Intro;
  while (true) {
    surface = SDL_GetWindowSurface(window);

    HandleInput();

    Game_Main_Func();

    SDL_UpdateWindowSurface(window);
  }

  Game_Shutdown();

  return 0;
}

int HandleKeyPress(SDL_Keysym keysym) {
  int wparam = keysym.sym;
  if (wparam == SDLK_ESCAPE) {
    if (Game_Main_Func == Game_Main) {
      Game_Main_Func = Game_Paused;
    } else {
      if (Game_Main_Func == Game_Paused)
        Game_Main_Func = Game_Main;
    }
  }
  return 0;
}

int HandleInput() {
  int wparam = 0;
  SDL_Event event;
  while (SDL_PollEvent(&event))
    switch (event.type) {
    case SDL_MOUSEMOTION:
      break;
    case SDL_MOUSEBUTTONDOWN:
      break;
    case SDL_QUIT:
      exit(0);
      break;

    case SDL_KEYDOWN: {
      SDL_Keysym keysym = event.key.keysym;
      if (!keys[keysym.sym]) {
        HandleKeyPress(keysym);
      }
      keys[keysym.sym] = true;
    } break;
    case SDL_KEYUP: {
      SDL_Keysym keysym = event.key.keysym;
      keys[keysym.sym] = false;
    } break;
    }

  return 0;
}
