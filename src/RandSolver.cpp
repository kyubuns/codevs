#include <iostream>
#include <queue>
#include <climits>
#include <set>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include "RandSolver.h"
#include "Logger.h"
#define DEBUG_FLAG false
using namespace std;
std::mt19937 RandSolver::engine = std::mt19937(static_cast<unsigned long>(time(0)));

static const array<int, 8> dx   = {{  1,  1,  0, -1, -1, -1,  0,  1}};
static const array<int, 8> dy   = {{  0, -1, -1, -1,  0,  1,  1,  1}};
static const array<int, 8> dcost= {{ 2, 3, 2, 3, 2, 3, 2, 3}};
boost::mt19937 no_state(static_cast<unsigned long>(time(0)));

unsigned bar(unsigned i)
{
	boost::uniform_int<> rng(0, i - 1);
	return rng(no_state);
}

bool RandSolver::canMove(const MapData &route, const Point &o, int i) const
{
	using namespace mark;
	if(dcost[i] == dcost[1])
	{
		//斜め移動できるか確認
		int n = i+1; if(n>7) n-=8;
		int b = i-1; if(b<0) b+=8;

		Point np(o.x + dx[n], o.y + dy[n]);
		Point bp(o.x + dx[b], o.y + dy[b]);
		if((route[np.x][np.y] != EMPTY && route[np.x][np.y] != START && route[np.x][np.y] != GOAL)
				|| (route[bp.x][bp.y] != EMPTY && route[bp.x][bp.y] != START && route[bp.x][bp.y] != GOAL)) return false;
	}
	Point p(o.x + dx[i], o.y + dy[i]);
	if(route[p.x][p.y] != EMPTY && route[p.x][p.y] != START && route[p.x][p.y] != GOAL) return false;
	return true;
}

void RandSolver::createOnlyGoal()
{
	using namespace mark;
	const MapData &map = getMap();

	//ゴールを最も遠い1つにする
	MapData checkedmap(stage.map.width, vector<char>(stage.map.height, 0));
	int goalCount = stage.map.goals.size();
	typedef pair<int, Point> NODE;
	priority_queue<NODE, vector<NODE>, greater<NODE>> que;
	for(auto it = stage.map.starts.begin(), end = stage.map.starts.end(); it != end; ++it)
	{
		que.push(make_pair(0, *it));
	}
	while(!que.empty())
	{
		int cost = que.top().first;
		Point o = que.top().second;
		que.pop();
		if(checkedmap[o.x][o.y] == 1) continue;
		checkedmap[o.x][o.y] = 1;

		if(goalCount == 1) break;
		if(map[o.x][o.y] == GOAL)
		{
			goalCount--;
			for(int i=0;i<8;i+=2)
			{
				Point p(o.x+dx[i], o.y+dy[i]);
				Task task(createTask(p));
				if(map[p.x][p.y] == TOWER) continue;
				if(check(task)) build(task);
			}
		}

		for(int i=0;i<8;++i)
		{
			Point p(o.x+dx[i], o.y+dy[i]);
			if(!canMove(map, o, i)) continue;
			que.push(make_pair(cost+dcost[i], p));
		}
	}
}

Point RandSolver::findGoal()
{
	using namespace mark;
	const MapData &map = getMap();

	//最終的なゴールを探索する
	MapData checkedmap(stage.map.width, vector<char>(stage.map.height, 0));
	typedef pair<int, Point> NODE;
	priority_queue<NODE, vector<NODE>, greater<NODE>> que;
	for(auto it = stage.map.starts.begin(), end = stage.map.starts.end(); it != end; ++it)
	{
		que.push(make_pair(0, *it));
	}
	while(!que.empty())
	{
		int cost = que.top().first;
		Point o = que.top().second;
		que.pop();
		if(checkedmap[o.x][o.y] == 1) continue;
		checkedmap[o.x][o.y] = 1;

		if(map[o.x][o.y] == GOAL)
		{
			return o;
		}

		for(int i=0;i<8;++i)
		{
			Point p(o.x+dx[i], o.y+dy[i]);
			if(!canMove(map, o, i)) continue;
			que.push(make_pair(cost+dcost[i], p));
		}
	}
	return Point(0,0);
}

