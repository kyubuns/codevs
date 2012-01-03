#include <iostream>
#include <queue>
#include <climits>
#include <set>
#include <random>
#include "Loader.h"
#include "Solver.h"
#include "OldSolver.h"
using namespace std;
#define DEBUG_FLAG false
bool DEBUG = false;

static const array<int, 8> dx   = {{  1,  1,  0, -1, -1, -1,  0,  1}};
static const array<int, 8> dy   = {{  0, -1, -1, -1,  0,  1,  1,  1}};
static const array<int, 8> dcost= {{ 2, 3, 2, 3, 2, 3, 2, 3}};
const static array<int, 40> mapkind = {{
	1, 0, 1, 0, 0, 0, 0, 0, 0,		//41-49
	1, 0, 1, 1, 0, 1, 0, 0, 0, 0,	//50-59
	1, 0, 1, 1, 1, 0, 1, 1, 1, 0,	//60-69 60,62
	0, 1, 1, 0, 0, 1, 1, 1, 1, 1,	//70-79
	1								//80
			//76まで
}};
/*
const static array<int, 40> mapkind = {{
	1, 1, 1, 1, 1, 1, 1, 1, 1,		//41-49
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	//51-59
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	//61-69
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	//71-79
	1								//81
}};
*/

class MazeSolver : public Solver
{
public:
	MazeSolver(const StageData &stageData, const LevelData &levelData) : Solver(stageData, levelData), count(0) {}
	MazeSolver(const StageData &stageData, const LevelData &levelData, int money) : Solver(stageData, levelData, money), count(0) {}
	void run(int point);

private:
	bool canMove(const MapData &route, const Point &o, int i) const;
	void createOnlyGoal();
	Point findGoal();
	vector<vector<int>> createDistMap(const Point &goal, int kind);
	void debugPrint(const vector<vector<int>> &map) const;
	const Point createMaze();
	void upgrade();
	int upgradeCir(const Point &o, int r);
	Task createTask(const Point &p);

private:
	int count;
};

bool MazeSolver::canMove(const MapData &route, const Point &o, int i) const
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

void MazeSolver::createOnlyGoal()
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

Point MazeSolver::findGoal()
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

Task MazeSolver::createTask(const Point &p)
{
	count++;
	if(count > 0) count = 0;
	if(count == 0) return Task(p, 0, 0);
	return Task(p, 0, 2);
}

vector<vector<int>> MazeSolver::createDistMap(const Point &goal, int kind)
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

void MazeSolver::debugPrint(const vector<vector<int>> &map) const
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

