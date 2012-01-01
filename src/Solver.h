#pragma once
#include "Datas.h"
#include "Task.h"

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
	Solver(const StageData &stageData, const LevelData &levelData);
	virtual ~Solver();
	virtual void run() = 0;
	void printMap();

protected:
	MapData createMap(const MapInfo &mapInfo, const Towers &towers);
	bool canGoal(const MapData &fmap);
	bool check(const Task &task);
	bool build(const Task &task);
	const Towers &getTowers() { return m_towers; }
	const MapData &getMap() { return m_map; }
	vector<int> simulate();

protected:
	const StageData stage;
	const LevelData level;

private:
	Wallet wallet;
	TaskList taskList;
	Towers m_towers;
	MapData m_map;
};

