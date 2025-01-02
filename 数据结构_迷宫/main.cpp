#include "include/draw.hpp"
#include <chrono>
#include <unistd.h>

int main()
{
    HWND hwnd;                                                 // ���ڻ���
    MOUSEMSG m;                                                // �����Ϣ
    int M = 0, N = 0, count = 0;                               // ��ʾ�Թ�ΪM*N����,����Թ�·��������
    int **maze, **maze_cache, **visited;                       // �Թ����Թ�����
    ALGraph *G;                                                // �Թ�ͼ
    Maze_Point start, end;                                     // �Թ�����յ�
    PathType path;                                             // �Թ�·������
    bool exist_path = false, is_input = false, is_use = false; // �ж��Ƿ����·��,�Ƿ��Ѿ������Թ���Ϣ,�Թ��Ƿ�ʹ�ù�
    std::string input_txt_path = "txt\\exp5_9_input.txt";      // �ļ�·��
    std::string output_txt_path = "txt\\exp5_9_output.txt";    // �ļ�·��

    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
    double duration_second;

    std::ofstream out;
    out.open(output_txt_path);

    hwnd = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOWCONSOLE);                                           // ��ʼ������
    setbkcolor(RGB(125, 200, 200));                                                                       // ���ñ���ɫ
    SetWindowPos(hwnd, HWND_NOTOPMOST, WINDOW_LEFT, WINDOW_TOP, WINDOW_WIDTH, WINDOW_HEIGHT, SWP_NOSIZE); // ���ô�������
    SetWindowText(hwnd, "�Թ�����");                                                                      // ���ô��ڱ���

    draw_select_input();
    while (true)
    {
        m = GetMouseMsg();

        if (m.x >= 295 && m.x <= 705 && m.y >= 245 && m.y <= 305) // �ֶ�����
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
        else if (m.x >= 295 && m.x <= 705 && m.y >= 345 && m.y <= 405) // �ļ�����
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
        else if (m.x >= 295 && m.x <= 705 && m.y >= 445 && m.y <= 505) // �˳�
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
            out << "�Թ����£�\n";
            for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    out << maze[i][j] << " ";
                }
                out << "\n";
            }
            out << "���Ϊ: (" << start.x << "," << start.y << ")\n";
            out << "�յ�Ϊ: (" << end.x << "," << end.y << ")\n\n";

            draw_select_mgpth();
            while (true)
            {
                m = GetMouseMsg();
                if (m.x >= 295 && m.x <= 705 && m.y >= 95 && m.y <= 155) // �ݹ�������·��
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 95, 705, 155);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        out << "ʹ�õݹ����Թ�����·����\n";
                        startTime = std::chrono::steady_clock::now();
                        recursion_mgpath(maze_cache, start, end, path, count, exist_path);
                        endTime = std::chrono::steady_clock::now();
                        duration_second = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                        printf("��������ʱ�䣺%fs", duration_second);
                        out << "��������ʱ��: " << duration_second << "s\n";

                        is_use = true;
                    }
                }

                else if (m.x >= 295 && m.x <= 705 && m.y >= 195 && m.y <= 255) // ջ������·��
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 195, 705, 255);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        out << "ʹ��ջ���Թ�����·����\n";
                        startTime = std::chrono::steady_clock::now();
                        stack_mgpath(maze_cache, start, end, count, exist_path);
                        endTime = std::chrono::steady_clock::now();
                        duration_second = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                        printf("��������ʱ�䣺%fs", duration_second);
                        out << "��������ʱ��: " << duration_second << "s\n";
                        is_use = true;
                    }
                }

                else if (m.x >= 295 && m.x <= 705 && m.y >= 295 && m.y <= 355) // ���������·��
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 295, 705, 355);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        out << "ʹ�ö������Թ����·����\n";
                        startTime = std::chrono::steady_clock::now();
                        queue_mgpath(maze_cache, start, end, count, exist_path);
                        endTime = std::chrono::steady_clock::now();
                        duration_second = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                        printf("��������ʱ�䣺%fs", duration_second);
                        out << "��������ʱ��: " << duration_second << "s\n";
                        is_use = true;
                    }
                }

                else if (m.x >= 295 && m.x <= 705 && m.y >= 395 && m.y <= 455) // �ݹ�(ͼ)������·��
                {
                    BeginBatchDraw();
                    setlinecolor(RED);
                    rectangle(295, 395, 705, 455);
                    EndBatchDraw();

                    if (m.uMsg == WM_LBUTTONDOWN)
                    {

                        out << "ʹ�õݹ飨ͼ�����Թ�����·����\n";
                        startTime = std::chrono::steady_clock::now();
                        CreateAdj(G, maze_cache, M, N);
                        graph_mgpath(G, visited, start, end, path, count, exist_path);
                        DestroyAdj(G, M, N);
                        endTime = std::chrono::steady_clock::now();
                        duration_second = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                        printf("��������ʱ�䣺%fs", duration_second);
                        out << "��������ʱ��: " << duration_second << "s\n";
                        is_use = true;
                    }
                }

                else if (m.x >= 295 && m.x <= 705 && m.y >= 495 && m.y <= 555) // ����
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
                        settextstyle(45, 0, "����");
                        setbkmode(TRANSPARENT);
                        outtextxy(350, 300, "δ�ҵ��Թ��Ľ�");
                        sleep(3);
                        draw_select_mgpth();
                        out << "���Թ�δ�ҵ�·��\n";
                    }

                    else
                    {
                        out << "�÷�������Թ�·������Ϊ��" << count << "\n";
                        for (int i = 0; i < count; i++)
                        {
                            show_mgpath(maze, M, N, start, end, paths[i]);
                            out << "��" << i + 1 << "���Թ�·������: \n";
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