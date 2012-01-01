#pragma once
#include "Datas.h"
#include <set>

class ActEnemy
{
public:
	ActEnemy(int id, const Enemy &enemy);
	int update(int time);
	void move(int id);
	bool damage(int point);
	void kill();
	bool isActive() const { return (active && !dead); }
	const Point &getPoint() const { return point; }
	int getId() const { return id; }

public:
	const Enemy data;

private:
	void resetCounter();

private:
	Point point;
	array<int, 2> counter;
	int id;
	int life;
	bool active;
	bool dead;
};

class ActTower
{
public:
	ActTower(int id, const Tower &tower);
	void enterEnemy(int id, int time, const Enemy &data);
	void leaveEnemy(int id);
	const Point &getPoint() const { return data.point; }
	int attack();

	ActTower operator=(const ActTower& tower)
	{
		ActTower t = ActTower(id, data);
		t.counter = counter;
		t.targets = targets;
		return t;
	}
	int getId() const { return id; }

public:
	const Tower data;

private:
	void resetCounter();
	int getTargetId();

private:
	int counter;
	int id;
	set<pair<pair<int,int>,int>> targets;
};

class Simulator
{
public:
	static Simulator &getInstance()
	{
		static Simulator simulator;
		return simulator;
	}

	vector<int> run(const MapInfo &mapInfo, const Towers &towers, const Enemies &enemies);

private:
	bool canMove(const MapData &route, const Point &o, int i) const;
	const MapData createMap(const MapInfo &mapInfo, const Towers &towers) const;
	const MapData createRouteMap(const MapData &map, const vector<Point> &starts, const vector<Point> &goals) const;
};
