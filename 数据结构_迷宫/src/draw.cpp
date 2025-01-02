#include "../include/draw.hpp"

void show_mgpath(int **mg, int M, int N, Maze_Point index_box, Maze_Point end_box, PathType path)
{
    BeginBatchDraw(); // 开始绘图
    int box_length = (WINDOW_WIDTH + WINDOW_HEIGHT) / (M * N);
    for (int index = 0; index < path.length; index++)
    {
        setfillcolor(WHITE);
        solidrectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        for (int i = 0; i < M + 3; i++)
        {
            for (int j = 0; j < N + 3; j++)
            {
                if (mg[i][j] == 1)
                {
                    setfillcolor(BLACK); // 绘制墙壁
                    solidrectangle(i * box_length, j * box_length, (i + 1) * box_length, (j + 1) * box_length);
                }
                if (i == end_box.x && j == end_box.y)
                {
                    setfillcolor(GREEN); // 绘制终点
                    solidrectangle(i * box_length, j * box_length, (i + 1) * box_length, (j + 1) * box_length);
                }
                if (i == path.data[index].x && j == path.data[index].y)
                {
                    setfillcolor(RED); // 绘制人物
                    solidrectangle(i * box_length, j * box_length, (i + 1) * box_length, (j + 1) * box_length);
                }
            }
        }
        FlushBatchDraw();
        Sleep(500);
    }
    EndBatchDraw();
}

void draw_select_input()
{
    cleardevice();

    BeginBatchDraw();

    IMAGE picture;
    loadimage(&picture, "C:/Users/Lin/Desktop/data_project/img/group.png", 100, 100);
    putimage(15, 15, &picture);

    setfillcolor(RGB(0, 0, 0));
    fillrectangle(300, 250, 700, 300);
    fillrectangle(300, 350, 700, 400);
    fillrectangle(300, 450, 700, 500);

    settextstyle(35, 0, "楷体");
    setbkmode(TRANSPARENT);
    outtextxy(435, 255, "手动输入");
    outtextxy(435, 355, "文件输入");
    outtextxy(465, 460, "退出");

    settextstyle(35, 0, "楷体");
    outtextxy(800, 550, "by 林展羽");

    EndBatchDraw();
}
void draw_select_mgpth()
{
    cleardevice();

    BeginBatchDraw();
    setfillcolor(RGB(0, 0, 0));
    fillrectangle(300, 100, 700, 150);
    fillrectangle(300, 200, 700, 250);
    fillrectangle(300, 300, 700, 350);
    fillrectangle(300, 400, 700, 450);
    fillrectangle(300, 500, 700, 550);

    settextstyle(35, 0, "楷体");
    setbkmode(TRANSPARENT);
    outtextxy(380, 105, "递归求所有路径");
    outtextxy(390, 205, "栈求所有路径");
    outtextxy(370, 305, "队列求最短路径");
    outtextxy(350, 405, "递归(图)求所有路径");
    outtextxy(460, 505, "返回");

    EndBatchDraw();
}

void draw_format_maze(int **&maze, int **&maze_cache, int **&visited, int &M, int &N, Maze_Point &start, Maze_Point &end)
{
    std::string cache;
    do
    {
        char input_maze_size[10];
        InputBox(input_maze_size, 10, "输入整数M和N, 用空格隔开, \n代表迷宫行数和列数", "输入M和N", NULL, 200, 100);
        cache = input_maze_size;
        sscanf(cache.c_str(), "%d%d", &M, &N);
        if (M == 0 || N == 0)
        {
            MessageBox(NULL, "该数据无效, 行数M和列数N必须不为0", "警告", MB_OK);
        }
    } while (M == 0 || N == 0);

    maze = new int *[M + 3];
    maze_cache = new int *[M + 3];
    visited = new int *[M + 3];
    for (int i = 0; i < M + 3; i++)
    {
        maze[i] = new int[N + 3];
        maze_cache[i] = new int[N + 3];
        visited[i] = new int[N + 3];
    }
    char input_maze_message[500];
    InputBox(input_maze_message, 500, "输入你的迷宫, 用空格隔开\n其中0表示通路,1表示墙壁:", "输入迷宫", NULL, 300, 300);
    cache = input_maze_message;
    std::istringstream maze_message(cache);
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            maze_message >> maze[i][j]; // 输入0，表示为通路；1表示墙
            maze_cache[i][j] = maze[i][j];
            visited[i][j] = 0;
        }
    }

    do
    {
        char input_start[10];
        InputBox(input_start, 10, "输入起点, 用空格隔开x与y:", "输入起点", NULL, 200, 100);
        cache = input_start;
        sscanf(cache.c_str(), "%d%d", &start.x, &start.y);

        if (maze[start.x][start.y] != 0)
        {
            MessageBox(NULL, "警告!该数据无效, 起始点为墙壁", "警告", MB_OK);
        }

    } while (maze[start.x][start.y] != 0);
    do
    {
        char input_end[10];
        InputBox(input_end, 10, "输入终点, 用空格隔开x与y:", "输入终点", NULL, 200, 100);
        cache = input_end;
        sscanf(cache.c_str(), "%d%d", &end.x, &end.y);

        if (maze[end.x][end.y] != 0)
        {
            MessageBox(NULL, "警告!该数据无效, 终点为墙壁", "警告", MB_OK);
        }

    } while (maze[end.x][end.y] != 0);
}