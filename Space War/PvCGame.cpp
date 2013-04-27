#include "stdafx.h"

#include "PvCGame.h"

PvCGame::PvCGame()
	: map(NULL),
	  ships(NULL)
{
}


PvCGame::~PvCGame()
{
}

void PvCGame::setMap(vector<vector<int>> *_map)
{
	map = _map;
}

void PvCGame::setShips(vector<Battleship> *_ships)
{
	ships = _ships;
}

void PvCGame::init()
{
	injured=0;
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
	int temp = map->at(x).at(y);
    
    if(temp >= 0 && temp < 10)
    {
        ships->at(temp).health--;
        if(ships->at(temp).health==0)
		{
			*result = -3;
			
		}
        else 
		{
			*result = -2;
			
		}
    }
	*result = 1;
 
	return 0;
}

int PvCGame::receiveState(int * _x, int * _y)
{
	static int direction[4];
    static int dir;
    static int step;
    static int loh;
    static int status;
    static int x1,y1,x2,y2;
    static int tempX,tempY,tempD;
    int i,j,randNum1,randNum2,f,ts,counter=0;
	int &x = *_x, &y = *_y;
	//first time or with 0 injured ships

    if(injured==0)//impusca ceva nou
    {
        randNum1=rand()%10;
        randNum2=rand()%10;
        f=0;
         for (i=randNum1; i<10; i++)
                for (j=randNum2; j<10; j++)
                if(f==0&&AIThinkSpace.at(i).at(j)==0)
                  {
                    f = 1;
                    x = i;
                    y = j;
                    break;
                }
        if(f==0)
        {
            for (i=0; i<randNum1; i++)
                for (j=0; j<randNum2; j++)
                if(f==0&&AIThinkSpace.at(i).at(j)==0)
                  {
                    f=1;
                    x=i;
                    y=j;
                    break;
                }
        }
        if(f==0)return -1;
        x1=y1=10;
        x2=y2=0;
    }
    else if(step == 1)//second try of injured ships
    {

        do
        {
            f=0;
            ts = rand() %4;
            if (direction[ts]!=0) f=1;
        }
        while(f);
        x=tempX;
        y=tempY;
        tempD=ts;
        switch(ts)
        {
        case 0:
            x-=1;
            break;
            //printf("sus\n");

        case 1:

            x+=1;
            // printf("jos\n");
            break;
        case 2:

            y-=1;
            //  printf("stinga\n");
            break;
        case 3:

            y+=1;
            // printf("dreapta\n");
            break;
        }
    }
    else if(step>1)
    {
        if(loh==1)//STEP>0 o ghicit 2 pozitii si o gresit la a treia
        {
            direction[tempD]=1;
            if(tempD<2)tempD=1-tempD;
            else tempD=5-tempD;
            x=tempX;
            y=tempY;
            if(tempD==0)x=x-step;
            if(tempD==1)x=x+step;
            if(tempD==2)y=y-step;
            if(tempD==3)y=y+step;
        }
        else //nu-i loh
        {
            ts=tempD;
            switch(ts)
            {
            case 0:
                x-=1;
                break;
                //printf("sus\n");

            case 1:

                x+=1;
                // printf("jos\n");
                break;
            case 2:

                y-=1;
                //   printf("stinga\n");
                break;
            case 3:

                y+=1;
                //   printf("dreapta\n");
                break;
            }
        }
    }
    status=receiveState(&x, &y);
    // printf("status: %d\n",status);
    switch(status)
    {
    case 3:
        AIThinkSpace.at(x).at(y)=2;
        // numShips--;
        injured=0;
        step=0;
        getMin(x1,y1,x,y);
        getMax(x2,y2,x,y);
        drawAround(x1,y1,x2,y2);
        x1=y1=10;
        x2=y2=0;
        for(i=0; i<4; i++)direction[i]=0;
        step=0;
        return -status;
    case 2:
        getMin(x1, y1, x, y);
        getMax(x2, y2, x, y);
        // numShips--;
        injured=1;
        step++;
        tempX=x;
        tempY=y;
        AIThinkSpace[x][y]=2;
        if(x==0)direction[0]=1;
        if(x==9)direction[1]=1;
        if(y==0)direction[2]=1;
        if(y==9)direction[3]=1;
        if(x>0)if( AIThinkSpace.at(x-1).at(y)==1)direction[0]=1;
        if(x<9)if( AIThinkSpace.at(x+1).at(y)==1)direction[1]=1;
        if(y>0)if( AIThinkSpace.at(x).at(y-1)==1)direction[2]=1;
        if(y<9)if( AIThinkSpace.at(x).at(y+1))direction[3]=1;
        loh=0;
        return -status;
    case 1:

        if(injured==1)
            direction[ts]=1;
        AIThinkSpace.at(x).at(y)=1;
        loh=1;
        return -1;
    }


	return 0;
}

int PvCGame::sendResult(int result)
{

	return 0;
}

void PvCGame::close()
{
}

void PvCGame::getMin(int &x1,int&y1,int x2,int y2)
{
    if(x1>x2)
    {
        x1=x2;
        y1=y2;
    }
    else if(y1>y2)
    {
        x1=x2;
        y1=y2;
    }
}
void PvCGame::getMax(int &x1,int&y1,int x2,int y2)
{
    if(x1<x2)
    {
        x1=x2;
        y1=y2;
    }
    else if(y1<y2)
    {
        x1=x2;
        y1=y2;
    }
}
void PvCGame::drawAround(int x1,int y1,int x2,int y2)
{
    int i, j;
    int minI=(0<x1) ? x1-1 : x1;
    int maxI=(9>x2) ? x2+1 : x2;
    int minJ=(0<y1) ? y1-1 : y1;
    int maxJ=(9>y2) ? y2+1 : y2;
    for(i=minI; i<=maxI; i++)
        for(j=minJ; j<=maxJ; j++)
            AIThinkSpace.at(i).at(j)=(AIThinkSpace.at(i).at(j)!=2)?1:3;
}