#include "Solver.h"
#include <random>

class RandSolver : public Solver
{
public:
	RandSolver(const StageData &stageData, const LevelData &levelData, int mapkind) : Solver(stageData, levelData), count(0), mapkind(mapkind) {}
	RandSolver(const StageData &stageData, const LevelData &levelData, int money, int mapkind) : Solver(stageData, levelData, money), count(0), mapkind(mapkind) {}
	pair<int,int> run(bool buildflag);
	int score();

private:
	bool canMove(const MapData &route, const Point &o, int i) const;
	void createOnlyGoal();
	Point findGoal();
	vector<vector<int>> createDistMap(const Point &goal, int kind);
	void debugPrint(const vector<vector<int>> &map) const;
	const Point createMaze();
	int upgrade();
	int upgradeCir(const Point &o, int r);
	Task createTask(const Point &p);

private:
	int count;
	int mapkind;
	static std::mt19937 engine;
};

