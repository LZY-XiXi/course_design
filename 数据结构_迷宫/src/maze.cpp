#include "../include/maze.hpp"
PathType paths[100];
int txt_lines = 0;

void recursion_mgpath(int **mg, Maze_Point index_box, Maze_Point end_box, PathType path, int &count, bool &exist_path)
{
    Maze_Point e;
    if (index_box.x == end_box.x && index_box.y == end_box.y)
    {
        exist_path = true;
        paths[count].length = 0;

        path.data[path.length].x = index_box.x;
        path.data[path.length].y = index_box.y;
        path.length++;

        printf("第%d条迷宫路径如下:\n", count + 1);
        for (int k = 0; k < path.length; k++)
        {
            printf("\t(%d,%d)", path.data[k].x, path.data[k].y);
            paths[count].data[paths[count].length++] = path.data[k];
        }
        count++;
        printf("\n\n");
    }
    else
    {
        if (mg[index_box.x][index_box.y] == 0)
        {
            e.di = 0;
            while (e.di < 4)
            {
                switch (e.di)
                {
                case 0:
                    e.x = index_box.x - 1;
                    e.y = index_box.y;
                    break;
                case 1:
                    e.x = index_box.x;
                    e.y = index_box.y + 1;
                    break;
                case 2:
                    e.x = index_box.x + 1;
                    e.y = index_box.y;
                    break;
                case 3:
                    e.x = index_box.x;
                    e.y = index_box.y - 1;
                    break;
                }
                path.data[path.length].x = index_box.x;
                path.data[path.length].y = index_box.y;
                path.length++;

                mg[index_box.x][index_box.y] = -1;
                recursion_mgpath(mg, Maze_Point{e.x, e.y}, Maze_Point{end_box.x, end_box.y}, path, count, exist_path);
                path.length--;
                mg[index_box.x][index_box.y] = 0;
                e.di++;
            }
        }
    }
}

void stack_mgpath(int **mg, Maze_Point index_box, Maze_Point end_box, int &count, bool &exist_path)
{
    bool find;
    Maze_Point next_box;
    StType *st;
    InitStack(st);
    index_box.di = -1;
    Push(st, index_box);

    mg[index_box.x][index_box.y] = -1;
    while (!StackEmpty(st))
    {
        GetTop(st, next_box);
        if (next_box.x == end_box.x && next_box.y == end_box.y)
        {
            paths[count].length = 0;
            printf("第%d条迷宫路径如下:\n", count + 1);
            for (int k = 0; k <= st->top; k++)
            {
                printf("\t(%d,%d) ", st->data[k].x, st->data[k].y);
                paths[count].data[k] = st->data[k];
                paths[count].length++;
            }

            printf("\n\n");
            mg[st->data[st->top].x][st->data[st->top].y] = 0;
            Pop(st, next_box);
            exist_path = true;
            count++;
        }
        find = false;

        while (next_box.di < 4 && !find)
        {
            next_box.di++;
            switch (next_box.di)
            {

            case 0:
                next_box.x = st->data[st->top].x - 1;
                next_box.y = st->data[st->top].y;
                break;

            case 1:
                next_box.x = st->data[st->top].x;
                next_box.y = st->data[st->top].y + 1;
                break;

            case 2:
                next_box.x = st->data[st->top].x + 1;
                next_box.y = st->data[st->top].y;
                break;

            case 3:
                next_box.x = st->data[st->top].x;
                next_box.y = st->data[st->top].y - 1;
                break;
            }

            if (mg[next_box.x][next_box.y] == 0)
            {
                find = true;
            }
        }

        if (find)
        {
            st->data[st->top].di = next_box.di;
            next_box.di = -1;
            Push(st, next_box);
            mg[next_box.x][next_box.y] = -1;
        }
        else
        {
            mg[st->data[st->top].x][st->data[st->top].y] = 0;
            Pop(st, next_box);
        }
    }
    DestroyStack(st);
}

