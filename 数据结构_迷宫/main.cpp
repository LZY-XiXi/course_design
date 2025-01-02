#include "include/draw.hpp"
#include <chrono>
#include <unistd.h>

int main()
{
    HWND hwnd;                                                 // 窗口话柄
    MOUSEMSG m;                                                // 鼠标消息
    int M = 0, N = 0, count = 0;                               // 表示迷宫为M*N矩阵,存放迷宫路径的条数
    int **maze, **maze_cache, **visited;                       // 迷宫和迷宫缓存
    ALGraph *G;                                                // 迷宫图
    Maze_Point start, end;                                     // 迷宫起点终点
    PathType path;                                             // 迷宫路径缓存
    bool exist_path = false, is_input = false, is_use = false; // 判断是否存在路径,是否已经输入迷宫信息,迷宫是否使用过
    std::string input_txt_path = "txt\\exp5_9_input.txt";      // 文件路径
    std::string output_txt_path = "txt\\exp5_9_output.txt";    // 文件路径

    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
    double duration_second;

    std::ofstream out;
    out.open(output_txt_path);

    hwnd = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOWCONSOLE);                                           // 初始化窗口
    setbkcolor(RGB(125, 200, 200));                                                                       // 设置背景色
    SetWindowPos(hwnd, HWND_NOTOPMOST, WINDOW_LEFT, WINDOW_TOP, WINDOW_WIDTH, WINDOW_HEIGHT, SWP_NOSIZE); // 设置窗口属性
    SetWindowText(hwnd, "迷宫问题");                                                                      // 设置窗口标题

    draw_select_input();
    while (true)
    {
        m = GetMouseMsg();

        if (m.x >= 295 && m.x <= 705 && m.y >= 245 && m.y <= 305) // 手动输入
        {
            BeginBatchDraw();
            setlinecolor(RED);
            rectangle(295, 245, 705, 305);
            EndBatchDraw();

            if (m.uMsg == WM_LBUTTONDOWN)
            {
                draw_format_maze(maze, maze_cache, visited, M, N, start, end);
                is_input = true;
            }
        }
        else if (m.x >= 295 && m.x <= 705 && m.y >= 345 && m.y <= 405) // 文件导入
        {
            BeginBatchDraw();
            setlinecolor(RED);
            rectangle(295, 345, 705, 405);
            EndBatchDraw();

            if (m.uMsg == WM_LBUTTONDOWN)
            {
                format_maze(maze, maze_cache, visited, M, N, start, end, input_txt_path);
                is_input = true;
            }
        }
        else if (m.x >= 295 && m.x <= 705 && m.y >= 445 && m.y <= 505) // 退出
        {
            BeginBatchDraw();
            setlinecolor(RED);
            rectangle(295, 445, 705, 505);
            EndBatchDraw();

            if (m.uMsg == WM_LBUTTONDOWN)
            {
                break;
            }
        }
        else
        {
            BeginBatchDraw();
            setlinecolor(RGB(125, 200, 200));
            rectangle(295, 245, 705, 305);
            rectangle(295, 345, 705, 405);
            rectangle(295, 445, 705, 505);
            EndBatchDraw();
        }

        if (is_input)
        {
            display_maze(maze_cache, M, N, start, end);
            out << "迷宫如下：\n";
            for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    out << maze[i][j] << " ";
                }
                out << "\n";
            }
            out << "起点为: (" << start.x << "," << start.y << ")\n";
            out << "终点为: (" << end.x << "," << end.y << ")\n\n";

            draw_select_mgpth();
            while (true)
            {
                m = GetMouseMsg();
                if (m.x >= 295 && m.x <= 705 && m.y >= 95 && m.y <= 155) // 递归求所有路径
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 95, 705, 155);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        out << "使用递归求迷宫所有路径：\n";
                        startTime = std::chrono::steady_clock::now();
                        recursion_mgpath(maze_cache, start, end, path, count, exist_path);
                        endTime = std::chrono::steady_clock::now();
                        duration_second = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                        printf("计算消耗时间：%fs", duration_second);
                        out << "计算消耗时间: " << duration_second << "s\n";

                        is_use = true;
                    }
                }

                else if (m.x >= 295 && m.x <= 705 && m.y >= 195 && m.y <= 255) // 栈求所有路径
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 195, 705, 255);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        out << "使用栈求迷宫所有路径：\n";
                        startTime = std::chrono::steady_clock::now();
                        stack_mgpath(maze_cache, start, end, count, exist_path);
                        endTime = std::chrono::steady_clock::now();
                        duration_second = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                        printf("计算消耗时间：%fs", duration_second);
                        out << "计算消耗时间: " << duration_second << "s\n";
                        is_use = true;
                    }
                }

                else if (m.x >= 295 && m.x <= 705 && m.y >= 295 && m.y <= 355) // 队列求最短路径
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 295, 705, 355);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        out << "使用队列求迷宫最短路径：\n";
                        startTime = std::chrono::steady_clock::now();
                        queue_mgpath(maze_cache, start, end, count, exist_path);
                        endTime = std::chrono::steady_clock::now();
                        duration_second = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                        printf("计算消耗时间：%fs", duration_second);
                        out << "计算消耗时间: " << duration_second << "s\n";
                        is_use = true;
                    }
                }

                else if (m.x >= 295 && m.x <= 705 && m.y >= 395 && m.y <= 455) // 递归(图)求所有路径
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 395, 705, 455);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {

                        out << "使用递归（图）求迷宫所有路径：\n";
                        startTime = std::chrono::steady_clock::now();
                        CreateAdj(G, maze_cache, M, N);
                        graph_mgpath(G, visited, start, end, path, count, exist_path);
                        DestroyAdj(G, M, N);
                        endTime = std::chrono::steady_clock::now();
                        duration_second = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                        printf("计算消耗时间：%fs", duration_second);
                        out << "计算消耗时间: " << duration_second << "s\n";
                        is_use = true;
                    }
                }

                else if (m.x >= 295 && m.x <= 705 && m.y >= 495 && m.y <= 555) // 返回
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 495, 705, 555);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        break;
                    }
                }

                else
                {
                    BeginBatchDraw();
                    setlinecolor(RGB(125, 200, 200));
                    rectangle(295, 95, 705, 155);
                    rectangle(295, 195, 705, 255);
                    rectangle(295, 295, 705, 355);
                    rectangle(295, 395, 705, 455);
                    rectangle(295, 495, 705, 555);
                    EndBatchDraw();
                }

                if (is_use)
                {
                    if (!exist_path)
                    {
                        cleardevice();
                        settextstyle(45, 0, "楷体");
                        setbkmode(TRANSPARENT);
                        outtextxy(350, 300, "未找到迷宫的解");
                        sleep(3);
                        draw_select_mgpth();
                        out << "该迷宫未找到路径\n";
                    }

                    else
                    {
                        out << "该方法求得迷宫路径个数为：" << count << "\n";
                        for (int i = 0; i < count; i++)
                        {
                            show_mgpath(maze, M, N, start, end, paths[i]);
                            out << "第" << i + 1 << "条迷宫路径如下: \n";
                            for (int j = 0; j < paths[i].length; j++)
                            {
                                out << "(" << paths[i].data[j].x << "," << paths[i].data[j].y << ")-->";
                            }
                            out << "final\n";
                        }
                    }

                    for (int i = 0; i < M; i++)
                    {
                        for (int j = 0; j < N; j++)
                        {
                            maze_cache[i][j] = maze[i][j];
                            visited[i][j] = 0;
                        }
                    }
                    is_use = false;
                    path.length = 0;
                    count = 0;
                    printf("\n\n");
                    out << "\n";
                    exist_path = false;
                    draw_select_mgpth();
                }
            }

            for (int i = 0; i < M + 5; i++)
            {
                delete[] maze[i];
                delete[] visited[i];
                delete[] maze_cache[i];
            }
            delete[] maze;
            delete[] visited;
            delete[] maze_cache;
            is_input = false;

            draw_select_input();
            system("cls");
        }
    }
    out.close();
    closegraph();
    system("exit");
    return 0;
}