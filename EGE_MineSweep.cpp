#include<ege.h>

#define NUM 42 //地雷数目
#define BLOCK_NUM 20 //每个边的方块数量，即总方块数 = BLOCK_NUM * BLOCK_NUM
#define BLOCK_SIZE 30 //方块的大小，即边长 = BLOCK_NUM * BLOCK_SIZE

#define MINE 1
#define FLAG 2

#define SWEPT 1
#define NORMAL 0

using namespace ege;

//定义结构体
struct Block {
    int type = NORMAL;//方块类型 NORMAL、MINE
    int state = NORMAL;//方块状态 NORMAL、SWEPT、FLAG
    int around = 0;//方块周围有几个地雷
};

//初始化地雷
void initBlocks(Block blocks[][BLOCK_NUM]) {
    int x, y;
    //随机放置地雷
    for (int i = 0; i < NUM;) {
        x = random(BLOCK_NUM);
        y = random(BLOCK_NUM);
        if (blocks[x][y].type == NORMAL) {
            blocks[x][y].type = MINE;
            i++;
        }
    }
    //计算周围地雷
    for (int i = 1; i < BLOCK_NUM - 1; i++) {
        for (int j = 1; j < BLOCK_NUM - 1; j++) {
            blocks[i][j].around
                    = blocks[i - 1][j - 1].type + blocks[i - 1][j].type + blocks[i - 1][j + 1].type //左边
                      + blocks[i][j - 1].type + blocks[i][j + 1].type //中间
                      + blocks[i + 1][j - 1].type + blocks[i + 1][j].type + blocks[i + 1][j + 1].type;//右边
        }
    }
    for (int i = 1; i < BLOCK_NUM - 1; i++) {
        blocks[i][0].around = blocks[i - 1][0].type + blocks[i - 1][1].type
                              + blocks[i][1].type
                              + blocks[i + 1][0].type + blocks[i + 1][1].type;
        blocks[i][BLOCK_NUM - 1].around = blocks[i - 1][BLOCK_NUM - 1].type + blocks[i - 1][BLOCK_NUM - 2].type
                                          + blocks[i][BLOCK_NUM - 2].type
                                          + blocks[i + 1][BLOCK_NUM - 1].type + blocks[i + 1][BLOCK_NUM - 2].type;
        blocks[0][i].around = blocks[0][i - 1].type + blocks[1][i - 1].type
                              + blocks[1][i].type
                              + blocks[0][i + 1].type + blocks[1][i + 1].type;
        blocks[BLOCK_NUM - 1][i].around = blocks[BLOCK_NUM - 1][i - 1].type + blocks[BLOCK_NUM - 2][i - 1].type
                                          + blocks[BLOCK_NUM - 2][i].type
                                          + blocks[BLOCK_NUM - 1][i + 1].type + blocks[BLOCK_NUM - 2][i + 1].type;
    }
    blocks[0][0].around = blocks[1][0].type + blocks[0][1].type + blocks[1][1].type;
    blocks[0][BLOCK_NUM - 1].around = blocks[0][BLOCK_NUM - 2].type
                                      + blocks[1][BLOCK_NUM - 2].type
                                      + blocks[1][BLOCK_NUM - 1].type;
    blocks[BLOCK_NUM - 1][BLOCK_NUM - 1].around = blocks[BLOCK_NUM - 1][BLOCK_NUM - 2].type
                                                  + blocks[BLOCK_NUM - 2][BLOCK_NUM - 1].type
                                                  + blocks[BLOCK_NUM - 2][BLOCK_NUM - 2].type;
    blocks[BLOCK_NUM - 1][0].around = blocks[BLOCK_NUM - 2][0].type
                                      + blocks[BLOCK_NUM - 2][1].type
                                      + blocks[BLOCK_NUM - 1][1].type;

}

//递归扫雷
int sweepMine(int x, int y, Block blocks[][BLOCK_NUM]) {
    if (blocks[x][y].type == MINE) {
        setfontbkcolor(RED);
        for (int i = 0; i < BLOCK_NUM; i++) {
            for (int j = 0; j < BLOCK_NUM; j++) {
                if (blocks[i][j].type == MINE) {
                    setcolor(EGERGB(0xee, 0x00, 0x00));
                    setbkmode(TRANSPARENT);
                    xyprintf(i * BLOCK_SIZE, j * BLOCK_SIZE, "*");
                }
            }
        }
        return 0;
    }
    return 1;
}