Task RandSolver::createTask(const Point &p)
{
	count++;
	if(count > 0) count = 0;
	if(count == 0) return Task(p, 0, 0);
	return Task(p, 0, 2);
}

vector<vector<int>> RandSolver::createDistMap(const Point &goal, int kind)
{
	vector<vector<int>> distmap(stage.map.width, vector<int>(stage.map.height, -1));

	using namespace mark;
	const MapData &map = getMap();

	//ゴールからの距離を探索
	typedef pair<int, Point> NODE;
	priority_queue<NODE, vector<NODE>, greater<NODE>> que;
	que.push(make_pair(0, goal));

	while(!que.empty())
	{
		int cost = que.top().first;
		Point o = que.top().second;
		que.pop();
		if(distmap[o.x][o.y] != -1) continue;
		distmap[o.x][o.y] = cost;

		for(int i=0;i<8;i++)
		{
			Point p(o.x+dx[i], o.y+dy[i]);
			if(kind == 0)
			{
				if(p.x<0 || p.x>=stage.map.width || p.y<0 || p.y>=stage.map.height) continue;
			}
			else
			{
				if(!canMove(map, o, i)) continue;
			}
			que.push(make_pair(cost+dcost[i], p));
		}
	}

	return distmap;
}

void RandSolver::debugPrint(const vector<vector<int>> &map) const
{
	int W = stage.map.width;
	int H = stage.map.height;
	for(int h=0; h<H; ++h)
	{
		for(int w=0; w<W; ++w)
		{
			cout << map[w][h] << " ";
		}
		cout << endl;
	}
}

const Point RandSolver::createMaze()
{
	using namespace mark;
	const MapData &map = getMap();
	
	createOnlyGoal();
	const Point goal = findGoal();
	vector<vector<int>> dismap(createDistMap(goal, mapkind));
	
	typedef pair<int, Point> NODE;
	priority_queue<NODE, vector<NODE>, greater<NODE>> que;
	MapData checkedmap(stage.map.width, vector<char>(stage.map.height, 0));
	for(auto it = stage.map.starts.begin(), end = stage.map.starts.end(); it != end; ++it)
	{
		que.push(make_pair(0, *it));
	}
	while(!que.empty())
	{
		int d = que.top().first;
		Point o = que.top().second;
		que.pop();
		if(checkedmap[o.x][o.y] == 1) continue;
		checkedmap[o.x][o.y] = 1;
		if(map[o.x][o.y] == GOAL) continue;

		set<pair<int, Point>> tmp;
		for(int i=0;i<8;i+=2)
		{
			Point p(o.x+dx[i], o.y+dy[i]);
			if(map[p.x][p.y]!=EMPTY && map[p.x][p.y]!=START && map[p.x][p.y]!=GOAL) continue;
			if(checkedmap[p.x][p.y] == 1) continue;
			tmp.insert(make_pair(dismap[p.x][p.y], p));
		}
		if(tmp.size() == 0) continue;
		for(auto it=tmp.begin(), end=tmp.end(); it!=end;)
		{
			if(tmp.size() == 1) break;
			const Point &p = it->second;

			Task task(createTask(p));
			if(!check(task)) { ++it; continue; }
			build(task);
			tmp.erase(it++);
		}
		que.push(make_pair(d+1, tmp.begin()->second));
	}
	return goal;
}

