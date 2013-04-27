#ifndef PVCGAME_H
#define PVCGAME_H

#pragma once

#include "BaseGame.h"
#include "Utils.h"

//Questions:
//1) How does AI know if he hit us or not?
//2) Notation for attack result: 1, 2, 3 or -1, -2, -3 ?
//3) What does receiveState return?

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

	void	setMap(vector<vector<int>> *map);
	void	setShips(vector<Battleship> *ships);
protected:
	void getMin(int &x1,int&y1,int x2,int y2);
	void getMax(int &x1,int&y1,int x2,int y2);
	void drawAround(int x1,int y1,int x2,int y2);

protected:
	vector<vector<int>> AIThinkSpace;//ai blocknote
	vector<vector<int>> *map;
	vector<Battleship>	*ships;
	int lastAttackResult;
	int injured;
};

#endif //PVCGAME_H
