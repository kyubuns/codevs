#include <iostream>
#include <random>
#include <ctime>
#include "Loader.h"
#include "Solver.h"
#include "Simulator.h"
using namespace std;

class OldSolver : public Solver
{
public:
	OldSolver(const StageData &stageData, const LevelData &levelData) : Solver(stageData, levelData) {}
	void run();
};

void OldSolver::run()
{
	using namespace mark;

	//設置
	static const int dx[] = {-1,  0,  1,  1,  1,  0, -1 ,-1};
	static const int dy[] = {-1, -1, -1,  0,  1,  1,  1,  0};
	const MapData &map = getMap();
	int towerMax = 1;
	bool ok = false;
	while(true)
	{
		for(auto it = stage.map.starts.begin(), end = stage.map.starts.end(); it != end; ++it)
		{
			while(true)
			{
				//近くにあるか確認
				int towerCount = 0;
				Point p = *it;
				for(int i = 0; i < 8; ++i) if(map[p.x + dx[i]][p.y + dy[i]] == TOWER) towerCount++;
				if(towerCount >= towerMax) break;

				//置けるか確認
				Task memo;
				bool hoge = false;
				for(int i = 0; i < 8; ++i)
				{
					Task task = Task(Point(p.x + dx[i], p.y + dy[i]), 0, 0);
					if(!check(task)) continue;
					memo = task;
					hoge = true;
					break;
				}
				if(!hoge) break;
				build(memo);
			}
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
		if(towerMax == 8) break;
		if(simulate()==0) break;
		towerMax++;
	}
}

int main()
{
	ios_base::sync_with_stdio(false);

	//ステージ数
	int S;
	cin >> S;
	for(int s=0; s<S; ++s)
	{
		const StageData stageData = Loader::LoadStage();
		for(int l=0; l<stageData.level; ++l)
		{
			const LevelData levelData = Loader::LoadLevel();

			OldSolver solver(stageData, levelData);
			/*
			solver.run();
			*/

			cout << s+1 << "-" << l+1 << endl;
			solver.printMap();
			/*
			if(l==0)continue;
			int result = Simulator::getInstance().run(stageData.map, levelData.towers, levelData.enemies);
			cout << s+1 << " - " << l+1 << " => " << result << endl;
			*/
		}
	}
	return 0;
}
