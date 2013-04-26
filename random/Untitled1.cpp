#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
int arr[10][10];
int AISpace[10][10];
struct Battleship
{
    int totalHealth;
    int health;
    int textureType;
    int x1,y1,x2,y2;
};
Battleship B[10];
int randships(int map[10][10],Battleship B[10]);
int check(int x,int y);
int think(int &x,int &y,int &status);
void getMax(int&,int&,int,int);
void getMin(int&,int&,int,int);
void drawAround(int,int,int,int);
//void explodeShip(int x,int y, int AISpace[10][10]);

int main()
{
    int i,j,x=0,y=0,status=-1;
    for (i=0; i<5; i++)randships(arr,B);
    for (i=0; i<10; i++)
    {
        for (j=0; j<10; j++)
            printf("%d\t",arr [i][j]);
        printf("\n");
    }
    // for (i=0; i<10; i++)printf("%d\n",B [i].health);
    for(i=0; i<10; i++)for(j=0; j<10; j++)AISpace[i][j]=0;
    for (i=0; i<50; i++)
    {
        printf("\ntry %d\n",i+1);
        think(x,y,status);
    }

    for (i=0; i<10; i++)
    {
        for (j=0; j<10; j++)
            printf("%d\t",AISpace [i][j]);
        printf("\n");
    }


    return 0;
}
int randships(int map[10][10],Battleship B[10])
{


    int i,j,f,k,r,h=9;
    int randNum1, randNum2, ts,s; //pozitionare+directie

    srand(time(NULL));      //huevoznaetce

    for (i=0; i<10; i++)
        for (j=0; j<10; j++)
            map [i][j]=-1;

    do
    {

        f=0;                                 //4-sized ship
        randNum1 =  rand() %10;
        randNum2=  rand() %10;

        ts = 1+rand()%4;
        //verify if ship does not exceed the limit of map
        if (ts== 1&&0<=randNum1 &&randNum1<3)f=1;
        if (ts== 2 && 7 <= randNum1 && randNum1 < 10)f=1;

        if (ts== 3 && 0<=randNum2 &&randNum2<3)f=1;
        if (ts== 4 && 7 <= randNum2 && randNum2 < 10)f=1;

        //    printf("%d %d %d %d\n",randNum1,randNum2,ts,i);
    }
    while (f==1);
    switch (ts)
    {
    case 1:
        map[randNum1][randNum2]=h;
        map[randNum1-1][randNum2]=h;
        map[randNum1-2][randNum2]=h;
        map[randNum1-3][randNum2]=h;

        B[h].x1=randNum1-3;
        B[h].x2=randNum1;
        B[h].y1=B[h].y2=randNum2;

        break;
    case 2:
        map[randNum1][randNum2]=h;
        map[randNum1+1][randNum2]=h;
        map[randNum1+2][randNum2]=h;
        map[randNum1+3][randNum2]=h;

        B[h].x1=randNum1;
        B[h].x2=randNum1+3;
        B[h].y1=B[h].y2=randNum2;

        break;
    case 3:
        map[randNum1][randNum2]=h;
        map[randNum1][randNum2-1]=h;
        map[randNum1][randNum2-2]=h;
        map[randNum1][randNum2-3]=h;

        B[h].x1=B[h].x2=randNum1;
        B[h].y1=randNum2-3;
        B[h].y2=randNum2;

        break;
    case 4:
        map[randNum1][randNum2]=h;
        map[randNum1][randNum2+1]=h;
        map[randNum1][randNum2+2]=h;
        map[randNum1][randNum2+3]=h;

        B[h].x1=B[h].x2=randNum1;
        B[h].y1=randNum2;
        B[h].y2=randNum2+3;

        break;
    }
    B[h].totalHealth=4;
    B[h].textureType=3;
     B[h].health=4;
     h--;

    for (k=0; k<2; k++)                                     //?3sized ship
    {
        do
        {
            f=0;
            randNum1 =  rand() %10;
            randNum2=  rand() %10;

            ts = 1+rand()%4;

            if (ts==1)
            {
                if(randNum1>1 )//up damper
                {
                    for (i=randNum1-3; i<randNum1+2; i++)
                        for (j=randNum2-1; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==2)
            {
                if (randNum1<8)//down damper
                {
                    for (i=randNum1-1; i<=randNum1+3; i++)
                        for (j=randNum2-1; j<=randNum2+1; j++)//
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==3)
            {
                if (randNum2>1)//left damper
                {
                    for (i=randNum1-1; i<=randNum1+1; i++)
                        for (j=randNum2-3; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==4)
            {
                if (randNum2<8) //right damper
                {
                    for (i=randNum1-1; i<=randNum1+1; i++)
                        for (j=randNum2-1; j<=randNum2+3; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
        }
        while (f==1);
        switch (ts)
        {
        case 1:
            map[randNum1][randNum2]=h;
            map[randNum1-1][randNum2]=h;
            map[randNum1-2][randNum2]=h;

            B[h].x1=randNum1-2;
            B[h].x2=randNum1;
            B[h].y1=B[h].y2=randNum2;

            break;
        case 2:
            map[randNum1][randNum2]=h;
            map[randNum1+1][randNum2]=h;
            map[randNum1+2][randNum2]=h;

            B[h].x1=randNum1;
            B[h].x2=randNum1+2;
            B[h].y1=B[h].y2=randNum2;

            break;
        case 3:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2-1]=h;
            map[randNum1][randNum2-2]=h;

            B[h].x1=B[h].x2=randNum1;
            B[h].y1=randNum2-2;
            B[h].y2=randNum2;

            break;
        case 4:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2+1]=h;
            map[randNum1][randNum2+2]=h;

            B[h].x1=B[h].x2=randNum1;
            B[h].y1=randNum2;
            B[h].y2=randNum2+2;

            break;
        }

        B[h].totalHealth=3;
        B[h].health=3;
        B[h].textureType=2;
        h--;
    }

    for (k=0; k<3; k++)                                     //2 sized ships
    {
        do
        {
            f=0;
            randNum1 =  rand() %10;
            randNum2=  rand() %10;

            ts = 1+rand()%4;

            if (ts==1)
            {
                if(randNum1 > 0)
                {
                    for (i=randNum1-2; i<randNum1+2; i++)
                        for (j=randNum2-1; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==2)
            {
                if (randNum1< 9)
                {
                    for (i=randNum1-1; i<=randNum1+2; i++)
                        for (j=randNum2-1; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==3)
            {
                if (randNum2> 0)
                {
                    for (i=randNum1-1; i<=randNum1+1; i++)
                        for (j=randNum2-2; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==4)
            {
                if (randNum2 < 9)
                {
                    for (i=randNum1-1; i<=randNum1+1; i++)
                        for (j=randNum2-1; j<=randNum2+2; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
        }
        while (f==1);
        switch (ts)
        {
        case 1:
            map[randNum1][randNum2]=h;
            map[randNum1-1][randNum2]=h;

             B[h].x1=randNum1-1;
            B[h].x2=randNum1;
            B[h].y1=B[h].y2=randNum2;

            break;
        case 2:
            map[randNum1][randNum2]=h;
            map[randNum1+1][randNum2]=h;

              B[h].x1=randNum1;
            B[h].x2=randNum1+1;
            B[h].y1=B[h].y2=randNum2;
            break;
        case 3:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2-1]=h;

             B[h].x1=B[h].x2=randNum1;
            B[h].y1=randNum2-1;
            B[h].y2=randNum2;

            break;
        case 4:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2+1]=h;

             B[h].x1=B[h].x2=randNum1;
            B[h].y1=randNum2;
            B[h].y2=randNum2+1;

            break;
        }
         B[h].totalHealth=2;
        B[h].health=2;
        B[h].textureType=1;
        h--;
    }

    for (i=0; i<4; i++ )                         //1 size
    {
        do
        {
            randNum1 =  rand() %10;
            randNum2=  rand() %10;
        }
        while (map[randNum1][randNum2]!=-1||map[randNum1+1][randNum2]!=-1||map[randNum1-1][randNum2]!=-1||map[randNum1][randNum2+1]!=-1||map[randNum1][randNum2-1]!=-1||map[randNum1+1][randNum2+1]!=-1||map[randNum1-1][randNum2-1]!=-1||map[randNum1+1][randNum2-1]!=-1||map[randNum1-1][randNum2+1]!=-1);

        map[randNum1][randNum2]=h;
        B[h].totalHealth=1;
          B[h].health=1;
          B[h].textureType=0;
          B[h].x1=B[h].x2=randNum1;
          B[h].y1=B[h].y2=randNum2;
           h--;
    }


    return 1;
}

int check(int x,int y)
{
    int temp=arr[x][y];
   printf("x: %d Y: %d Corabia:%d Sanatatea:%d\n",x+1,y+1,temp,B[temp].health);
    if(arr[x][y]!=-1)
    {
        B[temp].health--;
        // arr[x][y]=777;

        if(B[temp].health==0)return 3;
        else return 2;
    }
    return 1;
}
int think(int &x,int &y,int &status)
{
    static int direction[4];
    static int dir,tempDir;
    static int step;
    static int injured;
    static int loh;
    static int numShips;
    static int x1,y1,x2,y2;
    //  static int AISpace[10][10];
    static int tempX,tempY,tempD;
    int i,j,randNum1,randNum2,f,ts,counter=0;
    /* 0,curat 1-mima 2-ranin 3-ubit**/
    // printf("injured: %d\n",injured);
    if(status==-1)
    {
        injured=0;
        numShips=20;
    }
    //srand(time(NULL));
    if(numShips==0)
    {
       return 1;
    }
    //first time or with 0 injured ships
    do
    {
        if(numShips==0)return 1;
        if(injured==0)//impusca ceva nou
        {
            do
            {
                f=0;
                randNum1=rand()%10;
                randNum2=rand()%10;
                if(AISpace[randNum1][randNum2]!=0)f=1;
                x=randNum1;
                y=randNum2;
            }
            while(f);
            x1=y1=10;
            x2=y2=0;
        }
        else if(step==1)//second try of injured ships
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
            if(ts==0)
            {
                x-=1;
                printf("sus\n");
            }
            if(ts==1)
            {
                x+=1;
                printf("jos\n");
            }
            if(ts==2)
            {
                y-=1;
                printf("stinga\n");
            }
            if(ts==3)
            {
                y+=1;
                printf("dreapta\n");
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
            else
            {
                ts=tempD;
                if(ts==0)
                {
                    x-=1;
                    printf("sus\n");
                }
                if(ts==1)
                {
                    x+=1;
                    printf("jos\n");
                }
                if(ts==2)
                {
                    y-=1;
                    printf("stinga\n");
                }
                if(ts==3)
                {
                    y+=1;
                    printf("dreapta\n");
                }
            }
        }
        status=check(x,y);
        printf("status: %d\n",status);
        if(status==3)
        {
            AISpace[x][y]=2;
            numShips--;
            injured=0;
            step=0;
            getMin(x1,y1,x,y);
            getMax(x2,y2,x,y);
            drawAround(x1,y1,x2,y2);
            x1=y1=10;
            x2=y2=0;
            //explodeShip(x,y,AISpace);
            for(i=0; i<4; i++)direction[i]=0;
            step=0;

        }
        else if(status==2)
        {
            getMin(x1,y1,x,y);
            getMax(x2,y2,x,y);
            numShips--;
            injured=1;
            step++;
            tempX=x;
            tempY=y;
            AISpace[x][y]=2;
            if(x==0)direction[0]=1;
            if(x==9)direction[1]=1;
            if(y==0)direction[2]=1;
            if(y==9)direction[3]=1;
            if(x>0)if(AISpace[x-1][y]==1)direction[0]=1;
            if(x<9)if(AISpace[x+1][y]==1)direction[1]=1;
            if(y>0)if(AISpace[x][y-1]==1)direction[2]=1;
            if(y<9)if(AISpace[x][y+1]==1)direction[3]=1;
            loh=0;
        }
        else  if(status==1)
        {
            if(injured==1)
                direction[ts]=1;
            AISpace[x][y]=1;
            loh=1;
            return 0;
        }
       // printf("%d\n",status);
        for (i=0; i<10; i++)
        {
            for (j=0; j<10; j++)
                printf("%d\t",AISpace [i][j]);

            printf("\n");
        }
        getche();
    }
    while(status);
    return 1;
}
void explodeShip(int x,int y, int AISpace[10][10])
{
    if(AISpace[x][y]==2)AISpace[x][y]=3;
    if(x>0)
    {
        if(AISpace[x-1][y]==2)explodeShip(x-1,y,AISpace);
        else if(AISpace[x-1][y]==0)AISpace[x-1][y]=1;
    }
    if(y>0)
    {
        if(AISpace[x][y-1]==2) explodeShip(x,y-1,AISpace);
        else if(AISpace[x][y-1]==0)AISpace[x][y-1]=1;
    }
    if(x<9)
    {
        if(AISpace[x+1][y]==2) explodeShip(x+1,y,AISpace);
        else if(AISpace[x+1][y]==0)AISpace[x+1][y]=1;
    }
    if(y<9)
    {
        if(AISpace[x][y+1]==2) explodeShip(x,y+1,AISpace);
        else  if(AISpace[x][y+1]==0)AISpace[x][y+1]=1;
    }
    if(x<9&&y<9)
    {
        if(AISpace[x+1][y+1]==2) explodeShip(x+1,y+1,AISpace);
        else  if(AISpace[x+1][y+1]==0)AISpace[x+1][y+1]=1;
    }
     if(x>0&&y>0)
     {
         if(AISpace[x-1][y-1]==2) explodeShip(x-1,y-1,AISpace);
        else  if(AISpace[x-1][y-1]==0)AISpace[x-1][y-1]=1;
     }
      if(x<9&&y>0)
     {
         if(AISpace[x+1][y-1]==2) explodeShip(x+1,y-1,AISpace);
        else  if(AISpace[x+1][y-1]==0)AISpace[x+1][y-1]=1;
     }
      if(x>0&&y<9)
     {
         if(AISpace[x-1][y+1]==2) explodeShip(x-1,y+1,AISpace);
        else  if(AISpace[x-1][y+1]==0)AISpace[x-1][y+1]=1;
     }
}
void getMin(int &x1,int&y1,int x2,int y2)
{
    if(x1>x2){x1=x2;y1=y2;}
    else if(y1>y2){x1=x2;y1=y2;}
}
void getMax(int &x1,int&y1,int x2,int y2)
{
    if(x1<x2){x1=x2;y1=y2;}
    else if(y1<y2){x1=x2;y1=y2;}
}
void drawAround(int x1,int y1,int x2,int y2)
{
    int i,j;
    int minI=(0<x1)?x1-1:x1;
    int maxI=(9>x2)?x2+1:x2;
    int minJ=(0<y1)?y1-1:y1;
    int maxJ=(9>y2)?y2+1:y2;
    for(i=minI;i<=maxI;i++)
     for(j=minJ;j<=maxJ;j++)
      AISpace[i][j]=(AISpace[i][j]!=2)?1:3;
}