void search(Block blocks[][BLOCK_NUM], int x, int y, PIMAGE sweptImg) {
    if (blocks[x][y].state == SWEPT)
        return;
    blocks[x][y].state = SWEPT;
    if (blocks[x][y].around > 0) {
        putimage(x * BLOCK_SIZE, y * BLOCK_SIZE, sweptImg);
        setfontbkcolor(EGERGB(0x91, 0x91, 0x91));
        setcolor(YELLOW);
        setbkmode(TRANSPARENT);
        xyprintf(x * BLOCK_SIZE, y * BLOCK_SIZE, "%d", blocks[x][y].around);
        return;
    }
    if (blocks[x][y].around == 0) {
        putimage(x * BLOCK_SIZE, y * BLOCK_SIZE, sweptImg);
        if (x - 1 >= 0) {
            search(blocks, x - 1, y, sweptImg);
        }
        if (x + 1 < BLOCK_NUM) {
            search(blocks, x + 1, y, sweptImg);
        }
        if (y + 1 < BLOCK_NUM) {
            search(blocks, x, y + 1, sweptImg);
        }
        if (y - 1 >= 0) {
            search(blocks, x, y - 1, sweptImg);
        }
        if (y - 1 >= 0 && x - 1 >= 0) {
            search(blocks, x - 1, y - 1, sweptImg);
        }
        if (y - 1 >= 0 && x + 1 < BLOCK_NUM) {
            search(blocks, x + 1, y - 1, sweptImg);
        }
        if (y + 1 < BLOCK_NUM && x - 1 >= 0) {
            search(blocks, x - 1, y + 1, sweptImg);
        }
        if (y + 1 < BLOCK_NUM && x + 1 < BLOCK_NUM) {
            search(blocks, x + 1, y + 1, sweptImg);
        }
    }
}

int isVictory(Block blocks[][BLOCK_NUM]) {
    int count = 0;
    for (int i = 0; i < BLOCK_NUM; i++) {
        for (int j = 0; j < BLOCK_NUM; j++) {
            if (blocks[i][j].state != SWEPT) {
                count++;
            }
        }
    }
    if (count == NUM) {
        setfontbkcolor(GREEN);
        setcolor(RED);
        setbkmode(OPAQUE);
        int width = BLOCK_SIZE * BLOCK_NUM;
        //字体居中
        xyprintf(width / 2 - BLOCK_SIZE * 3.5, width / 2 - BLOCK_SIZE * 0.5, "VICTORY");
        return 1;
    }
    return 0;
}


void mainloop(Block blocks[][BLOCK_NUM], PIMAGE sweptImg, PIMAGE normalImg) {
    mouse_msg msg = {0};
    for (; is_run(); delay_fps(60)) {
        while (mousemsg()) {
            msg = getmouse();
            int x, y;
            x = msg.x / BLOCK_SIZE;
            y = msg.y / BLOCK_SIZE;
            if (msg.is_left() && blocks[x][y].state != FLAG) {
                if (sweepMine(x, y, blocks)) {
                    search(blocks, x, y, sweptImg);
                } else
                    return;
            } else if (msg.is_right() && msg.is_down()) {
                if (blocks[x][y].state == NORMAL) {
                    setbkmode(TRANSPARENT);
                    setcolor(YELLOW);
                    xyprintf(x * BLOCK_SIZE, y * BLOCK_SIZE, "?");
                    blocks[x][y].state = FLAG;
                } else if (blocks[x][y].state == FLAG) {
                    putimage(x * BLOCK_SIZE, y * BLOCK_SIZE, normalImg);
                    blocks[x][y].state = NORMAL;
                }
            }
        }
        if (isVictory(blocks)) {
            return;
        }
    }
}

int main() {
    PIMAGE normalImg, sweptImg;

    initgraph(BLOCK_NUM * BLOCK_SIZE, BLOCK_NUM * BLOCK_SIZE);
    randomize();//初始化随机数

    Block blocks[BLOCK_NUM][BLOCK_NUM];

    normalImg = newimage();
    sweptImg = newimage();
    //得到img
    setfillcolor(BLUE);
    setcolor(WHITE);
    bar3d(0, 0, BLOCK_SIZE, BLOCK_SIZE, 0, 0, NULL);
    getimage(normalImg, 0, 0, BLOCK_SIZE, BLOCK_SIZE);
    //得到sweptImg
    setfillcolor(EGERGB(0x91, 0x91, 0x91));
    setcolor(EGERGB(0x98, 0xF8, 0x98));
    bar3d(0, 0, BLOCK_SIZE, BLOCK_SIZE, 0, 0, NULL);
    getimage(sweptImg, 0, 0, BLOCK_SIZE, BLOCK_SIZE);

    setfont(BLOCK_SIZE, BLOCK_SIZE, "宋体");
    //初始化界面
    for (int i = 0; i < BLOCK_NUM; i++) {
        for (int j = 0; j < BLOCK_NUM; j++) {
            putimage(i * BLOCK_SIZE, j * BLOCK_SIZE, normalImg);
        }
    }

    initBlocks(blocks);
    //主循环
    mainloop(blocks, sweptImg, normalImg);

    getch();
    delimage(normalImg);
    delimage(sweptImg);
    closegraph();
    return 0;
}
