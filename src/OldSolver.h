#include "Solver.h"

class OldSolver : public Solver
{
public:
	OldSolver(const StageData &stageData, const LevelData &levelData) : Solver(stageData, levelData) {}
	void run();
};
