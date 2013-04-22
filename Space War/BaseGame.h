#ifndef BASEGAME_H
#define BASEGAME_H

#pragma once

#include <vector>

using namespace std;

struct Battleship
{
	int totalHealth;
	int health;
};

class BaseGame
{
public:
	BaseGame();
	virtual ~BaseGame();

	virtual void init() = 0;	
	virtual int sendState(int x, int y, bool *result) = 0;
	virtual int receiveState(int *x, int *y) = 0;
	virtual void close() = 0;

protected:
	vector<vector<int>> map;	//map of enemy's battleships
	vector<Battleship>  ships;	//where my ships are located
};

#endif //BASEGAME_H

