#include <vector>
#include <array>
#include <iostream>
using namespace std;

namespace rule
{
	static const array<int, 3> COST = {10, 15, 20};
	static const int UPGRADE_MAX_LEVEL = 4;
}

struct Point
{
	int x,y;

	Point() : x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}
	void print() { cout << "Point:(" << x << "," << y << ")"; }
	bool operator == (const Point &other) const
	{
		return (x == other.x && y == other.y);
	}
	bool operator != (const Point &other) const
	{
		return !(*this == other);
	}
	bool operator < (const Point &other) const
	{
		return (x*1000 + y < other.x*1000 + other.y);
	}
};
typedef vector<Point> Points;

typedef vector<vector<char>> MapData;
struct MapInfo
{
	int width, height;
	MapData data;
	Points starts;
	Points goals;

	MapInfo(int width, int height,
			const MapData &data,
			const Points &starts,const Points &goals)
		: width(width), height(height), data(data), starts(starts), goals(goals) {}

	void print()
	{
		cout << " --Map-- " << endl;
		int W = width;
		int H = height;
		for(int h=0;h<H;++h)
		{
			for(int w=0;w<W;++w)
			{
				cout << data[w][h];
			}
			cout << endl;
		}
		cout << " ------- " << endl;
	}
};

struct StageData
{
	MapInfo map;
	int level;

	StageData(const MapInfo &map, int level) : map(map), level(level) {}
};

struct Tower;
struct Task
{
	Point point;
	int level, kind;

	Task(const Point &point, int level, int kind) : point(point), level(level), kind(kind) {}
	Task(const Tower &tower);
	void print() { cout << "Task:(" << point.x << "," << point.y << ")" << ", L" << level << ", K" << kind << endl; }
};

struct Tower
{
	Point point;
	int level, kind;

	Tower(const Point &point, int level, int kind) : point(point), level(level), kind(kind) {}
	Tower(const Task &task) : point(task.point), level(task.level), kind(task.kind) {}
};
typedef vector<Tower> Towers;
inline Task::Task(const Tower &tower) : point(tower.point), level(tower.level), kind(tower.kind) {}

struct Enemy
{
	Point point;
	int time, life, speed;

	Enemy(const Point &point, int time, int life, int speed) : point(point), time(time), life(life), speed(speed) {}
};
typedef vector<Enemy> Enemies;

struct LevelData
{
	int life, money;
	Towers towers;
	Enemies enemies;

	LevelData(int life, int money, const Towers &towers, const Enemies &enemies) : life(life), money(money), towers(towers), enemies(enemies) {}
};

