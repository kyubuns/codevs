#include "Datas.h"

class Solver
{
public:
	Solver(const StageData &stageData) : stage(stageData) {}
	void run(const LevelData &levelData);

private:
	const StageData stage;
};

