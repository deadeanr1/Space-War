#include "stdafx.h"

#include "Utils.h"

int randships(int map[10][10],Battleship B[10])
{
    int i,j,f,k,r,h=9;
    int randNum1, randNum2,r1,r2, ts,s; //pozitionare+directie

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

    for (k=0; k<4; k++ )                         //1 size
    {
        randNum1=rand()%10;
        randNum2=rand()%10;
        f=0;
        for (i=randNum1; i<10; i++)
            for (j=randNum2; j<10; j++)
                if(f==0&&map[i][j]!=-1||map[i+1][j]!=-1||map[i-1][j]!=-1||map[i][j+1]!=-1
                        ||map[i][j-1]!=-1||map[i+1][j+1]!=-1||map[i-1][j-1]!=-1||map[i+1][j-1]!=-1
                        ||map[i-1][randNum2+1]!=-1)
                {
                    f=1;
                    randNum1=i;
                    randNum2=j;
                    break;
                }
        if(f==0)
    {
        for (i=0; i<randNum1; i++)
                for (j=0; j<randNum2; j++)
                    if(f==0&&map[i][j]!=-1||map[i+1][j]!=-1||map[i-1][j]!=-1||map[i][j+1]!=-1
                            ||map[i][j-1]!=-1||map[i+1][j+1]!=-1||map[i-1][j-1]!=-1||map[i+1][j-1]!=-1
                            ||map[i-1][randNum2+1]!=-1)
                    {
                        f=1;
                        randNum1=i;
                        randNum2=j;
                        break;
                    }
        }
        /*do
        {
            randNum1 =  rand() %10;
            randNum2=  rand() %10;
        }
        while (map[randNum1][randNum2]!=-1||map[randNum1+1][randNum2]!=-1||map[randNum1-1][randNum2]!=-1||map[randNum1][randNum2+1]!=-1||map[randNum1][randNum2-1]!=-1||map[randNum1+1][randNum2+1]!=-1||map[randNum1-1][randNum2-1]!=-1||map[randNum1+1][randNum2-1]!=-1||map[randNum1-1][randNum2+1]!=-1);
        */
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

void shuffleMap(vector<vector<int>> &map, vector<Battleship> &ships, vector<vector<int>> &enemy_map)
{
    int m[10][10];
    int i,j;
    Battleship B[10];
    vector<int> temp;
    randships(m,B);

    for(int i=0; i<map.size(); i++)
    {
        map.at(i).clear();
    }
    map.clear();

    ships.clear();

    map.resize(10);
    ships.resize(10);
    for (i=0; i<10; i++)
    {
        map.at(i).resize(10);
        for (j=0; j<10; j++)
        {
            map.at(i).at(j) = m[i][j];
            temp.push_back(m[i][j]);
        }
        ships.at(i) = B[i];
    }

    enemy_map.clear();
    enemy_map.resize(10);
    for(size_t i=0; i<10; i++)
    {
        enemy_map[i].resize(10);
        for(size_t j=0; j<10; j++)
        {
            enemy_map.at(i).at(j) = -1;
        }
    }
}

bool loadAndDrawBitmap(HDC hDC,vector<Battleship> &ships)
{
    HDC dcmem = CreateCompatibleDC ( NULL );
    int i;
    HANDLE  hBmp[10];
    BITMAP bm[10];
    char bmpfile[] = "..\\images\\";
    char temp[60];
    if ( ( NULL == hDC  ) /*|| ( NULL == bmpfile )*/ )
    {
        MessageBox(0, _T("Window Parameters Failed!"), _T("Error!"), MB_ICONSTOP | MB_OK);
        return false;
    }
    strcpy (temp, bmpfile);
    strcat (temp,"rship1x.bmp");
    for(i=0; i<4; i++)
    {
        hBmp[i] = LoadImageA ( NULL, temp, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE );
        GetObject ( hBmp[i], sizeof(bm[i]), &bm[i] );
        if ( BitBlt ( hDC, 50+40*ships[i].x1, 130+40*ships[i].y1, bm[i].bmWidth, bm[i].bmHeight, dcmem,
                      0, 0, SRCCOPY ) == 0 )
        {
            // failed the blit
            DeleteDC ( dcmem );
            return false;
        }
    }
    strcpy (temp, bmpfile);
    for(i=4; i<7; i++)
    {
        strcpy (temp, bmpfile);
        if(ships[i].x1!=ships[i].x2)
        {
            strcat (temp,"rship2x.bmp");

        }
        else
        {
            strcat (temp,"rship2xv.bmp");
        }
        hBmp[i] = LoadImageA( NULL, temp, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE );
        GetObject ( hBmp[i], sizeof(bm[i]), &bm[i] );
        if ( BitBlt ( hDC, 50+40*ships[i].x1, 130+40*ships[i].y1, bm[i].bmWidth, bm[i].bmHeight, dcmem,
                      0, 0, SRCCOPY ) == 0 )
        {
            // failed the blit
            DeleteDC ( dcmem );
            return false;
        }
    }
    for(i=7; i<9; i++)
    {
        strcpy (temp, bmpfile);
        if(ships[i].x1!=ships[i].x2)
        {
            strcat (temp,"rship3x.bmp");

        }
        else
        {
            strcat (temp,"rship3xv.bmp");
        }
        hBmp[i] = LoadImageA ( NULL, temp, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE );
        GetObject ( hBmp[i], sizeof(bm[i]), &bm[i] );
        if ( BitBlt ( hDC, 50+40*ships[i].x1, 130+40*ships[i].y1, bm[i].bmWidth, bm[i].bmHeight, dcmem,
                      0, 0, SRCCOPY ) == 0 )
        {
            // failed the blit
            DeleteDC ( dcmem );
            return false;
        }
    }

    strcpy (temp, bmpfile);
    if(ships[i].x1!=ships[i].x2)
    {
        strcat (temp,"rship4x.bmp");

    }
    else
    {
        strcat (temp,"rship4xv.bmp");
    }
    //hBmp[i] = LoadImageA ( NULL, bmpfile, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE );
    hBmp[i] = LoadImageA ( NULL, temp, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE );
    GetObject ( hBmp[i], sizeof(bm[i]), &bm[i] );
    if ( BitBlt ( hDC, 50+40*ships[i].x1, 130+40*ships[i].y1, bm[i].bmWidth, bm[i].bmHeight, dcmem,
                  0, 0, SRCCOPY ) == 0 )
    {
        // failed the blit
        DeleteDC ( dcmem );
        return false;
    }
    DeleteDC ( dcmem );  // clear up the memory dc
    return true;
}
