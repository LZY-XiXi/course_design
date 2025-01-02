#include "../include/draw.hpp"

void show_mgpath(int **mg, int M, int N, Maze_Point index_box, Maze_Point end_box, PathType path)
{
    BeginBatchDraw(); // ��ʼ��ͼ
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
                    setfillcolor(BLACK); // ����ǽ��
                    solidrectangle(i * box_length, j * box_length, (i + 1) * box_length, (j + 1) * box_length);
                }
                if (i == end_box.x && j == end_box.y)
                {
                    setfillcolor(GREEN); // �����յ�
                    solidrectangle(i * box_length, j * box_length, (i + 1) * box_length, (j + 1) * box_length);
                }
                if (i == path.data[index].x && j == path.data[index].y)
                {
                    setfillcolor(RED); // ��������
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

    settextstyle(35, 0, "����");
    setbkmode(TRANSPARENT);
    outtextxy(435, 255, "�ֶ�����");
    outtextxy(435, 355, "�ļ�����");
    outtextxy(465, 460, "�˳�");

    settextstyle(35, 0, "����");
    outtextxy(800, 550, "by ��չ��");

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

    settextstyle(35, 0, "����");
    setbkmode(TRANSPARENT);
    outtextxy(380, 105, "�ݹ�������·��");
    outtextxy(390, 205, "ջ������·��");
    outtextxy(370, 305, "���������·��");
    outtextxy(350, 405, "�ݹ�(ͼ)������·��");
    outtextxy(460, 505, "����");

    EndBatchDraw();
}

void draw_format_maze(int **&maze, int **&maze_cache, int **&visited, int &M, int &N, Maze_Point &start, Maze_Point &end)
{
    std::string cache;
    do
    {
        char input_maze_size[10];
        InputBox(input_maze_size, 10, "��������M��N, �ÿո����, \n�����Թ�����������", "����M��N", NULL, 200, 100);
        cache = input_maze_size;
        sscanf(cache.c_str(), "%d%d", &M, &N);
        if (M == 0 || N == 0)
        {
            MessageBox(NULL, "��������Ч, ����M������N���벻Ϊ0", "����", MB_OK);
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
    InputBox(input_maze_message, 500, "��������Թ�, �ÿո����\n����0��ʾͨ·,1��ʾǽ��:", "�����Թ�", NULL, 300, 300);
    cache = input_maze_message;
    std::istringstream maze_message(cache);
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            maze_message >> maze[i][j]; // ����0����ʾΪͨ·��1��ʾǽ
            maze_cache[i][j] = maze[i][j];
            visited[i][j] = 0;
        }
    }

    do
    {
        char input_start[10];
        InputBox(input_start, 10, "�������, �ÿո����x��y:", "�������", NULL, 200, 100);
        cache = input_start;
        sscanf(cache.c_str(), "%d%d", &start.x, &start.y);

        if (maze[start.x][start.y] != 0)
        {
            MessageBox(NULL, "����!��������Ч, ��ʼ��Ϊǽ��", "����", MB_OK);
        }

    } while (maze[start.x][start.y] != 0);
    do
    {
        char input_end[10];
        InputBox(input_end, 10, "�����յ�, �ÿո����x��y:", "�����յ�", NULL, 200, 100);
        cache = input_end;
        sscanf(cache.c_str(), "%d%d", &end.x, &end.y);

        if (maze[end.x][end.y] != 0)
        {
            MessageBox(NULL, "����!��������Ч, �յ�Ϊǽ��", "����", MB_OK);
        }

    } while (maze[end.x][end.y] != 0);
}