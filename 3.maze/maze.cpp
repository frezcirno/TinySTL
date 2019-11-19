#include <ctime>
#include <iostream>

#include "../tinySTL/pair.hpp"
#include "../tinySTL/queue.hpp"
#include "../tinySTL/stack.hpp"
#include "../tinySTL/vector.hpp"
#include "../tinySTL/priority_queue.hpp"
#include "disj_set.hpp"
using namespace std;

//类型定义
typedef tinySTL::pair<int, int> coord;
enum block_type { WALL, BLANK };
const coord step[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};  // 0,3,6,9

//全局变量
coord       mapSize;
block_type *map;

/**
 * 随机数宏
 */
#define ODD_RAND(x) (rand() % ((x) / 2) * 2 + 1)  //产生[0,x)之间的随机奇数(x=1时会返回0)
#define EVEN_RAND(x) (rand() % (((x) + 1) / 2) * 2)  //产生[0,x)之间的随机偶数
#define TO_ODD(x) (((x) % 2 == 0) ? (x) - 1 : (x))
#define TO_EVEN(x) (((x) % 2) ? (x) - 1 : (x))

#define ODD_RAND_RANGE(x, y) ( (x) + ODD_RAND( ((y)-(x)) ) )  //产生[x,y)之间下标为奇数的随机数(y=x+1时会返回x)
#define EVEN_RAND_RANGE(x, y) ( (x) + EVEN_RAND( ((y)-(x)) ) )  //产生[x,y)之间下标为偶数的随机数

/**
 * 检查坐标是否有效
 */
inline bool is_valid(const coord &pos)
{
    return (pos.first >= 0 && pos.first < mapSize.first && pos.second >= 0 &&
            pos.second < mapSize.second);
}

inline bool is_wall(const coord &pos)
{
    return !is_valid(pos) ||
           (map[pos.first * mapSize.first + pos.second] == WALL);
}


/**
 * 迷宫绘制
 */
#define WALL_PATTERN "##"
void paint_map(const tinySTL::vector<coord> *path = nullptr)
{
	//顶部数字
    printf("     ");
    for (int j = 0; j < mapSize.second; j++)
        printf("%2d", (j % 10 == 0 ? j / 10 : j % 10));
    printf("\n");

	//上边框
    printf("   " WALL_PATTERN);
    for (int j = 0; j < mapSize.second; j++) printf(WALL_PATTERN);
    printf(WALL_PATTERN "\n");

	//中间行
    for (int i = 0; i < mapSize.first; i++) {
        printf("%2d " WALL_PATTERN, (i % 10 == 0 ? i : i % 10));
        for (int j = 0; j < mapSize.second; j++) {
            if (path && tinySTL::find(path->begin(), path->end(),
                            coord(i, j)) != path->end())
                printf(" +");
            else
                printf("%2s",
                    (map[i * mapSize.first + j] == WALL ? WALL_PATTERN : "  "));
        }
        printf(WALL_PATTERN "\n");
    }

	//下边框
    printf("   " WALL_PATTERN);
    for (int j = 0; j < mapSize.second; j++) printf(WALL_PATTERN);
    printf(WALL_PATTERN "\n");
}


/**
 * 迷宫生成算法ABC
 */
void randomized_prim(coord start)
{
    srand(time(NULL));
    for (int i = 0; i < mapSize.first; i++)
        for (int j = 0; j < mapSize.second; j++)
            map[i * mapSize.first + j] = WALL;
    map[start.first * mapSize.first + start.second] = BLANK;

    disj_set<coord>        ds;  //并查集
    tinySTL::vector<coord> walls;
    for (int i = 0; i < mapSize.first; i++)
        for (int j = (i + 1) % 2; j < mapSize.second; j += 2)
            walls.push_back(coord(i, j));

    while (!walls.empty()) {
        int   randnum = rand() % walls.size();
        coord awall = walls[randnum];
        walls.erase(walls.begin() + randnum);

        if (awall.first % 2) {  //水平墙
            coord up(awall.first - 1, awall.second);
            coord down(awall.first + 1, awall.second);
            if (is_valid(up) && is_valid(down) && !ds.is_same(up, down)) {
                map[up.first * mapSize.first + up.second] = BLANK;
                map[down.first * mapSize.first + down.second] = BLANK;
                map[awall.first * mapSize.first + awall.second] = BLANK;
                ds.join(up, down);
            }
        } else {  //竖直墙
            coord left(awall.first, awall.second - 1);
            coord right(awall.first, awall.second + 1);
            if (is_valid(left) && is_valid(right) && !ds.is_same(left, right)) {
                map[left.first * mapSize.first + left.second] = BLANK;
                map[right.first * mapSize.first + right.second] = BLANK;
                map[awall.first * mapSize.first + awall.second] = BLANK;
                ds.join(left, right);
            }
        }
    }
}

