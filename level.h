#include "globals.h"
#include "compat.h"
#include <stdio.h>


#define DEBUG 0
class Level
{
	private:
  DWORD startTime;
	public:
	
	int foodLocCount;
	int foodLocI;
	int DFSstyle;
	int gwidth;
	int gheight;
	loc * safe;
	int numsafe;
	int foodCount;
	FILE * file;
	tile tiles[G_HEIGHT][G_WIDTH];
	loc foodLoc[160];
	loc fruitStart;
	loc fruitEnd;
	int path[100];
	int foodLocIs[160];
	int foodLocJs[160];
	SDL_Surface* foodRaw;
	SDL_Surface*  gridTiles[NUM_TILES];
	void initTiles();
	void fillFoodTextures(SDL_Surface* test);
	void fillWallTextures(SDL_Surface* test);
	void updateGrid(int i,int j);
	void updateAllGrid();
	void drawLevel(SDL_Surface*  screen);
	int checkGrid(int i, int j);
	int checkGridEnemy(int i, int j);
	int canGo(int visited[G_HEIGHT][G_WIDTH],int cutoff,int i, int j, int dir);
	inline int canGo2(int visited[G_HEIGHT][G_WIDTH],int cutoff,int i, int j, int dir);
	int findPath(int si,int sj, int di, int dj);
	int findPath2(int si,int sj, int di, int dj);
	int findPathLong(int si,int sj, int di, int dj);
	int BFS(loc * target, nqueue * q,int visited[G_HEIGHT][G_WIDTH],int mark);
	int limitedDFS(loc * target,loc * stack,int  *instack,int * path, int * inpath,int visited[G_HEIGHT][G_WIDTH],int cutoff);

	Level(void);
	~Level();
	int loadLevel(char * str);
};
	