void queue_mgpath(int **mg, Maze_Point index_box, Maze_Point end_box, int &count, bool &exist_path)
{
    Maze_Point next_box;
    QuType *qu;    // 定义顺序队指针qu
    InitQueue(qu); // 初始化队列qu
    next_box = index_box;
    next_box.di = -1;
    enQueue(qu, next_box);
    mg[index_box.x][index_box.y] = -1; // 将其赋值-1,以避免回过来重复搜索
    while (!QueueEmpty(qu))            // 队不空且循环
    {
        deQueue(qu, next_box); // 出队方块e,由于不是环形队列，该出队元素仍在队列中
        index_box.x = next_box.x;
        index_box.y = next_box.y;
        if (index_box.x == end_box.x && index_box.y == end_box.y) // 找到了出口,输出路径
        {
            exist_path = true;
            paths[count].length = 0;
            printf("最短迷宫路径如下:\n");
            int k = qu->front, j;
            do // 反向找到最短路径,将该路径上的方块的pre成员设置成-1
            {
                j = k;
                k = qu->data[k].di;
                qu->data[j].di = -1;
            } while (k > 0 && k < MaxSize);
            k = 0;
            while (k < MaxSize) // 正向搜索到pre为-1的方块,即构成正向的路径
            {
                if (qu->data[k].di == -1)
                {
                    printf("\t(%d,%d)", qu->data[k].x, qu->data[k].y);
                    paths[count].data[paths[count].length++] = qu->data[k];
                }
                k++;
            }
            printf("\n\n");
            count++;
        }
        for (index_box.di = 0; index_box.di < 4; index_box.di++) // 循环扫描每个方位,把每个可走的方块插入队列中
        {
            switch (index_box.di)
            {
            case 0:
                next_box.x = index_box.x - 1;
                next_box.y = index_box.y;
                break;
            case 1:
                next_box.x = index_box.x;
                next_box.y = index_box.y + 1;
                break;
            case 2:
                next_box.x = index_box.x + 1;
                next_box.y = index_box.y;
                break;
            case 3:
                next_box.x = index_box.x;
                next_box.y = index_box.y - 1;
                break;
            }
            if (mg[next_box.x][next_box.y] == 0)
            {
                next_box.di = qu->front;         // 指向路径中上一个方块的下标
                enQueue(qu, next_box);           //(i1,j1)方块进队
                mg[next_box.x][next_box.y] = -1; // 将其赋值-1,以避免回过来重复搜索
            }
        }
    }
    DestroyQueue(qu); // 销毁队列
}

void graph_mgpath(ALGraph *G, int **&visited, Maze_Point index_box, Maze_Point end_box, PathType path, int &count, bool &exist_path)
{
    ArcNode *p;
    visited[index_box.x][index_box.y] = 1; // 置已访问标记
    path.data[path.length] = index_box;
    path.length++;
    if (index_box.x == end_box.x && index_box.y == end_box.y)
    {
        exist_path = true;
        paths[count].length = 0;

        printf("第%d条迷宫路径如下:\n", count + 1);
        for (int k = 0; k < path.length; k++)
        {
            printf("\t(%d,%d) ", path.data[k].x, path.data[k].y);
            paths[count].data[paths[count].length++] = path.data[k];
        }
        count++;
        printf("\n\n");
    }
    p = G->adjlist[index_box.x][index_box.y].firstarc; // p指向顶点v的第一条边顶点
    while (p != NULL)
    {
        if (visited[p->i][p->j] == 0) // 若(p->i,p->j)方块未访问,递归访问它
        {
            graph_mgpath(G, visited, Maze_Point{p->i, p->j}, Maze_Point{end_box.x, end_box.y}, path, count, exist_path);
        }
        p = p->nextarc; // p指向顶点v的下一条边顶点
    }
    visited[index_box.x][index_box.y] = 0;
}

void format_maze(int **&maze, int **&maze_cache, int **&visited, int &M, int &N, Maze_Point &start, Maze_Point &end, std::string input_txt_path)
{
    std::ifstream in;
    in.open(input_txt_path);
    for (int i = 0; i < txt_lines; i++)
    {
        std::getline(in, input_txt_path);
    }
    in >> M >> N;
    txt_lines = txt_lines + M + 3;
    maze = new int *[M + 3];
    maze_cache = new int *[M + 3];
    visited = new int *[M + 3];
    for (int i = 0; i < M + 3; i++)
    {
        maze[i] = new int[N + 3];
        maze_cache[i] = new int[N + 3];
        visited[i] = new int[N + 3];
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            in >> maze[i][j]; // 输入0，表示为通路；1表示墙
            maze_cache[i][j] = maze[i][j];
            visited[i][j] = 0;
        }
        printf("\n");
    }
    in >> start.x >> start.y >> end.x >> end.y;
    in.close();
}

void display_maze(int **maze_cache, int M, int N, Maze_Point start, Maze_Point end)
{
    system("cls");
    printf("本次迷宫大小为%dx%d\n", M, N);
    printf("迷宫如下:\n");
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", maze_cache[i][j]);
        }
        printf("\n");
    }
    printf("迷宫起点为(%d,%d) 终点为(%d,%d)\n\n", start.x, start.y, end.x, end.y);
}
