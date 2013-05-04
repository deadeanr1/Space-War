#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include<windows.h>
using namespace std;

#define SAFE_DELETE(x) { if(x) { delete (x); (x)=NULL; } }

#define MAX_LOADSTRING 130

#define			LeftZone1		50
#define			RightZone1		450
#define			TopZone1		130
#define			BottomZone1		530
#define			LeftZone2		549
#define			RightZone2		949
#define			TopZone2		130
#define			BottomZone2		530

struct Battleship
{
    int totalHealth;
    int health;
    int textureType;
    int x1, y1, x2, y2;
};

int randships(int map[10][10],Battleship B[10]);
void shuffleMap(vector<vector<int>> &map, vector<Battleship> &ships, vector<vector<int>> &enemy_map);
void safePutValue(vector<vector<int>> &map, int i_where, int j_where, int value);

#endif //UTILS_H