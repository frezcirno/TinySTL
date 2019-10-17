#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

constexpr auto WALL = '#';

struct coord
{
	int x;
	int y;
	bool operator==(coord &rhs) { return x == rhs.x && y == rhs.y; }
};

coord mapSize;
char *map;

void printmap()
{
	cout << setw(6) << " ";
	for (int j = 0; j < mapSize.y; j++)
	{
		cout << j << left << setw(5) << "列";
	}
	cout << endl;
	for (int i = 0; i < mapSize.x; i++)
	{
		cout << i << left << setw(5) << "行";
		for (int j = 0; j < mapSize.y; j++)
		{
			cout << left << setw(6) << map[i * mapSize.x + j];
		}
		cout << endl
			 << endl;
	}
}

bool dfs(vector<coord> &path, coord now, coord end)
{
	path.push_back(now);
	map[now.x * mapSize.x + now.y] = WALL;
	if (now == end)
		return true;
	const coord step[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
	for (int dir = 0; dir < 4; dir++)
	{
		coord next;
		next.x = now.x + step[dir].x;
		next.y = now.y + step[dir].y;
		if (next.x < 0 || next.x >= mapSize.x || next.y < 0 || next.y >= mapSize.y)
			continue;
		if (map[next.x * mapSize.x + next.y] == WALL)
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
	cin >> mapSize.x >> mapSize.y;
	map = new char[mapSize.x * mapSize.y]; //一维数组存储二维
	for (int i = 0; i < mapSize.x; i++)
	{
		for (int j = 0; j < mapSize.y; j++)
		{
			cin >> map[i * mapSize.x + j];
		}
	}

	coord start, end;
	vector<coord> path; //vector模拟栈

	printmap();
	cout << "请输入起点坐标和终点坐标(行 列): ";
	cin >> start.x >> start.y >> end.x >> end.y;
	if (dfs(path, start, end))
	{
		for (int i = 0; i < path.size(); i++)
		{
			if (i != 0)
				cout << " ---> ";
			cout << '(' << path[i].x << ',' << path[i].y << ')';
		}
	}
	else
		cout << "找不到路径,请检查起点和终点坐标" << endl;

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