int RandSolver::score()
{
	using namespace mark;
	createMaze();
	createMaze();
	createMaze();

	int mapscore = INT_MAX;

	for(auto it = stage.map.starts.begin(), end = stage.map.starts.end(); it != end; ++it)
	{
		int score = 0;

		typedef pair<int, Point> NODE;
		priority_queue<NODE, vector<NODE>, greater<NODE>> que;
		auto map = getMap();
		MapData checkedmap(stage.map.width, vector<char>(stage.map.height, 0));
		que.push(make_pair(0, *it));
		while(!que.empty())
		{
			int d = que.top().first;
			Point o = que.top().second;
			que.pop();
			if(checkedmap[o.x][o.y] == 1) continue;
			checkedmap[o.x][o.y] = 1;
			if(map[o.x][o.y] == GOAL)
			{
				score = d;
				break;
			}

			for(int i=0;i<8;i+=2)
			{
				Point p(o.x+dx[i], o.y+dy[i]);
				if(map[p.x][p.y]!=EMPTY && map[p.x][p.y]!=START && map[p.x][p.y]!=GOAL) continue;
				que.push(make_pair(d+1, p));
			}
		}

		mapscore = min(mapscore, score);
	}

	return mapscore;
}

pair<int,int> RandSolver::run(bool buildflag)
{
	createMaze();
	createMaze();
	const Point goal = createMaze();

	int damage;
	for(int i=1;i<=5;++i)
	{
		vector<int> result = simulate();
		damage = result.size();
		if(damage == 0) break;
		if(DEBUG_FLAG) cout << "RESULT1 : " << result.size() << endl;
		upgradeCir(goal, i);
	}
	if(damage>0)
	{
		if(buildflag)
		{
			for(int w=0; w<stage.map.width; ++w)
			{
				for(int h=0; h<stage.map.height; ++h)
				{
					Task task(Point(w,h), 0, 0);
					if(check(task)) build(task);
				}
			}
		}
		const Towers &towers = getTowers();
		int towerSize = towers.size();
		vector<int> randsh(towerSize);
		for(int i=0;i<towerSize;++i) randsh[i] = i;
		random_shuffle(randsh.begin(), randsh.end(), bar);
		static const int di = 1;
		int i = 0;
		while(damage > 0 && i<towerSize)
		{
			int memo = i;
			for(;i<memo+di;++i)
			{
				if(i>=towerSize) break;
				Task task(towers[randsh[i]].point, 4, 0);
				if(check(task)) build(task);
				else Logger::write("n");
			}

			vector<int> result = simulate();
			damage = result.size();
		}
	}
	if(DEBUG_FLAG)
	{
		if(damage)
		{
			cout << "\x1b[31m" << "Damage : " << damage << "\x1b[39m" <<  endl;
		}
	}
	return make_pair(damage, wallet.getUseMoney());
}

int RandSolver::upgrade()
{
	//アップグレード
	int count = 0;
	const Towers &towers = getTowers();
	std::uniform_int_distribution<int> randno( 0, towers.size()-1 ) ;
	if(towers.size()==0) return -1;
	for(int i=0;i<3;++i)
	{
		const Tower &tower = towers[randno(engine)];
		if(tower.kind == 2) continue;
		if(tower.level == rule::UPGRADE_MAX_LEVEL) continue;

		Task task(tower);
		task.level += 4;

		if(!check(task)) continue;
		build(task);
		count++;
	}
	return count;
}

int RandSolver::upgradeCir(const Point &o, int r)
{
	int counter = 0;
	typedef pair<int, Point> NODE;
	priority_queue<NODE, vector<NODE>, greater<NODE>> que;
	MapData checkedmap(stage.map.width, vector<char>(stage.map.height, 0));
	que.push(make_pair(0, o));
	checkedmap[o.x][o.y] = 1;

	static const array<int, 4> dx = {{  1,  0, -1,  0}};
	static const array<int, 4> dy = {{  0, -1,  0,  1}};
	while(!que.empty())
	{
		int cost = que.top().first;
		Point p = que.top().second;
		que.pop();
		for(int i=0;i<4;++i)
		{
			Point n(p.x+dx[i], p.y+dy[i]);
			if(n.x<0 || n.x>=stage.map.width || n.y<0 || n.y>=stage.map.height) continue;
			if(checkedmap[n.x][n.y]) continue;
			checkedmap[n.x][n.y] = 1;
			Task task(n, 0, 0);
			if(check(task))
			{
				build(task);
				//task.print();
				++counter;
			}
			if(cost+1 >= r) continue;
			que.push(make_pair(cost+1, n));
		}
	}
	return counter;
}
