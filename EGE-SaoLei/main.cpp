#include<ege.h>
#define NUM 13 //地雷数目

using namespace ege;

//定义结构体
typedef struct Dilei
{
	int flag = 0;//是否有地雷 没有为0
	int wa = 0;//是否被挖开 没有为0
	int arr = 0;//旁边有几个地雷
}Lei, *pLei;
//初始化地雷
void initLei(Lei lei[][10], int x, int y)
{
	int a, b;
	//随机10个地雷
	for (int i = 0; i < NUM; i++)
	{
		a = random(10);
		b = random(10);
		if (lei[a][b].flag == 0)
		{
			lei[a][b].flag = 1;
		}
		else
			i--;
	}
	//计算周围地雷
	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 9; j++)
		{
			lei[i][j].arr
				= lei[i - 1][j - 1].flag + lei[i - 1][j].flag + lei[i - 1][j + 1].flag //左边
				+ lei[i][j - 1].flag + lei[i][j + 1].flag //中间
				+ lei[i + 1][j - 1].flag + lei[i + 1][j].flag + lei[i + 1][j + 1].flag;//右边
		}
	}
	for (int i = 1; i < 9; i++)
	{
		lei[i][0].arr = lei[i - 1][0].flag + lei[i - 1][1].flag
			+ lei[i][1].flag
			+ lei[i + 1][0].flag + lei[i + 1][1].flag;
		lei[i][9].arr = lei[i - 1][9].flag + lei[i - 1][8].flag
			+ lei[i][8].flag
			+ lei[i + 1][9].flag + lei[i + 1][8].flag;
		lei[0][i].arr = lei[0][i - 1].flag + lei[1][i - 1].flag
			+ lei[1][i].flag
			+ lei[0][i + 1].flag + lei[1][i + 1].flag;
		lei[9][i].arr = lei[9][i - 1].flag + lei[8][i - 1].flag
			+ lei[8][i].flag
			+ lei[9][i + 1].flag + lei[8][i + 1].flag;
	}
	lei[0][0].arr = lei[1][0].flag + lei[0][1].flag + lei[1][1].flag;
	lei[0][9].arr = lei[0][8].flag + lei[1][8].flag + lei[1][9].flag;
	lei[9][9].arr = lei[9][8].flag + lei[8][9].flag + lei[8][8].flag;
	lei[9][0].arr = lei[8][0].flag + lei[9][1].flag + lei[8][1].flag;
}
//递归扫雷
int WaDiLei(int  x, int y, Lei lei[][10])
{
	if (lei[x][y].flag == 1)
	{
		setfontbkcolor(RED);
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (lei[i][j].flag == 1)
				{
					setcolor(EGERGB(0xee, 0x00, 0x00));
					setbkmode(TRANSPARENT);
					xyprintf(i * 30, j * 30, "*");
				}
			}
		}
		return 0;
	}
	return 1;
}

void SouSuo(Lei lei[][10], int x, int y, PIMAGE imgc)
{
	if (lei[x][y].wa == 1)
		return;
	lei[x][y].wa = 1;
	if (lei[x][y].arr > 0)
	{
		putimage(x * 30, y * 30, imgc);
		setfontbkcolor(EGERGB(0x91, 0x91, 0x91));
		setcolor(YELLOW);
		setbkmode(TRANSPARENT);
		xyprintf(x * 30, y * 30, "%d", lei[x][y].arr);
		return;
	}
	if (lei[x][y].arr == 0)
	{
		putimage(x * 30, y * 30, imgc);
		if (x - 1 >= 0)
		{
			SouSuo(lei, x - 1, y, imgc);
		}
		if (x + 1 < 10)
		{
			SouSuo(lei, x + 1, y, imgc);
		}
		if (y + 1 < 10)
		{
			SouSuo(lei, x, y + 1, imgc);
		}
		if (y - 1 >= 0)
		{
			SouSuo(lei, x, y - 1, imgc);
		}
		if (y - 1 >= 0 && x - 1 >= 0)
		{
			SouSuo(lei, x - 1, y - 1, imgc);
		}
		if (y - 1 >= 0 && x + 1 < 10)
		{
			SouSuo(lei, x + 1, y - 1, imgc);
		}
		if (y + 1 < 10 && x - 1 >= 0)
		{
			SouSuo(lei, x - 1, y + 1, imgc);
		}
		if (y + 1 < 10 && x + 1 < 10)
		{
			SouSuo(lei, x + 1, y + 1, imgc);
		}
	}
}

int ifVoctory(Lei lei[][10])
{
	int count = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)

		{
			if (lei[i][j].wa != 1)
			{
				count++;
			}
		}
	}
	if (count == NUM)
	{
		setfontbkcolor(GREEN);
		setcolor(RED);
		setbkmode(OPAQUE);
		xyprintf(45, 135, "VOCTORY");
		return 1;
	}
	return 0;
}

void mainloop(Lei lei[][10], PIMAGE imgc, PIMAGE img)
{
	mouse_msg msg = { 0 };
	for (; is_run(); delay_fps(60))
	{
		while (mousemsg())
		{
			msg = getmouse();
			int x, y;
			x = msg.x / 30;
			y = msg.y / 30;
			if (msg.is_left() && lei[x][y].wa != 2)
			{
				if (WaDiLei(x, y, lei))
				{
					SouSuo(lei, x, y, imgc);
				}
				else
					return;
			}
			else if (msg.is_right() && msg.is_down())
			{
				if (lei[x][y].wa == 0)
				{
					setbkmode(TRANSPARENT);
					setcolor(YELLOW);
					xyprintf(x * 30, y * 30, "?");
					lei[x][y].wa = 2;
				}
				else if (lei[x][y].wa == 2)
				{
					putimage(x * 30, y * 30, img);
					lei[x][y].wa = 0;
				}
			}
		}
		if (ifVoctory(lei))
		{
			return;
		}
	}
}

int main()
{
	PIMAGE img, imgc;

	initgraph(300, 300);
	randomize();//初始化随机数

	Lei lei[10][10];

	img = newimage();
	imgc = newimage();
	//得到img
	setfillcolor(BLUE);
	setcolor(WHITE);
	bar3d(0, 0, 30, 30, 0, 0, NULL);
	getimage(img, 0, 0, 30, 30);
	//得到imgc
	setfillcolor(EGERGB(0x91, 0x91, 0x91));
	setcolor(EGERGB(0x98, 0xF8, 0x98));
	bar3d(0, 0, 30, 30, 0, 0, NULL);
	getimage(imgc, 0, 0, 30, 30);

	setfont(30, 30, "宋体");
	//初始化界面
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			putimage(i * 30, j * 30, img);
		}
	}

	initLei(lei, 10, 10);
	//主循环
	mainloop(lei, imgc, img);

	getch();
	delimage(img);
	delimage(imgc);
	closegraph();
	return 0;
}
