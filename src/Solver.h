#pragma once
#include "Datas.h"
#include "Task.h"

class Wallet
{
public:
	Wallet(int money, int maxMoney);
	int price(const Task &task, const Towers &towers) const;
	bool check(const Task &task, const Towers &towers) const;
	bool buy(const Task &task, Towers *towers);
	void setMaxMoney() { m_money = m_maxMoney; }

private:
	int calcCost(int kind, int oldLevel, int newLevel) const;

private:
	int m_money;
	int m_maxMoney;
};

class Solver
{
public:
	Solver(const StageData &stageData, const LevelData &levelData);
	Solver(const StageData &stageData, const LevelData &levelData, int money);
	virtual ~Solver();
	//virtual void run() = 0;
	void printMap();
	void result();

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

protected:
	Wallet wallet;

private:
	TaskList taskList;
	Towers m_towers;
	MapData m_map;
};