void recursive_backtrack(coord start)
{
    srand(time(NULL));
    for (int i = 0; i < mapSize.first; i++)
        for (int j = 0; j < mapSize.second; j++)
            map[i * mapSize.first + j] = WALL;

    bool *visited = new bool[mapSize.first * mapSize.second];
    memset(visited, 0, mapSize.first * mapSize.second);
    visited[start.first * mapSize.first + start.second] = true;

    tinySTL::stack<coord> s;
    s.push(start);
    map[start.first * mapSize.first + start.second] = BLANK;

    coord current;
    while (!s.empty()) {  // 2.当还存在未标记的迷宫单元，进行循环
        current = s.top();
        s.pop();

        map[current.first * mapSize.first + current.second] = BLANK;

        tinySTL::vector<coord> unvisited_neib;
        for (int dir = 0; dir < 4; dir++) {
            coord next(current.first + 2 * step[dir].first,
                current.second + 2 * step[dir].second);
            if (is_valid(next) &&
                !visited[next.first * mapSize.first + next.second]) {
                coord mid((current.first + next.first) / 2,
                    (current.second + next.second) / 2);
                map[mid.first * mapSize.first + mid.second] = BLANK;
                visited[next.first * mapSize.first + next.second] = true;
                unvisited_neib.push_back(next);
            }
        }
        if (unvisited_neib.size()) {
            // 当前迷宫单元有未被访问过的的相邻的迷宫单元
            int rand_dir = rand() % unvisited_neib.size();
            for (size_t i = 0; i < unvisited_neib.size(); i++)
                if (i != rand_dir) s.push(unvisited_neib[i]);
            s.push(unvisited_neib[rand_dir]);
        }
    }
    delete[] visited;
}

void _recursive_division(coord leftup, coord rightdown)
{
    if (rightdown.first - leftup.first < 3 ||
        rightdown.second - leftup.second < 3)
        return;

    coord div_point(ODD_RAND_RANGE(leftup.first, rightdown.first),
        ODD_RAND_RANGE(leftup.second, rightdown.second));

	for (int i = leftup.first; i < rightdown.first; i++)
        map[i * mapSize.first + div_point.second] = WALL;
    for (int i = leftup.second; i < rightdown.second; i++)
        map[div_point.first * mapSize.first + i] = WALL;

    //paint_map();
    int lucky = rand() % 4;
    if (lucky != 0) {
        map[EVEN_RAND_RANGE(leftup.first, div_point.first) * mapSize.first +
            div_point.second] = BLANK;
    }
    if (lucky != 1) {
        map[div_point.first * mapSize.first +
            EVEN_RAND_RANGE(div_point.second + 1, rightdown.second)] = BLANK;
    }
    if (lucky != 2) {
        map[(EVEN_RAND_RANGE(div_point.first + 1, rightdown.first)) *
                mapSize.first +
            div_point.second] = BLANK;
    }
    if (lucky != 3) {
        map[div_point.first * mapSize.first +
            EVEN_RAND_RANGE(leftup.second, div_point.second)] = BLANK;
    }
    //paint_map();
    _recursive_division(leftup, div_point);
    _recursive_division(coord(leftup.first, div_point.second + 1),
        coord(div_point.first, rightdown.second));
    _recursive_division(coord(div_point.first + 1, leftup.second),
        coord(rightdown.first, div_point.second));
    _recursive_division(
        coord(div_point.first + 1, div_point.second + 1), rightdown);
}

