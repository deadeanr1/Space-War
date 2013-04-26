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
	vector<vector<int>> AIThinkSpace;//ai blocknote
};

#endif //PVCGAME_H
