#ifndef _DRAW_HPP_
#define _DRAW_HPP_
#include "../include/maze.hpp"
#include <graphics.h>
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define WINDOW_LEFT 230
#define WINDOW_TOP 20
#define BUFFERSIZE 1024
#endif

void draw_select_input();
void draw_select_mgpth();
void show_mgpath(int **mg, int M, int N, Maze_Point index_box, Maze_Point end_box, PathType path);
void draw_format_maze(int **&maze, int **&maze_cache, int **&visited, int &M, int &N, Maze_Point &start, Maze_Point &end);