void recursive_division()
{
    srand(time(NULL));
    coord rightdown(TO_ODD(mapSize.first), TO_ODD(mapSize.second));
    for (int i = 0; i < mapSize.first; i++)
        for (int j = 0; j < mapSize.second; j++)
            map[i * mapSize.first + j] =
                ((i == rightdown.first || j == rightdown.second) ? WALL : BLANK);
    _recursive_division(coord(0, 0), rightdown);
}

void manual_input()
{
    std::cout << "请逐行输入map数据(#代表墙壁):" << std::endl;
    for (int i = 0; i < mapSize.first; i++)
        for (int j = 0; j < mapSize.second; j++) {
            char ch;
            std::cin >> ch;
            map[i * mapSize.first + j] = (ch == '#' ? WALL : BLANK);
        }
}

/**
 * 迷宫寻路算法ABC
 */
bool dfs(tinySTL::vector<coord> &path, coord start, coord end)
{
    bool *visited = new bool[mapSize.first * mapSize.second];
    memset(visited, 0, mapSize.first * mapSize.second);

    path.clear();
    path.push_back(start);  //用参数path作栈
    visited[start.first * mapSize.first + start.second] = true;

    coord current;
    while (!path.empty()) {
        current = path.back();
        if (current == end) break;
        bool no_way = true;
        for (int i = 0; i < 4; i++) {
            coord next(
                current.first + step[i].first, current.second + step[i].second);
            if (!is_wall(next) &&
                !visited[next.first * mapSize.second + next.second]) {
                path.push_back(next);
                visited[next.first * mapSize.second + next.second] = true;
                no_way = false;
                break;
            }
        }
        if (no_way) path.pop_back();
    }
    delete[] visited;
    if (path.empty()) return false;
    return true;
}

bool bfs(tinySTL::vector<coord> &path, coord start, coord end)
{
    tinySTL::map<coord, coord> parent;

    tinySTL::queue<coord> q;
    q.push(start);
    parent[start] = start;

    coord current;
    while (!q.empty()) {
        current = q.front(); q.pop();
        if (current == end) break;
        for (int i = 0; i < 4; i++) {
            coord next;
            next.first = current.first + step[i].first;
            next.second = current.second + step[i].second;
            if (!is_wall(next) && !parent.count(next)) {
                q.push(next);
                parent[next] = current;
            }
        }
    }
    if (q.empty()) return false;
    path.clear();
    while (parent[current] != current) {
        path.insert(path.begin(), current);
        current = parent[current];
    }
    path.insert(path.begin(), start);
    return true;
}

struct coord_compare {
    coord end;
    tinySTL::map<coord, int> *g_map;
    coord_compare(const coord &iend, tinySTL::map<coord, int> *ig_map) : end(iend),g_map(ig_map) {}
    int H(const coord &cur)
    {
        return abs(cur.first - end.first) + abs(cur.second - end.second);
    }
    int F(const coord &cur)
    {
        return (g_map->count(cur) ? (*g_map)[cur] : 0) + H(cur);
    }
    bool operator()(const coord &x, const coord &y) { return F(x) - F(y); }
};

bool a_star(tinySTL::vector<coord> &path, coord start, coord end)
{
    tinySTL::map<coord, coord> parent;
    tinySTL::map<coord, int>   g_map;

    tinySTL::priority_queue<coord, coord_compare> pq(coord_compare(end, &g_map));
    pq.push(start);
    parent[start] = start;
    g_map[start] = 0;

    coord current;
    while (!pq.empty()) {
        current = pq.top(); pq.pop();
        if (current == end) break;
        for (int i = 0; i < 4; i++) {
            coord next;
            next.first = current.first + step[i].first;
            next.second = current.second + step[i].second;
            if (!is_wall(next) &&
                (!g_map.count(next) || g_map[next] > g_map[current] + 1)) {
                parent[next] = current;
                g_map[next] = g_map[current] + 1;
                pq.push(next);
            }
        }
    }
    if (pq.empty()) return false;
    path.clear();
    while (parent[current] != current) {
        path.insert(path.begin(), current);
        current = parent[current];
    }
    path.insert(path.begin(), start);
    return true;
}

