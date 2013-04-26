#include "stdafx.h"

#include "PvCGame.h"

PvCGame::PvCGame()
{
}


PvCGame::~PvCGame()
{
}

void PvCGame::init()
{
	for(int i=0; i<AIThinkSpace.size(); i++)
    {
        AIThinkSpace.at(i).clear();
    }
    AIThinkSpace.clear();
    
	AIThinkSpace.resize(10);
    for(int i=0; i<10; i++)
    {
        AIThinkSpace.at(i).resize(10);
        for(int j=0; j<10; j++)
		{
			AIThinkSpace.at(i).at(j)=0;
		}
    }
}

int PvCGame::sendState(int x, int y, int *result)
{
	return 0;
}

int PvCGame::receiveState(int *x, int *y)
{
	return 0;
}

int PvCGame::sendResult(int result)
{
	return 0;
}

void PvCGame::close()
{
}