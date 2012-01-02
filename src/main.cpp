#include <iostream>
#include <queue>
#include <climits>
#include <set>
#include <random>
#include "Loader.h"
#include "Solver.h"
#include "OldSolver.h"
using namespace std;

static const array<int, 8> dx   = {{  1,  1,  0, -1, -1, -1,  0,  1}};
static const array<int, 8> dy   = {{  0, -1, -1, -1,  0,  1,  1,  1}};
static const array<int, 8> dcost= {{ 10, 14, 10, 14, 10, 14, 10, 14}};

class MazeSolver : public Solver
{
public:
	MazeSolver(const StageData &stageData, const LevelData &levelData) : Solver(stageData, levelData), count(0) {}
	void run();

private:
	bool canMove(const MapData &route, const Point &o, int i) const;
	void createOnlyGoal();
	Point findGoal();
	vector<vector<int>> createDistMap(const Point &goal);
	void debugPrint(const vector<vector<int>> &map) const;
	void createMaze();
	void upgrade();
	Task createTask(const Point &p);

private:
	int count;
};

bool MazeSolver::canMove(const MapData &route, const Point &o, int i) const
{
	using namespace mark;
	if(dcost[i] == 14)
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

vector<vector<int>> MazeSolver::createDistMap(const Point &goal)
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

		for(int i=0;i<8;++i)
		{
			Point p(o.x+dx[i], o.y+dy[i]);
			if(!canMove(map, o, i)) continue;
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

void MazeSolver::createMaze()
{
	using namespace mark;
	const MapData &map = getMap();
	
	createOnlyGoal();
	const Point goal = findGoal();
	/*
	cout << "goal : ";
	goal.print();
	*/
	vector<vector<int>> dismap(createDistMap(goal));
	
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
		//o.print();

		set<pair<int, Point>> tmp;
		for(int i=0;i<8;i+=2)
		{
			Point p(o.x+dx[i], o.y+dy[i]);
			if(map[p.x][p.y]==GOAL)
			{
				tmp.clear();
				tmp.insert(make_pair(dismap[p.x][p.y], p));
				break;
			}
			if(map[p.x][p.y]!=EMPTY && map[p.x][p.y]!=START) continue;
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
			/*
			cout << " x";
			p.print();
			*/
		}
		que.push(make_pair(d+1, tmp.begin()->second));
		/*
		cout << "=>";
		tmp.begin()->second.print();
		*/
	}
}

void MazeSolver::run()
{
	createMaze();
	int wdt = 0;
	while(true)
	{
		wdt++;
		if(wdt>300) break;

		vector<int> result = simulate();
		if(result.size() == 0) break;
		upgrade();
	}
	vector<int> result = simulate();
	if(!result.empty())
	{
		for(int w=0;w<stage.map.width;++w)
		{
			for(int h=0;h<stage.map.height;++h)
			{
				Task task(createTask(Point(w,h)));
				if(check(task)) build(task);
			}
		}
	}
	wdt = 0;
	while(true)
	{
		wdt++;
		if(wdt>300) break;

		vector<int> result = simulate();
		if(result.size() == 0) break;
		upgrade();
	}
}

void MazeSolver::upgrade()
{
	static std::mt19937 engine( static_cast<unsigned long>(time(0)) );

	/*
	//新規設置
	std::uniform_int_distribution<int> randx( 0, stage.map.width-1 ) ;
	std::uniform_int_distribution<int> randy( 0, stage.map.height-1 ) ;
	for(int i=0;i<10;++i)
	{
		Task task(createTask(Point(randx(engine), randy(engine))));
		if(check(task)) build(task);
	}
	*/

	//アップグレード
	const Towers &towers = getTowers();
	std::uniform_int_distribution<int> randno( 0, towers.size()-1 ) ;
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
			const LevelData levelData = Loader::LoadLevel();
			OldSolver solver(stageData, levelData);
			solver.run();
		}
	}
	for(;s<S; ++s)
	{
		const StageData stageData = Loader::LoadStage();
		for(int l=0; l<stageData.level; ++l)
		{
			const LevelData levelData = Loader::LoadLevel();
			if(l!=0) continue;

			MazeSolver solver(stageData, levelData);
			cout << s+1 << "-" << l+1 << " : ";
			solver.printMap();
			solver.run();
			solver.printMap();
		}
	}
	return 0;
}
