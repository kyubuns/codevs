#include <iostream>
#include "Loader.h"
#include "Solver.h"
using namespace std;

int main()
{
	//ステージ数
	int S;
	cin >> S;
	for(int s=0; s<S; ++s)
	{
		const StageData stageData = Loader::LoadStage();
		
		Solver solver(stageData);
		for(int l=0; l<stageData.level; ++l)
		{
			const LevelData levelData = Loader::LoadLevel();
			solver.run(levelData);
		}
	}
	return 0;
}
