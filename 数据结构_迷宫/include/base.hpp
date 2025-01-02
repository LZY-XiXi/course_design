#ifndef _BASE_HPP_
#define _BASE_HPP_
#include <iostream>
#include <fstream>
#include <sstream>
#define MaxSize 300
#endif

typedef struct
{
    int x;      // 当前方块的行号
    int y;      // 当前方块的列号
    int di = 0; // 下一可走相邻方位的方位号
} Maze_Point;

typedef struct
{
    Maze_Point data[MaxSize];
    int length = 0; // 路径长度
} PathType;         // 定义路径类型

typedef struct
{
    Maze_Point data[MaxSize]; // 存放方块
    int top;                  // 栈顶指针
} StType;                     // 定义栈类型

typedef struct
{
    Maze_Point data[MaxSize];
    int front = 0, rear = 0; // 队头指针和队尾指针
} QuType;                    // 顺序队类型

typedef struct ANode // 边的节点结构类型
{
    int i, j;              // 该边的终点位置(i,j)
    struct ANode *nextarc; // 指向下一条边的指针
} ArcNode;

typedef struct Vnode // 邻接表头节点的类型
{
    ArcNode *firstarc; // 指向第一个相邻点
} VNode;

typedef struct
{
    VNode **adjlist; // 邻接表头节点数组
} ALGraph;           // 图的邻接表类型

void InitStack(StType *&s);            // 初始化栈
void DestroyStack(StType *&s);         // 销毁栈
bool StackEmpty(StType *s);            // 判断栈是否为空
bool Push(StType *&s, Maze_Point e);   // 进栈元素e
bool Pop(StType *&s, Maze_Point &e);   // 出栈元素e
bool GetTop(StType *s, Maze_Point &e); // 取栈顶元素e

void InitQueue(QuType *&q);              // 初始化队列
void DestroyQueue(QuType *&q);           // 销毁队列
bool QueueEmpty(QuType *q);              // 判断队列是否为空
bool enQueue(QuType *&q, Maze_Point e);  // 进队列
bool deQueue(QuType *&q, Maze_Point &e); // 出队列

void CreateAdj(ALGraph *&G, int **mg, int M, int N); // 建立迷宫数组对应的邻接表G
void DispAdj(ALGraph *G, int M, int N);              // 输出邻接表G
void DestroyAdj(ALGraph *&G, int M, int N);          // 销毁邻接表
