#include <vector>
using namespace std;

struct Point
{
	int x,y;

	Point(int x, int y) : x(x), y(y) {}
	bool operator == (const Point &other) const
	{
		return (x == other.x && y == other.y);
	}
	bool operator < (const Point &other) const
	{
		return (x*1000 + y < other.x*1000 + other.y);
	}
};
typedef vector<Point> Points;

struct MapData
{
	int width, height;
	vector<vector<char>> data;
	Points starts;
	Points goals;

	MapData(int width, int height,
			const vector<vector<char>> &data,
			const Points &starts,const Points &goals)
		: width(width), height(height), data(data), starts(starts), goals(goals) {}
};

struct StageData
{
	MapData map;
	int level;

	StageData(const MapData &map, int level) : map(map), level(level) {}
};

struct Tower
{
	Point point;
	int level, kind;

	Tower(const Point &point, int level, int kind) : point(point), level(level), kind(kind) {}
};

struct Enemy
{
	Point point;
	int time, life, speed;

	Enemy(const Point &point, int time, int life, int speed) : point(point), time(time), life(life), speed(speed) {}
};

struct LevelData
{
	int life, money;
	vector<Tower> towers;
	vector<Enemy> enemies;

	LevelData(int life, int money, const vector<Tower> &towers, const vector<Enemy> &enemies) : life(life), money(money), towers(towers), enemies(enemies) {}
};

