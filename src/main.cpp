#include <iostream>
#include <random>
#include <ctime>
#include "Loader.h"
#include "Solver.h"
using namespace std;

class MainSolver : public Solver
{
public:
	MainSolver(const StageData &stageData, const LevelData &levelData) : Solver(stageData, levelData) {}
	void run();

private:
	static const int BUILD_TOWER = 1;
	static const int CHECK_BUILD_HP = 4;
	static const int CHECK_BUILD_TOWER = 1;

	static const int UPGRADE_TOWER = 4;
	static const int CHECK_UPGRADE_HP = 2;
	static const int CHECK_UPGRADE_TOWER = 4;
};

void MainSolver::run()
{
	//設置
	static const int dx[] = {-1,  0,  1,  1,  1,  0, -1 ,-1};
	static const int dy[] = {-1, -1, -1,  0,  1,  1,  1,  0};
	const MapData &map = getMap();
	for(auto it = stage.map.starts.begin(), end = stage.map.starts.end(); it != end; ++it)
	{
		while(true)
		{
			//近くにあるか確認
			int towerCount = 0;
			Point p = *it;
			for(int i = 0; i < 8; ++i) if(map[p.x + dx[i]][p.y + dy[i]] == 't') towerCount++;
			if(towerCount >= CHECK_BUILD_TOWER || (level.life >= CHECK_BUILD_HP && towerCount >= BUILD_TOWER)) break;

			//無いので設置
			//--置けるか確認
			Point memo;//todo:optional<Task>にする
			for(int i = 0; i < 8; ++i)
			{
				Task task = Task(Point(p.x + dx[i], p.y + dy[i]), 0, 0);
				if(!check(task)) continue;
				memo = Point(p.x + dx[i], p.y + dy[i]);
				break;
			}
			if(memo == Point(0,0)) break;

			const int kind = 0;
			build(Task(memo, 0, kind));
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
	cout.flush();
}

int main()
{
	//ステージ数
	int S;
	cin >> S;
	for(int s=0; s<S; ++s)
	{
		const StageData stageData = Loader::LoadStage();
		for(int l=0; l<stageData.level; ++l)
		{
			const LevelData levelData = Loader::LoadLevel();
			MainSolver solver(stageData, levelData);
			try{
				solver.run();
			}
			catch(string mes)
			{
				cout << "ERROR : " << mes << endl;
			}
		}
	}
	return 0;
}
