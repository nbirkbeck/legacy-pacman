#include "level.h"
#include <stdio.h>

#include "compat.h"

Level::Level() {
  gwidth = G_WIDTH;
  gheight = G_HEIGHT;
  foodCount = 0;
  numsafe = 0;
  safe = (loc*)malloc(sizeof(loc) * 10);
  memset(tiles, 0, gwidth * gheight * sizeof(tile));
  memset(foodLoc, 0, sizeof(loc) * 160);

  file = fopen("pathdebug.txt", "wa");

  gridTiles[BL_IF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[UL_IF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[UR_IF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[BR_IF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[V_RF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[H_UF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);

  gridTiles[BL_OF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[UL_OF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[UR_OF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[BR_OF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[V_LF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[H_BF] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);

  gridTiles[FILLER] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);

  gridTiles[DOOR_UL] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[DOOR_UR] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[DOOR_BL] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
  gridTiles[DOOR_BR] = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);

  gridTiles[BIGFOOD] = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
  gridTiles[SMALLFOOD] = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
}

Level::~Level() {
  int i = 0;
  if (safe)
    free(safe);
  while (i < NUM_TILES) {
    if (gridTiles[i]) {
      SDL_FreeSurface(gridTiles[i]);
      gridTiles[i] = NULL;
    }
    i++;
  }
  fclose(file);
}

void Level::drawLevel(SDL_Surface* screen) {
  int i, j;
  int x, y;
  y = Y_OFFSET;
  i = 0;
  while (i < gheight) {
    j = 0;
    x = X_OFFSET;
    while (j < gwidth) {
      if (tiles[i][j].valid == 1 || tiles[i][j].valid == 4) {
        DDraw_Draw_Surface(gridTiles[tiles[i][j].ul], x, y, 10, 10, screen, 1);
        DDraw_Draw_Surface(gridTiles[tiles[i][j].ur], x + 10, y, 10, 10, screen,
                           1);
        DDraw_Draw_Surface(gridTiles[tiles[i][j].bl], x, y + 10, 10, 10, screen,
                           1);
        DDraw_Draw_Surface(gridTiles[tiles[i][j].br], x + 10, y + 10, 10, 10,
                           screen, 1);
      } else if (tiles[i][j].valid == 2 || tiles[i][j].valid == 3)
        DDraw_Draw_Surface(gridTiles[tiles[i][j].ul], X_OFFSET + j * 20,
                           Y_OFFSET + i * 20, 20, 20, screen, 1);
      j++;
      x += 20;
    }
    y += 20;
    i++;
  }
}

void Level::fillFoodTextures(SDL_Surface* test) {
  // BITMAP_FILE bitFile;
  // SDL_Surface* test;

  // Load_Bitmap_File(&bitFile,"food.bmp");
  // test = SDL_CreateRGBSurface(40,20,DDSCAPS_VIDEOMEMORY,0);
  // Scan_Image_Bitmap16(&bitFile,test,0,0,40,20);
  // Unload_Bitmap_File(&bitFile);

  NDDraw_Draw_Surface(test, 0, 0, 20, 20, gridTiles[BIGFOOD], 0);
  NDDraw_Draw_Surface(test, 20, 0, 20, 20, gridTiles[SMALLFOOD], 0);
}
void Level::fillWallTextures(SDL_Surface* test) {

  // BITMAP_FILE bitFile;
  // SDL_Surface* test;

  // Load_Bitmap_File(&bitFile,filename);
  // test = SDL_CreateRGBSurface(40,50,DDSCAPS_VIDEOMEMORY,0);
  // Scan_Image_Bitmap16(&bitFile,test,0,0,40,50);
  // Unload_Bitmap_File(&bitFile);

  NDDraw_Draw_Surface(test, 0, 0, 10, 10, gridTiles[BL_IF], 0);
  NDDraw_Draw_Surface(test, 10, 0, 10, 10, gridTiles[UL_IF], 0);
  NDDraw_Draw_Surface(test, 20, 0, 10, 10, gridTiles[UR_IF], 0);
  NDDraw_Draw_Surface(test, 30, 0, 10, 10, gridTiles[BR_IF], 0);
  NDDraw_Draw_Surface(test, 0, 10, 10, 10, gridTiles[V_RF], 0);
  NDDraw_Draw_Surface(test, 10, 10, 10, 10, gridTiles[H_UF], 0);

  NDDraw_Draw_Surface(test, 0, 20, 10, 10, gridTiles[BL_OF], 0);
  NDDraw_Draw_Surface(test, 10, 20, 10, 10, gridTiles[UL_OF], 0);
  NDDraw_Draw_Surface(test, 20, 20, 10, 10, gridTiles[UR_OF], 0);
  NDDraw_Draw_Surface(test, 30, 20, 10, 10, gridTiles[BR_OF], 0);
  NDDraw_Draw_Surface(test, 0, 30, 10, 10, gridTiles[V_LF], 0);
  NDDraw_Draw_Surface(test, 10, 30, 10, 10, gridTiles[H_BF], 0);
  NDDraw_Draw_Surface(test, 20, 30, 10, 10, gridTiles[FILLER], 0);
  NDDraw_Draw_Surface(test, 0, 40, 10, 10, gridTiles[DOOR_UL], 0);
  NDDraw_Draw_Surface(test, 10, 40, 10, 10, gridTiles[DOOR_UR], 0);
  NDDraw_Draw_Surface(test, 20, 40, 10, 10, gridTiles[DOOR_BL], 0);
  NDDraw_Draw_Surface(test, 30, 40, 10, 10, gridTiles[DOOR_BR], 0);

  // test->Release();
}

int Level::checkGrid(int i, int j) {
  if (i < 0 || j < 0)
    return 0;
  if (i >= G_HEIGHT || j >= G_WIDTH)
    return 0;
  if (tiles[i][j].valid == 1)
    return 1;
  if (tiles[i][j].valid == 4)
    return 1;
  else
    return 0;
}

int Level::checkGridEnemy(int i, int j) {
  if (i < 0 || j < 0)
    return 0;
  if (i >= G_HEIGHT || j >= G_WIDTH)
    return 0;
  if (tiles[i][j].valid == 1)
    return 1;
  else
    return 0;
}

void Level::updateGrid(int i, int j) {

  if (i < 0 || i >= G_HEIGHT)
    return;
  if (j < 0 || j >= G_WIDTH)
    return;

  if (tiles[i][j].valid != 1)
    return;

  if (checkGrid(i - 1, j - 1) && checkGrid(i, j - 1) && checkGrid(i - 1, j))
    tiles[i][j].ul = FILLER;

  else if (checkGrid(i, j - 1) && checkGrid(i - 1, j))
    tiles[i][j].ul = BR_OF;
  else if (checkGrid(i, j - 1) && !checkGrid(i - 1, j))
    tiles[i][j].ul = H_BF;
  else if (!checkGrid(i, j - 1) && checkGrid(i - 1, j))
    tiles[i][j].ul = V_RF;
  else if (!checkGrid(i, j - 1) && !checkGrid(i - 1, j))
    tiles[i][j].ul = UL_IF;

  if (checkGrid(i - 1, j + 1) && checkGrid(i - 1, j) && checkGrid(i, j + 1))
    tiles[i][j].ur = FILLER;
  else if (checkGrid(i, j + 1) && !checkGrid(i - 1, j))
    tiles[i][j].ur = H_BF;
  else if (checkGrid(i, j + 1) && checkGrid(i - 1, j))
    tiles[i][j].ur = BL_OF;
  else if (!checkGrid(i, j + 1) && checkGrid(i - 1, j))
    tiles[i][j].ur = V_LF;
  else if (!checkGrid(i, j + 1) && !checkGrid(i - 1, j))
    tiles[i][j].ur = UR_IF;

  if (checkGrid(i + 1, j - 1) && checkGrid(i + 1, j) && checkGrid(i, j - 1))
    tiles[i][j].bl = FILLER;
  else if (checkGrid(i, j - 1) && !checkGrid(i + 1, j))
    tiles[i][j].bl = H_UF;
  else if (checkGrid(i, j - 1) && checkGrid(i + 1, j))
    tiles[i][j].bl = UR_OF;
  else if (!checkGrid(i, j - 1) && checkGrid(i + 1, j))
    tiles[i][j].bl = V_RF;
  else if (!checkGrid(i, j - 1) && !checkGrid(i + 1, j))
    tiles[i][j].bl = BL_IF;

  if (checkGrid(i + 1, j + 1) && checkGrid(i + 1, j) && checkGrid(i, j + 1))
    tiles[i][j].br = FILLER;
  else if (checkGrid(i, j + 1) && !checkGrid(i + 1, j))
    tiles[i][j].br = H_UF;
  else if (checkGrid(i, j + 1) && checkGrid(i + 1, j))
    tiles[i][j].br = UL_OF;
  else if (!checkGrid(i, j + 1) && checkGrid(i + 1, j))
    tiles[i][j].br = V_LF;
  else if (!checkGrid(i, j + 1) && !checkGrid(i + 1, j))
    tiles[i][j].br = BR_IF;
}

void Level::initTiles() {
  int i = 0;
  int j;
  for (; i < G_HEIGHT; i++) {
    for (j = 0; j < G_WIDTH; j++) {
      if (rand() % 3 > 0)
      // if(i==1)
      {
        tiles[i][j].valid = 1;
      } else
        tiles[i][j].valid = 0;
    }
  }
  updateAllGrid();
}

void Level::updateAllGrid() {
  int i = 0;
  int j = 0;
  for (; i < G_HEIGHT; i++) {
    for (j = 0; j < G_WIDTH; j++) {
      updateGrid(i, j);
    }
  }
}

int Level::loadLevel(char* filename) {
  FILE* ifile = fopen(filename, "r");
  FILE* dbg = fopen("debug.txt", "w");
  int i = 0;
  int j = 0;
  int fi = 0;

  foodLocI = 0;
  this->foodLocCount = 0;
  foodCount = 0;
  numsafe = 0;

  if (!file)
    exit(1);
  while (i < gheight) {
    j = 0;

    while (1) {
      char c = getc(ifile);
      if (c == 'w') {
        fprintf(dbg, "Got a wall\n");
        tiles[i][j].valid = 1;

      } else if (c == '*') {
        tiles[i][j].valid = 2;
        tiles[i][j].ul = BIGFOOD;
        foodCount++;
        fi++;

      } else if (c == '.') {
        fprintf(dbg, "Got a SmallFood\n");
        tiles[i][j].valid = 3;
        tiles[i][j].ul = SMALLFOOD;
        foodCount++;
        fi++;

      } else if (c == '\n') {
        break;
      } else if (c == '=') {
        tiles[i][j].valid = 4;
        tiles[i][j].ul = DOOR_UL;
        tiles[i][j].ur = DOOR_UR;
        tiles[i][j].bl = DOOR_BL;
        tiles[i][j].br = DOOR_BR;

      } else if (c == '#') {
        fprintf(dbg, "Got a safe spot\n");
        tiles[i][j].valid = 5;
        safe[numsafe].i = i;
        safe[numsafe++].j = j;
      } else if (c == 'f') {
        tiles[i][j].valid = 3;
        tiles[i][j].ul = SMALLFOOD;
        foodCount++;
        fi++;
        if (j == 0) {
          fruitStart.i = i;
          fruitStart.j = j;
        }
        if (j == (G_WIDTH - 1)) {
          fruitEnd.i = i;
          fruitEnd.j = j;
        }
      } else {
        tiles[i][j].valid = 0;
      }
      if (fi == 4) {
        fi = 0;
        /*
        this->foodLoc[this->foodLocCount].i=i;
        this->foodLoc[this->foodLocCount].j=j;
        */
        foodLocIs[foodLocCount] = i;
        foodLocJs[foodLocCount] = j;
        fprintf(dbg, "foodLoc[%d]=[%d,%d]\n", foodLocCount,
                foodLocIs[foodLocCount], foodLocJs[foodLocCount]);
        this->foodLocCount++;
      }
      j++;
    }
    i++;
  }
  {
    int ii = 0;
    for (; ii < foodLocCount; ii++) {
      fprintf(dbg, "ii= %d,%d\n", foodLocIs[ii], foodLocJs[ii]);
    }
  }
  updateAllGrid();

  fclose(ifile);
  fclose(dbg);
  return 1;
}

int Level::findPath(int si, int sj, int di, int dj) {
  int visited[G_HEIGHT][G_WIDTH]; // = (int **)malloc(sizeof(int)**G_WIDTH);
  int inpath = 0;
  int success = 0;
  loc stack[100];
  loc target = {di, dj};
  int instack = 0;

  int cutoff = 99;
  DWORD stime = GetTickCount();
  if (DEBUG)
    fprintf(file, "Starting to findpath From %d,%d to %d,%d\n", si, sj, di, dj);

  DFSstyle = (DFSstyle + 1) % 4;
  while (cutoff < 100 && !success) {
    inpath = 0;
    instack = 0;
    stack[instack].i = si;
    stack[instack++].j = sj;
    if (DEBUG)
      fprintf(file, "Attempt %d\n", cutoff);

    memset(visited, 0, sizeof(int) * G_HEIGHT * G_WIDTH);

    if (limitedDFS(&target, stack, &instack, path, &inpath, visited, cutoff)) {
      success = 1;
      break;
    }
    cutoff++;
  }
  if (DEBUG) {
    if (success) {
      int i = 0;
      fprintf(file, "Found path of depth %d, inpath=%d\n", cutoff, inpath);
      while (i < inpath) {
        if (DEBUG)
          fprintf(file, "%d:take %d\n", i, path[i]);
        i++;
      }
    } else
      fprintf(file, "Couldnt find a path\n");
  }
  {
    int i, j;
    fprintf(file, "Grid:\n");
    for (i = 0; i < G_HEIGHT; i++) {
      for (j = 0; j < G_WIDTH; j++) {
        fprintf(file, "%d ", visited[i][j]);
      }
      fprintf(file, "\n");
    }
  }
  fprintf(file, "Time:\n");
  fprintf(file, "Time: %d\n", GetTickCount() - stime);
  if (inpath == 0 || cutoff == 1)
    return -1;
  return path[0];
}

int Level::findPath2(int si, int sj, int di, int dj) {
  int visited[G_HEIGHT][G_WIDTH]; // = (int **)malloc(sizeof(int)**G_WIDTH);
  int success = 0;
  nqueue q;
  loc target = {di, dj};
  int inq = 0;
  if (DEBUG)
    fprintf(file, "Starting to findpath From %d,%d to %d,%d\n", si, sj, di, dj);
  // q.data = (loc *)malloc(sizeof(loc)*400);
  q.start = 0;
  q.end = 1;
  q.size = 400;
  q.data[0].i = si;
  q.data[0].j = sj;
  memset(visited, -1, sizeof(int) * G_HEIGHT * G_WIDTH);
  while (q.start != q.end) {
    if (BFS(&target, &q, visited, -1)) {
      success = 1;
      break;
    }
  }
  // free(q.data);

  if (DEBUG) {
    int i = 0;
    int j = 0;
    fprintf(file, "Grid:\n");
    for (; i < G_HEIGHT; i++) {
      for (j = 0; j < G_WIDTH; j++) {
        fprintf(file, "%2d ", visited[i][j]);
      }
      fprintf(file, "\n");
    }
  }
  return visited[target.i][target.j];
}

int Level::findPathLong(int si, int sj, int di, int dj) {
  int visited[G_HEIGHT][G_WIDTH]; // = (int **)malloc(sizeof(int)**G_WIDTH);
  int success = 0;
  int found = 0;
  nqueue q;
  loc target = {di, dj};
  int inq = 0;
  DWORD stime = GetTickCount();
  if (DEBUG)
    fprintf(file, "Starting to findpath From %d,%d to %d,%d\n", si, sj, di, dj);
  // q.data = (loc *)malloc(sizeof(loc)*400);
  q.start = 0;
  q.end = 1;
  q.size = 400;
  q.data[0].i = si;
  q.data[0].j = sj;
  memset(visited, -1, sizeof(int) * G_HEIGHT * G_WIDTH);

  while (q.start != q.end) {
    if (BFS(&target, &q, visited, -1)) {
      found++;
      if (found == 3) {
        success = 1;
        break;
      }
    }
  }
  // free(q.data);

  if (DEBUG) {
    int i = 0;
    int j = 0;
    fprintf(file, "Grid:\n");
    for (; i < G_HEIGHT; i++) {
      for (j = 0; j < G_WIDTH; j++) {
        fprintf(file, "%2d ", visited[i][j]);
      }
      fprintf(file, "\n");
    }
  }
  return visited[target.i][target.j];
}

int Level::BFS(loc* target, nqueue* q, int visited[G_HEIGHT][G_WIDTH],
               int mark) {
  loc current;
  int pathNum = -1;
  current.i = q->data[q->start].i;
  current.j = q->data[q->start].j;
  q->start = (q->start + 1) % q->size;

  mark = visited[current.i][current.j];
  if (current.i == target->i && current.j == target->j)
    return 1;

  if (canGo2(visited, 0, current.i, current.j, D_UP)) {
    q->data[q->end].i = current.i - 1;
    q->data[q->end].j = current.j;

    if (mark == -1)
      visited[q->data[q->end].i][q->data[q->end].j] = D_UP;
    else
      visited[q->data[q->end].i][q->data[q->end].j] = mark;

    q->end = (q->end + 1) % q->size;
    //	if(BFS(target,q,visited,mark))
    //		return 1;
  }
  if (canGo2(visited, 0, current.i, current.j, D_RIGHT)) {
    q->data[q->end].i = current.i;
    q->data[q->end].j = current.j + 1;

    if (mark == -1)
      visited[q->data[q->end].i][q->data[q->end].j] = D_RIGHT;
    else
      visited[q->data[q->end].i][q->data[q->end].j] = mark;

    q->end = (q->end + 1) % q->size;
    //	if(BFS(target,q,visited,mark))
    //		return 1;
  }
  if (canGo2(visited, 0, current.i, current.j, D_DOWN)) {
    q->data[q->end].i = current.i + 1;
    q->data[q->end].j = current.j;

    if (mark == -1)
      visited[q->data[q->end].i][q->data[q->end].j] = D_DOWN;
    else
      visited[q->data[q->end].i][q->data[q->end].j] = mark;

    q->end = (q->end + 1) % q->size;
    //	if(BFS(target,q,visited,mark))
    //		return 1;
  }

  if (canGo2(visited, 0, current.i, current.j, D_LEFT)) {
    q->data[q->end].i = current.i;
    q->data[q->end].j = current.j - 1;

    if (mark == -1)
      visited[q->data[q->end].i][q->data[q->end].j] = D_LEFT;
    else
      visited[q->data[q->end].i][q->data[q->end].j] = mark;

    q->end = (q->end + 1) % q->size;
    //	if(BFS(target,q,visited,mark))
    //		return 1;
  }
  return 0;
}

int Level::limitedDFS(loc* target, loc* stack, int* instack, int* path,
                      int* inpath, int visited[G_HEIGHT][G_WIDTH], int cutoff) {
  int pathback = *inpath;
  loc current;
  (*instack)--;
  current.i = stack[*instack].i;
  current.j = stack[*instack].j;

  visited[current.i][current.j] = cutoff;
  if (cutoff <= 0)
    return 0;
  if (current.i == target->i && current.j == target->j)
    return 1;
  if (DEBUG)
    fprintf(file, "cutoff is %d: ", cutoff);

  switch (DFSstyle) {
  case 0:
    if (canGo(visited, cutoff, current.i, current.j, D_UP)) {
      if (DEBUG)
        fprintf(file, "Trying up from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i - 1;
      stack[(*instack)++].j = current.j;
      path[*inpath] = D_UP;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_LEFT)) {
      if (DEBUG)
        fprintf(file, "Trying left from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i;
      stack[(*instack)++].j = current.j - 1;
      path[*inpath] = D_LEFT;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_DOWN)) {
      if (DEBUG)
        fprintf(file, "Trying down from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i + 1;
      stack[(*instack)++].j = current.j;
      path[*inpath] = D_DOWN;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_RIGHT)) {
      if (DEBUG)
        fprintf(file, "Trying right from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i;
      stack[(*instack)++].j = current.j + 1;
      path[*inpath] = D_RIGHT;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }
    break;
  case 2:
    if (canGo(visited, cutoff, current.i, current.j, D_DOWN)) {
      if (DEBUG)
        fprintf(file, "Trying down from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i + 1;
      stack[(*instack)++].j = current.j;
      path[*inpath] = D_DOWN;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_LEFT)) {
      if (DEBUG)
        fprintf(file, "Trying left from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i;
      stack[(*instack)++].j = current.j - 1;
      path[*inpath] = D_LEFT;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_UP)) {
      if (DEBUG)
        fprintf(file, "Trying up from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i - 1;
      stack[(*instack)++].j = current.j;
      path[*inpath] = D_UP;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_RIGHT)) {
      if (DEBUG)
        fprintf(file, "Trying right from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i;
      stack[(*instack)++].j = current.j + 1;
      path[*inpath] = D_RIGHT;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }
    break;
  case 3:
    if (canGo(visited, cutoff, current.i, current.j, D_DOWN)) {
      if (DEBUG)
        fprintf(file, "Trying down from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i + 1;
      stack[(*instack)++].j = current.j;
      path[*inpath] = D_DOWN;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_RIGHT)) {
      if (DEBUG)
        fprintf(file, "Trying right from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i;
      stack[(*instack)++].j = current.j + 1;
      path[*inpath] = D_RIGHT;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_UP)) {
      if (DEBUG)
        fprintf(file, "Trying up from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i - 1;
      stack[(*instack)++].j = current.j;
      path[*inpath] = D_UP;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_LEFT)) {
      if (DEBUG)
        fprintf(file, "Trying left from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i;
      stack[(*instack)++].j = current.j - 1;
      path[*inpath] = D_LEFT;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    break;
  case 4:
    if (canGo(visited, cutoff, current.i, current.j, D_UP)) {
      if (DEBUG)
        fprintf(file, "Trying up from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i - 1;
      stack[(*instack)++].j = current.j;
      path[*inpath] = D_UP;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_RIGHT)) {
      if (DEBUG)
        fprintf(file, "Trying right from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i;
      stack[(*instack)++].j = current.j + 1;
      path[*inpath] = D_RIGHT;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_DOWN)) {
      if (DEBUG)
        fprintf(file, "Trying down from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i + 1;
      stack[(*instack)++].j = current.j;
      path[*inpath] = D_DOWN;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    if (canGo(visited, cutoff, current.i, current.j, D_LEFT)) {
      if (DEBUG)
        fprintf(file, "Trying left from %d %d\n", current.i, current.j);
      stack[*instack].i = current.i;
      stack[(*instack)++].j = current.j - 1;
      path[*inpath] = D_LEFT;
      (*inpath)++;
      if (limitedDFS(target, stack, instack, path, inpath, visited, cutoff - 1))
        return 1;
      *inpath = pathback;
    }

    break;
  }
  return 0;
}
int Level::canGo(int visited[G_HEIGHT][G_WIDTH], int cutoff, int i, int j,
                 int dir) {
  switch (dir) {
    /*
case D_UP:
    if(i-1<0)
            return 0;
    return !checkGrid(i-1,j) &&(cutoff>visited[i-1][j]);
case D_DOWN:
    if(i+1>=G_HEIGHT)
            return 0;
    return !checkGrid(i+1,j) &&(cutoff>visited[i+1][j]);
case D_RIGHT:
    if(j+1>=G_WIDTH)
            return 0;
    return !checkGrid(i,j+1)&&(cutoff>visited[i][j+1]);
case D_LEFT:
    if(j-1<0)
            return 0;
    return !checkGrid(i,j-1)&&(cutoff>visited[i][j-1]);
    */
  case D_UP:
    if (i - 1 < 0)
      return 0;
    return !checkGridEnemy(i - 1, j) && (!visited[i - 1][j]);
  case D_DOWN:
    if (i + 1 >= G_HEIGHT)
      return 0;
    return !checkGridEnemy(i + 1, j) && (!visited[i + 1][j]);
  case D_RIGHT:
    if (j + 1 >= G_WIDTH)
      return 0;
    return !checkGridEnemy(i, j + 1) && (!visited[i][j + 1]);
  case D_LEFT:
    if (j - 1 < 0)
      return 0;
    return !checkGridEnemy(i, j - 1) && (!visited[i][j - 1]);
  }
  return 0;
}

inline int Level::canGo2(int visited[G_HEIGHT][G_WIDTH], int cutoff, int i,
                         int j, int dir) {
  switch (dir) {
  case D_UP:
    if (i - 1 < 0)
      return 0;
    return !checkGridEnemy(i - 1, j) && (visited[i - 1][j] == -1);
  case D_DOWN:
    if (i + 1 >= G_HEIGHT)
      return 0;
    return !checkGridEnemy(i + 1, j) && (visited[i + 1][j] == -1);
  case D_RIGHT:
    if (j + 1 >= G_WIDTH)
      return 0;
    return !checkGridEnemy(i, j + 1) && (visited[i][j + 1] == -1);
  case D_LEFT:
    if (j - 1 < 0)
      return 0;
    return !checkGridEnemy(i, j - 1) && (visited[i][j - 1] == -1);
  }
  return 0;
}
