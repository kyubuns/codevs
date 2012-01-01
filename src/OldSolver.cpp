#include "OldSolver.h"
#include <set>

void OldSolver::run()
{
	using namespace mark;

	//設置
	static const int dx[] = {-1,  0,  1,  1,  1,  0, -1 ,-1};
	static const int dy[] = {-1, -1, -1,  0,  1,  1,  1,  0};
	const MapData &map = getMap();
	set<Point> danger;
	vector<int> before;
	while(true)
	{
		for(auto it = stage.map.starts.begin(), end = stage.map.starts.end(); it != end; ++it)
		{
			//近くにあるか確認
			int towerCount = 0;
			Point p = *it;
			for(int i = 0; i < 8; ++i) if(map[p.x + dx[i]][p.y + dy[i]] == TOWER) towerCount++;
			if(towerCount >= 1 && danger.find(p) == danger.end()) continue;

			//置けるか確認
			Task memo;
			bool canBuild = false;
			for(int i = 0; i < 8; ++i)
			{
				Task task = Task(Point(p.x + dx[i], p.y + dy[i]), 0, 0);
				if(!check(task)) continue;
				memo = task;
				canBuild = true;
				break;
			}
			if(!canBuild) continue;
			build(memo);
		}

		//アップグレード
		const Towers &towers = getTowers();
		for(auto it = towers.begin(); it != towers.end(); ++it)
		{
			const Tower &tower = *it;
			if(tower.level == rule::UPGRADE_MAX_LEVEL) continue;

			Task task(tower);
			task.level = max(rule::UPGRADE_MAX_LEVEL, task.level + 4);
			for(int i=0;i<4;++i)
			{
				if(task.level == tower.level) break;
				if(check(task)) break;
				task.level--;
			}
			if(task.level == tower.level) continue;

			build(task);
			continue;
		}
		vector<int> result = simulate();
		if(result.size()==0) break;
		if(before == result) break;
		before = result;
		danger.clear();
		for(int i=0;i<result.size();++i)
		{
			danger.insert(level.enemies[result[i]].point);
		}
	}
}
