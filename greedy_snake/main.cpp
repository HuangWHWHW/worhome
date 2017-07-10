#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#include "conio.h"
#include "data.h"

using namespace std;

/* 当前光标位置 */
POS_S stCurrentPos;

/* 当前蛇尾位置 */
POS_S stTailPos;

/* 当前虫子位置 */
POS_S stBeanPos;

/* 蛇尾缩进队列 */
vector<int> vecTail;

/* 蛇尾缩进队列尾指针 */
int iVecTail = 0;

/* 蛇全身坐标 */
int aSnakePos[BORDER_WIDTH][BORDER_HEIGHT] = {0};

/* 游戏结束标识 */
bool DeadFlag = false;

/* 函数指针 */
typedef void PfGoStep(void);

/* 记录蛇当前前进方向 */
int currentDirect = direct_right;

/* 蛇移动锁 */
bool snakeLock = false;

/* 退出标志 */
bool quit = false;

PfGoStep *pfGoStep[4];

/* 边框 */
void border()
{
	for (int i = 0; i < BORDER_WIDTH; ++i)
	{
		cout << "*";
	}
	cout << endl;
	
	for (int i = 0; i < BORDER_HEIGHT - 2; ++i)
	{
		cout << "**";
		for (int j = 0; j < BORDER_WIDTH - 4; ++j)
		{
			cout << " ";
		}
		cout << "**" << endl;
	}
	
	for (int i = 0; i < BORDER_WIDTH; ++i)
	{
		cout << "*";
	}
	cout << endl;
}

void GoToPos(int x, int y)
{
    if (x < stCurrentPos.x)
    {
        MOVELEFT(stCurrentPos.x - x);
    }
    else if (x > stCurrentPos.x)
    {
        MOVERIGHT(x - stCurrentPos.x);
    }
    
    if (y < stCurrentPos.y)
    {
        MOVEUP(stCurrentPos.y - y);
    }
    else if (y > stCurrentPos.y)
    {
        MOVEDOWN(y - stCurrentPos.y);
    }
}

void UpdateTail()
{
    int dir = vecTail[iVecTail++];
    switch (dir)
    {
        case direct_up:
        {
            stTailPos.y = stTailPos.y - 1;
            break;
        }
        case direct_down:
        {
            stTailPos.y = stTailPos.y + 1;
            break;
        }
        case direct_left:
        {
            stTailPos.x = stTailPos.x - 1;
            break;
        }
        case direct_right:
        {
            stTailPos.x = stTailPos.x + 1;
            break;
        }
    }
}

void FreshBean()
{
    stBeanPos.x = random() % (BORDER_WIDTH - 4) + 2;
    stBeanPos.y = random() % (BORDER_HEIGHT - 2) + 1;
    
    GoToPos(stBeanPos.x, stBeanPos.y);
    cout << '$' << flush;
    
    POS_S stPosTemp;
    stPosTemp.x = stCurrentPos.x;
    stPosTemp.y = stCurrentPos.y;
    stCurrentPos.x = stBeanPos.x + 1;
    stCurrentPos.y = stBeanPos.y;
    
    GoToPos(stPosTemp.x, stPosTemp.y);
    stCurrentPos.x = stPosTemp.x;
    stCurrentPos.y = stPosTemp.y;
}

void CleanTail()
{
    aSnakePos[stTailPos.x][stTailPos.y] = 0;
    GoToPos(stTailPos.x - 1, stTailPos.y);
    cout << ' ' << flush;
    
    POS_S stPosTemp;
    stPosTemp.x = stCurrentPos.x;
    stPosTemp.y = stCurrentPos.y;
    stCurrentPos.x = stTailPos.x;
    stCurrentPos.y = stTailPos.y;
    
    GoToPos(stPosTemp.x, stPosTemp.y);
    stCurrentPos.x = stPosTemp.x;
    stCurrentPos.y = stPosTemp.y;
}

bool IsDead()
{
    DeadFlag = true;
    if (stCurrentPos.x > BORDER_WIDTH - 3 || stCurrentPos.x < 2)
    {
        return true;
    }
    
    if (stCurrentPos.y > BORDER_HEIGHT - 2 || stCurrentPos.y < 1)
    {
        return true;
    }
    
    if (1 == aSnakePos[stCurrentPos.x][stCurrentPos.y])
    {
        return true;
    }
    
    DeadFlag = false;
    return false;
}

bool GetBean()
{
    return ((stCurrentPos.x - 1 == stBeanPos.x) && (stCurrentPos.y == stBeanPos.y));
}

void GoUp()
{
	GoToPos(stCurrentPos.x - 1, stCurrentPos.y - 1);
	cout << 'x' << flush;
	stCurrentPos.y = stCurrentPos.y - 1;
    if (IsDead())
    {
        return;
    }
    aSnakePos[stCurrentPos.x][stCurrentPos.y] = 1;
    
    if (! GetBean())
    {
        CleanTail();
        UpdateTail();
    }
    else
    {
        FreshBean();
    }
}

