#include "../include/base.hpp"

void InitStack(StType *&s) // ��ʼ��ջ
{
    s = (StType *)malloc(sizeof(StType));
    s->top = -1;
}

void DestroyStack(StType *&s) // ����ջ
{
    free(s);
}

bool StackEmpty(StType *s) // �ж�ջ�Ƿ�Ϊ��
{
    return (s->top == -1);
}

bool Push(StType *&s, Maze_Point e) // ��ջԪ��e
{
    if (s->top == MaxSize - 1)
        return false;
    s->top++;
    s->data[s->top] = e;
    return true;
}

bool Pop(StType *&s, Maze_Point &e) // ��ջԪ��e
{
    if (s->top == -1)
        return false;
    s->top--;
    e = s->data[s->top];
    return true;
}

bool GetTop(StType *s, Maze_Point &e) // ȡջ��Ԫ��e
{
    if (s->top == -1)
        return false;
    e = s->data[s->top];
    return true;
}

void InitQueue(QuType *&q) // ��ʼ������
{
    q = (QuType *)malloc(sizeof(QuType));
    q->front = q->rear = -1;
}

void DestroyQueue(QuType *&q) // ���ٶ���
{
    free(q);
}

bool QueueEmpty(QuType *q) // �ж϶����Ƿ�Ϊ��
{
    return (q->front == q->rear);
}

bool enQueue(QuType *&q, Maze_Point e) // ������
{
    if (q->rear == MaxSize - 1) // ���������
        return false;           // ���ؼ�
    q->rear++;                  // ��β��1
    q->data[q->rear] = e;       // rearλ�ò���Ԫ��e
    return true;                // ������
}

bool deQueue(QuType *&q, Maze_Point &e) // ������
{
    if (q->front == q->rear) // �ӿ������
        return false;
    q->front++;
    e = q->data[q->front];
    return true;
}

void CreateAdj(ALGraph *&G, int **mg, int M, int N) // �����Թ������Ӧ���ڽӱ�G
{
    int i, j, i1, j1, di;
    ArcNode *p;
    G = (ALGraph *)malloc(sizeof(ALGraph));
    G->adjlist = (VNode **)malloc((M + 2) * sizeof(VNode *));
    for (i = 0; i < M + 2; i++)
    {
        G->adjlist[i] = (VNode *)malloc((N + 2) * sizeof(VNode)); // ����ÿ�е����ڴ�
    }
    for (i = 0; i < M + 2; i++) // ���ڽӱ�������ͷ�ڵ��ָ�����ó�ֵ
    {
        for (j = 0; j < N + 2; j++)
        {
            G->adjlist[i][j].firstarc = NULL;
        }
    }
    for (i = 1; i <= M; i++) // ���mg��ÿ��Ԫ��
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
                    if (mg[i1][j1] == 0) //(i1,j1)Ϊ���߷���
                    {
                        p = (ArcNode *)malloc(sizeof(ArcNode)); // ����һ���ڵ�*p
                        p->i = i1;
                        p->j = j1;
                        p->nextarc = G->adjlist[i][j].firstarc; // ��*p�ڵ����������
                        G->adjlist[i][j].firstarc = p;
                    }
                    di++;
                }
            }
        }
    }
}

void DispAdj(ALGraph *G, int M, int N) // ����ڽӱ�G
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

void DestroyAdj(ALGraph *&G, int M, int N) // �����ڽӱ�
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