const Point MazeSolver::createMaze()
{
	using namespace mark;
	const MapData &map = getMap();
	
	createOnlyGoal();
	const Point goal = findGoal();
	vector<vector<int>> dismap(createDistMap(goal, mapkind[level.stage-41]));
	
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

void MazeSolver::run(int point)
{
	const Point goal = createMaze();

	int mapscore = 0;
	for(int w=0; w<stage.map.width; ++w)
	{
		for(int h=0; h<stage.map.height; ++h)
		{
			Task task(Point(w,h), -1, 0);
			if(check(task)) mapscore++;
		}
	}

	int damage;
	for(int i=1;i<=2;++i)
	{
		vector<int> result = simulate();
		damage = result.size();
		if(damage == 0) break;
		if(DEBUG_FLAG) cout << "RESULT : " << result.size() << endl;
		upgradeCir(goal, i);
	}
	if(damage>0)
	{
		int wdt = 0;
		while(wdt++<100)
		{
			vector<int> result = simulate();
			damage = result.size();
			if(damage == 0) break;
			if(DEBUG_FLAG) cout << "RESULT : " << result.size() << endl;
			upgrade();
		}
	}

	/*
	int r = 2;
	int damage;
	bool overdr = false;
	while(true)
	{
		vector<int> result = simulate();
		damage = result.size();
		if(DEBUG_FLAG) cout << "RESULT : " << result.size() << endl;
		if(result.size() <= point) break;

		int build = 0;
		while(build==0)
		{
			cout << "Upgrade : " << r << endl;
			if(r == 15 && ((damage > 0 && level.life == 0) || damage > 1) && !overdr)
			{
				wallet.setMaxMoney();
				overdr = true;
				r = 2;
			}
			if(r%2==0)	build = upgradeCir(goal, r/2);
			else{
				set<Point> enstarts;
				for(int i=0;i<result.size();++i)
				{
					enstarts.insert(level.enemies[i].point);
				}
				for(auto it=enstarts.begin(), end=enstarts.end(); it!=end; ++it)
				{
					//cout << "-";
					//it->print();
					build = upgradeCir(*it, r/2);
				}
			}
			r++;
			if(r>100) break;
		}
		if(r>100) break;
	}
	*/
	if(DEBUG_FLAG)
	{
		static int life = 9;
		if(damage)
		{
			cout << "\x1b[31m" << "Damage : " << damage << "\x1b[39m" <<  endl;
			cout << "Simulate Life : " << life << " => " << life - damage << endl;
		}
		else
		{
			cout << "Simulate Life : " << life << endl;
		}
		life-=damage;
		cout << endl;
	}
}

void MazeSolver::upgrade()
{
	static std::mt19937 engine(19216801);

	//アップグレード
	const Towers &towers = getTowers();
	std::uniform_int_distribution<int> randno( 0, towers.size()-1 ) ;
	if(towers.size()==0) return;
	for(int i=0;i<10;++i)
	{
		const Tower &tower = towers[randno(engine)];
		if(tower.kind == 2) continue;
		if(tower.level == rule::UPGRADE_MAX_LEVEL) continue;

		Task task(tower);
		task.level += 4;

		if(!check(task)) continue;
		build(task);
	}
}

int MazeSolver::upgradeCir(const Point &o, int r)
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
			Task task(n, 4, 0);
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

int main()
{
	ios_base::sync_with_stdio(false);

	//ステージ数
	int s=0,S;
	cin >> S;
	for(s=0; s<40; ++s)
	{
		const StageData stageData = Loader::LoadStage();
		for(int l=0; l<stageData.level; ++l)
		{
			const LevelData levelData = Loader::LoadLevel(s+1, l+1);
			if(!DEBUG)
			{
				OldSolver solver(stageData, levelData);
				solver.run();
				if(!DEBUG_FLAG) solver.result();
			}
		}
	}
	int memoLife = 10;
	for(;s<S; ++s)
	{
		int memoMoney = 0;
		const StageData stageData = Loader::LoadStage();
		for(int l=0; l<stageData.level; ++l)
		{
			const LevelData levelData = Loader::LoadLevel(s+1, l+1);
//			if(s+1!=53 || l+1!=19) continue;
			int a = s+1;
			int b = l+1;
			if(DEBUG) b--;
//if(DEBUG_FLAG) if(l!=0) continue;

			if(!DEBUG)
			{
				int money = 3000;
				if(s<40) money = 4000;
				if(s<50) money = 4500;
				if(s<60) money = 5000;
				if(s<70) money = 5500;
				if(s<80) money = 6000;
				MazeSolver solver(stageData, levelData, money);
				if(DEBUG_FLAG) cout << s+1 << "-" << l+1 << endl;
				if(DEBUG_FLAG) cout << "Life : " << levelData.life << endl;
		//		cout << "MONEY : " << levelData.money << endl;
		//		solver.printMap();
				int point = 0;
				solver.run(point);
				if(!DEBUG_FLAG) solver.result();
		//		if(DEBUG_FLAG) solver.printMap();
			}
			else
			{
				cout << s+1 << "-" << l+1 << endl;
				cout << "Life : " << levelData.life << endl;
			}
		}
	}
	return 0;
}
