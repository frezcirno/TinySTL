#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

constexpr auto WALL = '#';

typedef pair<int, int> pii;

pii mapSize;
char* map;

void printmap() {
	cout << setw(6) <<" ";
	for (int j = 0; j < mapSize.second; j++)
	{
		cout << j << left << setw(5) << "列";
	}
	cout << endl;
	for (int i = 0; i < mapSize.first; i++)
	{
		cout << i << left << setw(5) << "行";
		for (int j = 0; j < mapSize.second; j++)
		{
			cout << left << setw(6) << map[i * mapSize.first + j];
		}
		cout << endl << endl;
	}
}


bool dfs(vector<pii>& path, pii now, pii end) {
	path.push_back(now); map[now.first * mapSize.first + now.second] = WALL;
	if (now == end) return true;
	const pii step[4] = { {0,-1},{1,0},{0,1},{-1,0} };
	for (int dir = 0; dir < 4; dir++)
	{
		pii next(now.first + step[dir].first, now.second + step[dir].second);
		if (next.first < 0 || next.first >= mapSize.first || next.second < 0 || next.second >= mapSize.second)
			continue;
		if (map[next.first * mapSize.first + next.second] == WALL)
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
	map = new char[mapSize.first * mapSize.second];//一维数组存储二维
	for (int i = 0; i < mapSize.first; i++)
	{
		for (int j = 0; j < mapSize.second; j++)
		{
			cin >> map[i * mapSize.first + j];
		}
	}

	pii start, end;
	vector<pii> path;//vector模拟栈

	printmap();
	cout << "请输入起点坐标和终点坐标(行 列): ";
	cin >> start.first >> start.second >> end.first >> end.second;
	if (dfs(path, start, end)) {
		for (int i = 0; i < path.size(); i++)
		{
			if (i != 0)cout << " ---> ";
			cout << '(' << path[i].first << ',' << path[i].second << ')';
		}
	} else cout << "找不到路径,请检查起点和终点坐标" << endl;

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