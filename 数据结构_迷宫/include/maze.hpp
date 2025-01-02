#ifndef _MAZE_HPP_
#define _MAZE_HPP_
#include "base.hpp"
#include <graphics.h>
extern PathType paths[100];
extern int txt_lines;
#endif

void recursion_mgpath(int **mg, Maze_Point index_box, Maze_Point end_box, PathType path, int &count, bool &exist_path); // 递归算法求解从入口到出口的所有迷宫路径
void stack_mgpath(int **mg, Maze_Point index_box, Maze_Point end_box, int &count, bool &exist_path);                    // 栈算法求解从入口到出口的所有迷宫路径
void queue_mgpath(int **mg, Maze_Point index_box, Maze_Point end_box, int &count, bool &exist_path);                    // 队列算法求解从入口到出口的所有迷宫路径
void graph_mgpath(ALGraph *G, int **&visited, Maze_Point index_box, Maze_Point end_box, PathType path, int &count, bool &exist_path);
void format_maze(int **&maze, int **&maze_cache, int **&visited, int &M, int &N, Maze_Point &start, Maze_Point &end, std::string file_path = "");
void display_maze(int **maze_cache, int M, int N, Maze_Point start, Maze_Point end);

