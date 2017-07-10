#ifndef __DATA_H__
#define __DATA_H__

/* 边框宽度 */
#define BORDER_WIDTH	60

/* 边框长度 */
#define BORDER_HEIGHT	20

/* 贪吃蛇头部初始x位置 */
#define SNAKE_HEAD_POS_X 	10

/* 贪吃蛇头部初始y位置 */
#define SNAKE_HEAD_POS_Y 	10

/* 贪吃蛇尾部初始x位置 */
#define SNAKE_TAIL_POS_X 	5	

/* 贪吃蛇尾部初始y位置 */
#define SNAKE_TAIL_POS_Y 	10

/* 上移光标 */
#define MOVEUP(x) printf("\033[%dA", (x))

/* 下移光标 */
#define MOVEDOWN(x) printf("\033[%dB", (x)) 

/* 左移光标 */
#define MOVELEFT(y) printf("\033[%dD", (y))

/* 右移光标 */
#define MOVERIGHT(y) printf("\033[%dC",(y))

/* 隐藏光标 */
#define HIDE_CURSOR() printf("\033[?25l")

/* 清除屏幕 */
#define CLEAR() printf("\033[2J")

/* 定义方向枚举 */
typedef enum
{
    direct_up       = 0,
    direct_down     = 1,
    direct_left     = 2,
    direct_right    = 3
}DIRECT_EN;

/* 定义坐标结构体 */
typedef struct __POS
{
	__POS()
	{
		x = 0;
		y = 0;
	}

    ~__POS(){};

	int x;
	int y;
}POS_S;

#endif //__DATA_H__
