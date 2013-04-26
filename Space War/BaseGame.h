#ifndef BASEGAME_H
#define BASEGAME_H

#pragma once

#include "Utils.h"

using namespace std;

class BaseGame
{
public:
	BaseGame();
	virtual ~BaseGame();

	virtual void init() = 0;	
	virtual int sendState(int x, int y, int *result) = 0;
	virtual int receiveState(int *x, int *y) = 0;
	virtual int sendResult(int result) = 0;
	virtual void close() = 0;
};

#endif //BASEGAME_H

