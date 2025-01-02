#ifndef _BASE_HPP_
#define _BASE_HPP_
#include <iostream>
#include <fstream>
#include <sstream>
#define MaxSize 300
#endif

typedef struct
{
    int x;      // ��ǰ������к�
    int y;      // ��ǰ������к�
    int di = 0; // ��һ�������ڷ�λ�ķ�λ��
} Maze_Point;

typedef struct
{
    Maze_Point data[MaxSize];
    int length = 0; // ·������
} PathType;         // ����·������

typedef struct
{
    Maze_Point data[MaxSize]; // ��ŷ���
    int top;                  // ջ��ָ��
} StType;                     // ����ջ����

typedef struct
{
    Maze_Point data[MaxSize];
    int front = 0, rear = 0; // ��ͷָ��Ͷ�βָ��
} QuType;                    // ˳�������

typedef struct ANode // �ߵĽڵ�ṹ����
{
    int i, j;              // �ñߵ��յ�λ��(i,j)
    struct ANode *nextarc; // ָ����һ���ߵ�ָ��
} ArcNode;

typedef struct Vnode // �ڽӱ�ͷ�ڵ������
{
    ArcNode *firstarc; // ָ���һ�����ڵ�
} VNode;

typedef struct
{
    VNode **adjlist; // �ڽӱ�ͷ�ڵ�����
} ALGraph;           // ͼ���ڽӱ�����

void InitStack(StType *&s);            // ��ʼ��ջ
void DestroyStack(StType *&s);         // ����ջ
bool StackEmpty(StType *s);            // �ж�ջ�Ƿ�Ϊ��
bool Push(StType *&s, Maze_Point e);   // ��ջԪ��e
bool Pop(StType *&s, Maze_Point &e);   // ��ջԪ��e
bool GetTop(StType *s, Maze_Point &e); // ȡջ��Ԫ��e

void InitQueue(QuType *&q);              // ��ʼ������
void DestroyQueue(QuType *&q);           // ���ٶ���
bool QueueEmpty(QuType *q);              // �ж϶����Ƿ�Ϊ��
bool enQueue(QuType *&q, Maze_Point e);  // ������
bool deQueue(QuType *&q, Maze_Point &e); // ������

void CreateAdj(ALGraph *&G, int **mg, int M, int N); // �����Թ������Ӧ���ڽӱ�G
void DispAdj(ALGraph *G, int M, int N);              // ����ڽӱ�G
void DestroyAdj(ALGraph *&G, int M, int N);          // �����ڽӱ�
