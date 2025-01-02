#include "../include/base.hpp"

void InitStack(StType *&s) // 初始化栈
{
    s = (StType *)malloc(sizeof(StType));
    s->top = -1;
}

void DestroyStack(StType *&s) // 销毁栈
{
    free(s);
}

bool StackEmpty(StType *s) // 判断栈是否为空
{
    return (s->top == -1);
}

bool Push(StType *&s, Maze_Point e) // 进栈元素e
{
    if (s->top == MaxSize - 1)
        return false;
    s->top++;
    s->data[s->top] = e;
    return true;
}

bool Pop(StType *&s, Maze_Point &e) // 出栈元素e
{
    if (s->top == -1)
        return false;
    s->top--;
    e = s->data[s->top];
    return true;
}

bool GetTop(StType *s, Maze_Point &e) // 取栈顶元素e
{
    if (s->top == -1)
        return false;
    e = s->data[s->top];
    return true;
}

void InitQueue(QuType *&q) // 初始化队列
{
    q = (QuType *)malloc(sizeof(QuType));
    q->front = q->rear = -1;
}

void DestroyQueue(QuType *&q) // 销毁队列
{
    free(q);
}

bool QueueEmpty(QuType *q) // 判断队列是否为空
{
    return (q->front == q->rear);
}

bool enQueue(QuType *&q, Maze_Point e) // 进队列
{
    if (q->rear == MaxSize - 1) // 队满上溢出
        return false;           // 返回假
    q->rear++;                  // 队尾增1
    q->data[q->rear] = e;       // rear位置插入元素e
    return true;                // 返回真
}

bool deQueue(QuType *&q, Maze_Point &e) // 出队列
{
    if (q->front == q->rear) // 队空下溢出
        return false;
    q->front++;
    e = q->data[q->front];
    return true;
}

void CreateAdj(ALGraph *&G, int **mg, int M, int N) // 建立迷宫数组对应的邻接表G
{
    int i, j, i1, j1, di;
    ArcNode *p;
    G = (ALGraph *)malloc(sizeof(ALGraph));
    G->adjlist = (VNode **)malloc((M + 2) * sizeof(VNode *));
    for (i = 0; i < M + 2; i++)
    {
        G->adjlist[i] = (VNode *)malloc((N + 2) * sizeof(VNode)); // 分配每行的列内存
    }
    for (i = 0; i < M + 2; i++) // 给邻接表中所有头节点的指针域置初值
    {
        for (j = 0; j < N + 2; j++)
        {
            G->adjlist[i][j].firstarc = NULL;
        }
    }
    for (i = 1; i <= M; i++) // 检查mg中每个元素
    {
        for (j = 1; j <= N; j++)
        {
            if (mg[i][j] == 0)
            {
                di = 0;
                while (di < 4)
                {
                    switch (di)
                    {
                    case 0:
                        i1 = i - 1;
                        j1 = j;
                        break;
                    case 1:
                        i1 = i;
                        j1 = j + 1;
                        break;
                    case 2:
                        i1 = i + 1;
                        j1 = j;
                        break;
                    case 3:
                        i1 = i;
                        j1 = j - 1;
                        break;
                    }
                    if (mg[i1][j1] == 0) //(i1,j1)为可走方块
                    {
                        p = (ArcNode *)malloc(sizeof(ArcNode)); // 创建一个节点*p
                        p->i = i1;
                        p->j = j1;
                        p->nextarc = G->adjlist[i][j].firstarc; // 将*p节点链到链表后
                        G->adjlist[i][j].firstarc = p;
                    }
                    di++;
                }
            }
        }
    }
}

void DispAdj(ALGraph *G, int M, int N) // 输出邻接表G
{
    int i, j;
    ArcNode *p;
    for (i = 0; i < M + 2; i++)
        for (j = 0; j < N + 2; j++)
        {
            printf("  [%d,%d]: ", i, j);
            p = G->adjlist[i][j].firstarc;
            while (p != NULL)
            {
                printf("(%d,%d)  ", p->i, p->j);
                p = p->nextarc;
            }
            printf("\n");
        }
}

void DestroyAdj(ALGraph *&G, int M, int N) // 销毁邻接表
{
    int i, j;
    ArcNode *pre, *p;
    for (i = 0; i < M + 2; i++)
    {
        for (j = 0; j < N + 2; j++)
        {
            pre = G->adjlist[i][j].firstarc;
            if (pre != NULL)
            {
                p = pre->nextarc;
                while (p != NULL)
                {
                    free(pre);
                    pre = p;
                    p = p->nextarc;
                }
                free(pre);
            }
        }
        free(G->adjlist[i]);
    }
    free(G->adjlist);
    free(G);
    G = NULL;
}