int main()
{
    char ch;
    /* Step 1. 输入迷宫的大小 */
    while (true) {
        cout << "请输入地图的行数和列数, 以空格分隔(默认为51 51): ";
        if (cin.get() == '\n') {
            mapSize = {51, 51};  //默认迷宫大小
            break;
        }
        cin.unget();
        cin >> mapSize.first >> mapSize.second;
        cin.ignore(4096, '\n');  //跳过后续的输入
        if (cin.fail())
            printf("输入有误\n");
        else if (mapSize.first <= 0 || mapSize.first > 100)
            printf("行数有误\n");
        else if (mapSize.second <= 0 || mapSize.second > 100)
            printf("列数有误\n");
        else
            break;
        cin.clear();             //重置错误标志位
    }

    printf("迷宫大小: %d x %d\n\n", mapSize.first, mapSize.second);
    map = new block_type[mapSize.first * mapSize.second];

    /* Step 2. 选择迷宫生成算法 */
    cout << "迷宫生成方式: " << endl;
    cout << "0. Randomized Prim's algorithm (default)" << endl;
    cout << "1. Recursive backtracker" << endl;
    cout << "2. Recursive division" << endl;
    cout << "3. 手动输入" << endl;
    cout << "请选择迷宫生成方式: ";
    ch=cin.get();
    while (isblank(ch) && ch != '\n') ch = cin.get();
    switch (ch) {
        case '1': recursive_backtrack(coord(0, 0)); break;
        case '2': recursive_division(); break;
        case '3': manual_input(); break;
        default: randomized_prim(coord(0, 0)); break;
    }

    paint_map();

	/* Step 3. 输入迷宫起点和终点 */
    coord start, end;
    while (true) {
        cout << "请输入起点坐标和终点坐标(行 列): ";
        if (cin.get() == '\n') {
            start = {0, 0};  //默认迷宫起点为左上角, 终点为右下角
            end = {TO_EVEN(mapSize.first - 1), TO_EVEN(mapSize.second - 1)};
            break;
        }
        cin.unget();
        cin >> start.first >> start.second >> end.first >> end.second;
        if (cin.fail())
            printf("输入有误\n");
        else if (is_wall(start))
            printf("起点有误, 请重新输入\n");
        else if (is_wall(end))
            printf("终点有误, 请重新输入\n");
        else
            break;
        cin.clear();             //重置错误标志位
        cin.ignore(4096, '\n');  //跳过后续的输入
    }
    printf("迷宫起点为(%d, %d), 终点为(%d, %d)\n\n", start.first, start.second,
        end.first, end.second);

    /* Step 4. 选择迷宫寻路算法 */
step4:
	tinySTL::vector<coord> route;  // vector模拟栈
    cout << "迷宫求解方式：" << endl;
    cout << "0. A-star (default)" << endl;
    cout << "1. DFS" << endl;
    cout << "2. BFS" << endl;
    cout << "请选择迷宫求解方式：";
    ch = cin.get();
    while (isblank(ch) && ch != '\n') ch = cin.get();
    switch (ch) {
        case '1': dfs(route, start, end); break;
        case '2': bfs(route, start, end); break;
        default: a_star(route, start, end); break;
    }

    /* Step 5. 打印路径 */
    if (route.empty())
        cout << "找不到路径,请检查起点和终点坐标" << endl;
    else {
        for (int i = 0; i < route.size(); i++) {
            if (i != 0) cout << " ---> ";
            printf("(%d, %d)", route[i].first, route[i].second);
        }
        cout << endl;
        paint_map(&route);
    }

	cin.ignore(4096, '\n');
    cout << "继续尝试其他算法?[y]/n";
    ch = cin.get();
    if (ch == '\n' || ch == 'y') goto step4;
    return 0;
}