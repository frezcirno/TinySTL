#include <iostream>
#include "../tinySTL/vector.hpp"
#include "../tinySTL/pair.hpp"
using namespace std;

typedef tinySTL::pair<int, int> coord;

constexpr auto WALL = '#';

coord mapSize;
char *map;

//检查坐标是否有效
inline bool checkpos(coord pos)
{
    return (pos.first >= 0 && pos.first < mapSize.first && pos.second >= 0 && pos.second < mapSize.second);
}

void printmap()
{
    printf("     ");
    for (int j = 0; j < mapSize.second; j++)
        printf("%2d列  ", j);
    printf("\n");
    for (int i = 0; i < mapSize.first; i++)
    {
        printf("%2d行  ", i);
        for (int j = 0; j < mapSize.second; j++)
            printf("%-6c", map[i * mapSize.first + j]);
        printf("\n\n");
    }
    //PS:丑爆了
}

bool dfs(tinySTL::vector<coord> &path, coord now, coord end)
{
    const static coord step[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    path.push_back(now);
    map[now.first * mapSize.first + now.second] = WALL;
    if (now == end)
        return true;
    for (int i = 0; i < 4; i++)
    {
        coord next(now.first + step[i].first, now.second + step[i].second);
        if (!checkpos(next) || map[next.first * mapSize.first + next.second] == WALL)
            continue;
        if (dfs(path, next, end))
            return true;
    }
    path.pop_back();
    return false;
}

int main()
{
    cout << "请输入地图的行数和列数: " << endl;
    cin >> mapSize.first >> mapSize.second;
    map = new char[mapSize.first * mapSize.second]; //一维数组存储二维
    for (int i = 0; i < mapSize.first; i++)
        for (int j = 0; j < mapSize.second; j++)
            cin >> map[i * mapSize.first + j];

    printmap();

    coord start, end;
    tinySTL::vector<coord> path; //vector模拟栈

    cout << "请输入起点坐标和终点坐标(行 列): ";
    cin >> start.first >> start.second >> end.first >> end.second;
    if (!dfs(path, start, end))
        cout << "找不到路径,请检查起点和终点坐标" << endl;
    else
    {
        for (int i = 0; i < path.size(); i++)
        {
            if (i != 0)
                cout << " ---> ";
            cout << '(' << path[i].first << ',' << path[i].second << ')';
        }
    }

    return 0;
}

/*
10 10
. . # # # . . . # .
# . # . . . # . # .
. . . . # # # . # .
. # # . . . # . # .
. . # # # . # . # .
# . # . . . # . # .
# . # . # # # . # .
# . # . . . # . # .
# . # # # . # . . .
# . . . . . # # # #

0 0 0 9


*/