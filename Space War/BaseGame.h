#ifndef BASEGAME_H
#define BASEGAME_H

#pragma once

#include "stdafx.h"

using namespace std;

#define MAP_SIZE 10

struct Battleship;

class BaseGame
{
public:
	BaseGame();
	virtual ~BaseGame();

	virtual void init() = 0;	
	virtual int sendState(int x, int y, int *result) = 0;
	virtual int receiveState(int *x, int *y) = 0;
	virtual void close() = 0;

protected:
	vector<vector<int>> *map;	//map of my battleships
	vector<Battleship>  *ships;	//where my ships are located
};

#endif //BASEGAME_H