void GoLeft()
{
    GoToPos(stCurrentPos.x - 2, stCurrentPos.y);
    cout << 'x' << flush;
    stCurrentPos.x = stCurrentPos.x - 1;
    if (IsDead())
    {
        return;
    }
    aSnakePos[stCurrentPos.x][stCurrentPos.y] = 1;
    
    if (! GetBean())
    {
        CleanTail();
        UpdateTail();
    }
    else
    {
        FreshBean();
    }
}

void GoRight()
{
    GoToPos(stCurrentPos.x, stCurrentPos.y);
    cout << 'x' << flush;
    stCurrentPos.x = stCurrentPos.x + 1;
    if (IsDead())
    {
        return;
    }
    aSnakePos[stCurrentPos.x][stCurrentPos.y] = 1;
    
    if (! GetBean())
    {
        CleanTail();
        UpdateTail();
    }
    else
    {
        FreshBean();
    }
}

void GoDown()
{
    GoToPos(stCurrentPos.x - 1, stCurrentPos.y + 1);
    cout << 'x' << flush;
    stCurrentPos.y = stCurrentPos.y + 1;
    if (IsDead())
    {
        return;
    }
    aSnakePos[stCurrentPos.x][stCurrentPos.y] = 1;
    
    if (! GetBean())
    {
        CleanTail();
        UpdateTail();
    }
    else
    {
        FreshBean();
    }
}

/* 初始化贪吃蛇位置 */
void init()
{
    HIDE_CURSOR();
    stCurrentPos.y	= BORDER_HEIGHT;
    stTailPos.x 	= SNAKE_TAIL_POS_X + 2;
    stTailPos.y 	= SNAKE_TAIL_POS_Y + 1;
    GoToPos(SNAKE_TAIL_POS_X + 2, SNAKE_TAIL_POS_Y + 1);
    stCurrentPos.x 	= SNAKE_HEAD_POS_X + 1;
    stCurrentPos.y	= SNAKE_HEAD_POS_Y + 1;
    cout << "xxxxx" << flush;
    vecTail.push_back(direct_right);
    vecTail.push_back(direct_right);
    vecTail.push_back(direct_right);
    vecTail.push_back(direct_right);
    
    aSnakePos[stTailPos.x][stTailPos.y] = 1;
    aSnakePos[stTailPos.x + 1][stTailPos.y] = 1;
    aSnakePos[stTailPos.x + 2][stTailPos.y] = 1;
    aSnakePos[stTailPos.x + 3][stTailPos.y] = 1;
    aSnakePos[stTailPos.x + 4][stTailPos.y] = 1;
    
    /* 更新虫子 */
    FreshBean();
    
    pfGoStep[direct_up] = GoUp;
    pfGoStep[direct_left] = GoLeft;
    pfGoStep[direct_down] = GoDown;
    pfGoStep[direct_right] = GoRight;
}

/* 蛇自动移动 */
void *autoMove(void *args)
{
    while ((! DeadFlag) && (! quit))
    {
        while (snakeLock)
        {
            struct timeval t;
            t.tv_sec = 0;
            t.tv_usec = 50000;
            select(1, NULL, NULL, NULL, &t);
        }
        snakeLock = true;
        pfGoStep[currentDirect]();
        vecTail.push_back(currentDirect);
        snakeLock = false;
        
        struct timeval t;
        t.tv_sec = 0;
        t.tv_usec = 500000;
        select(1, NULL, NULL, NULL, &t);
        //sleep(1);
    }
    
    return NULL;
}

int main()
{
	border();
	init();
    
    char ch = getch();
    //autoMove(NULL);
#if 1
    /* 开启子线程让蛇自动移动 */
    pthread_t tids;
    int ret = pthread_create(&tids, NULL, autoMove, NULL);
    if (ret != 0)
    {
        cout << "create thread failed!" << endl;
        return 0;
    }
    
    /* 获取键盘输入调整蛇的移动方向 */
	while (ch != 'q')
	{
        int direct;
        int opDirect;
		switch (ch)
		{
			case 'w' :
			{
                opDirect = direct_down;
                direct = direct_up;
				break;
			}
			case 'a' : 
			{
                opDirect = direct_right;
                direct = direct_left;
				break;
			}
			case 's' :
			{
                opDirect = direct_up;
                direct = direct_down;
				break;
			}
			case 'd' :
			{
                opDirect = direct_left;
                direct = direct_right;
				break;
			}
            default :
            {
                opDirect = currentDirect;
            }
		}
        
        if (currentDirect != opDirect)
        {
            while (snakeLock)
            {
                struct timeval t;
                t.tv_sec = 0;
                t.tv_usec = 50000;
                select(1, NULL, NULL, NULL, &t);
            }
            snakeLock = true;
            currentDirect = direct;
            pfGoStep[currentDirect]();
            vecTail.push_back(currentDirect);
            snakeLock = false;
        }
        
        if (DeadFlag)
        {
            break;
        }
		ch = getch();
	}
    pthread_join(tids, NULL);
#endif
    GoToPos(BORDER_WIDTH, BORDER_HEIGHT + 2);
	cout << "GOME OVER!" << endl;
	return 0;
}
