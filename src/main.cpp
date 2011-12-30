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
	void run()
	{
		std::mt19937 engine(static_cast<unsigned long>(time(0)));
		std::uniform_int_distribution<int> randw( 0, stage.map.width-1 ) ;
		std::uniform_int_distribution<int> randh( 0, stage.map.height-1 ) ;

		for ( int i = 0 ; i != 100 ; ++i )
		{
			Point p(randw(engine), randh(engine));
			Task task(p, 0, 0);
			if(!check(task)) continue;

			build(task);
		}

	}
};

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
			solver.run();
		}
	}
	return 0;
}
