#include <iostream>
#include <random>
#include <ctime>
#include "Loader.h"
#include "Solver.h"
#include "OldSolver.h"
#include "Simulator.h"
using namespace std;

class MazeSolver : public Solver
{
public:
	MazeSolver(const StageData &stageData, const LevelData &levelData) : Solver(stageData, levelData) {}
	void run();
};

void MazeSolver::run()
{
}

int main()
{
	ios_base::sync_with_stdio(false);

	//ステージ数
	int s,S;
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
			//OldSolver solver(stageData, levelData);
			//solver.run();

			/*
			cout << s+1 << "-" << l+1 << endl;
			solver.printMap();
			*/

			/*
			if(l==0)continue;
			int result = Simulator::getInstance().run(stageData.map, levelData.towers, levelData.enemies);
			cout << s+1 << " - " << l+1 << " => " << result << endl;
			*/
		}
	}
	return 0;
}
