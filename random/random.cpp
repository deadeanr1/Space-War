#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
struct Battleship
{
    int totalHealth;
    int health;
};
int randships(int map[10][10])
{
    Battleship B[10];

    int i,j,f,k,r,h=10;
    int randNum1, randNum2, ts,s; //pozitionare+directie

    srand(time(NULL));      //huevoznaetce

    for (i=0; i<10; i++)
        for (j=0; j<10; j++)
            map [i][j]=0;

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

        break;
    case 2:
        map[randNum1][randNum2]=h;
        map[randNum1+1][randNum2]=h;
        map[randNum1+2][randNum2]=h;
        map[randNum1+3][randNum2]=h;
        break;
    case 3:
        map[randNum1][randNum2]=h;
        map[randNum1][randNum2-1]=h;
        map[randNum1][randNum2-2]=h;
        map[randNum1][randNum2-3]=h;
        break;
    case 4:
        map[randNum1][randNum2]=h;
        map[randNum1][randNum2+1]=h;
        map[randNum1][randNum2+2]=h;
        map[randNum1][randNum2+3]=h;
        break;
    }
    h--;
    //B[h].totalHealth=4;
    // B[h].health=4;

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
                            if (map[i][j]!=0)
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
                            if (map[i][j]!=0)
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
                            if (map[i][j]!=0)
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
                            if (map[i][j]!=0)
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
            break;
        case 2:
            map[randNum1][randNum2]=h;
            map[randNum1+1][randNum2]=h;
            map[randNum1+2][randNum2]=h;
            break;
        case 3:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2-1]=h;
            map[randNum1][randNum2-2]=h;
            break;
        case 4:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2+1]=h;
            map[randNum1][randNum2+2]=h;
            break;
        }
        h--;
        // B[h].totalHealth=3;
        //B[h].health=3;
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
                            if (map[i][j]!=0)
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
                            if (map[i][j]!=0)
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
                            if (map[i][j]!=0)
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
                            if (map[i][j]!=0)
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
            break;
        case 2:
            map[randNum1][randNum2]=h;
            map[randNum1+1][randNum2]=h;
            break;
        case 3:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2-1]=h;
            break;
        case 4:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2+1]=h;
            break;
        }

        h--;
        // B[h].totalHealth=2;
        // B[h].health=2;
    }

    for (i=0; i<4; i++ )                         //1 size
    {
        do
        {
            randNum1 =  rand() %10;
            randNum2=  rand() %10;
        }
        while (map[randNum1][randNum2]!=0||map[randNum1+1][randNum2]!=0||map[randNum1-1][randNum2]!=0||map[randNum1][randNum2+1]!=0||map[randNum1][randNum2-1]!=0||map[randNum1+1][randNum2+1]!=0||map[randNum1-1][randNum2-1]!=0||map[randNum1+1][randNum2-1]!=0||map[randNum1-1][randNum2+1]!=0);

        map[randNum1][randNum2]=h;
        h--;
        //    B[h].totalHealth=1;
        //  B[h].health=1;
    }


    return 1;
}

int main()
{
    int arr[10][10],i,j;
    randships(arr);

    for (i=0; i<10; i++)
    {
        for (j=0; j<10; j++)
            printf("%d\t",arr [i][j]);
        printf("\n");
    }
    return 0;
}
