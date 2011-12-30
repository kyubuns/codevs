#include "Datas.h"

class Wallet
{
public:
	Wallet(int money) : m_money(money) {}
	int price(const Task &task, const Towers &towers) const;
	bool check(const Task &task, const Towers &towers) const;
	bool buy(const Task &task, Towers *towers);

private:
	int calcCost(int kind, int oldLevel, int newLevel) const;

private:
	int m_money;
};

class Solver
{
public:
	Solver(const StageData &stageData) : stage(stageData) {}
	void run(const LevelData &levelData);

private:
	MapData createMap(const MapInfo &mapInfo, const Towers &towers);
	bool canGoal(const Towers &towers);

private:
	const StageData stage;
};

