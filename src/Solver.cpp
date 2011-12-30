#include "Solver.h"
#include "Task.h"
#include <iostream>
#include <set>
#include <queue>
using namespace std;

//==========Wallet==========
int Wallet::calcCost(int kind, int oldLevel, int newLevel) const
{
	int cost = 0;
	for(int i=oldLevel+1; i<=newLevel; ++i) cost += rule::COST[kind] * (1+i);
	return cost;
}

int Wallet::price(const Task &task, const Towers &towers) const
{
	for(int i=0; i<towers.size(); ++i)
	{
		const Tower &tower = towers[i];
		if(task.point != tower.point) continue;
		if(task.kind != tower.kind) throw "other kind tower";
		
		return calcCost(task.kind, tower.level, task.level);
	}

	return calcCost(task.kind, -1, task.level);
}

bool Wallet::check(const Task &task, const Towers &towers) const
{
	return (m_money >= price(task, towers));
}

bool Wallet::buy(const Task &task, Towers *towers)
{
	int cost = price(task, *towers);
	if(m_money < cost) return false;

	m_money -= cost;
	for(int i=0; i<towers->size(); ++i)
	{
		Tower &tower = (*towers)[i];
		if(task.point != tower.point) continue;

		//立て替え
		tower.level = task.level;
		tower.kind = task.kind;
		return true;
	}
	towers->push_back(Tower(task.point, task.level, task.kind));
	return true;
}

//==========Solver==========
void Solver::run(const LevelData &levelData)
{
	Wallet wallet(levelData.money);
	TaskList taskList;



	taskList.output();
}

MapData Solver::createMap(const MapInfo &mapInfo, const Towers &towers)
{
	MapData map(mapInfo.width, vector<char>(mapInfo.height));
	for(int h=0; h<mapInfo.height; ++h)
		for(int w=0; w<mapInfo.width; ++w)
			map[w][h] = mapInfo.data[w][h];

	for(int i=0; i<towers.size(); ++i) map[towers[i].point.x][towers[i].point.y] = 't';
	
	return map;
}

bool Solver::canGoal(const Towers &towers)
{
	//タワー情報入りのマップデータ作る
	MapData fmap(createMap(stage.map, towers));

	set<Point> checkedStart;
	for(auto it = stage.map.starts.begin(), end = stage.map.starts.end(); it != end; ++it)
	{
		if(checkedStart.find(*it) != checkedStart.end()) continue;

		priority_queue<pair<int, Point>> que;	//距離, ポイント
		MapData map(fmap);
		que.push(make_pair(0, Point(it->x, it->y)));
		bool findGoal = false;
		while(!que.empty())
		{
			int r = que.top().first;
			Point p = que.top().second;
			que.pop();
			
			if(map[p.x][p.y] == 'g')
			{
				findGoal = true;
				break;
			}
			if(map[p.x][p.y] == 's') checkedStart.insert(p);
			if(map[p.x][p.y] != '0' && map[p.x][p.y] != 's') continue;

			map[p.x][p.y] = 'x';
			que.push(make_pair(r-1, Point(p.x, p.y+1)));
			que.push(make_pair(r-1, Point(p.x+1, p.y)));
			que.push(make_pair(r-1, Point(p.x, p.y-1)));
			que.push(make_pair(r-1, Point(p.x-1, p.y)));
		}
		if(findGoal == false) return false;
	}
	return true;
}
