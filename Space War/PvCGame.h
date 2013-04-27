#ifndef PVCGAME_H
#define PVCGAME_H

#pragma once

#include "BaseGame.h"
#include "Utils.h"

class PvCGame : public BaseGame
{
public:
	PvCGame();
	~PvCGame();

	virtual void init();	
	virtual int sendState(int x, int y, int *result);
	virtual int receiveState(int *x, int *y);
	virtual int sendResult(int result);
	virtual void close();

protected:
	void getMin(int &x1,int&y1,int x2,int y2);
	void getMax(int &x1,int&y1,int x2,int y2);
	void drawAround(int x1,int y1,int x2,int y2);

protected:
	vector<vector<int>> AIThinkSpace;//ai blocknote
	vector<vector<int>> map;
	vector<vector<int>> enemy_map;
	vector<Battleship>	ships;

	int lastAttackResult;
	int direction[4];
	int dir;
	int step;
	int loh;
	int injured;
	int x1,y1,x2,y2;
	int x, y;			//chosen location to drop the bomb
	int ts;
	int tempX,tempY,tempD;
};

#endif //PVCGAME_H
