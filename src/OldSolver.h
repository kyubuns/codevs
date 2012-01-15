#include "Solver.h"

//第1ステージ(40-25まで)専用
class OldSolver : public Solver
{
public:
	OldSolver(const StageData &stageData, const LevelData &levelData) : Solver(stageData, levelData) {}
	void run();
};
