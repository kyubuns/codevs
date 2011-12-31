#pragma once
#include "Datas.h"

class ActEnemy
{
public:
	ActEnemy(const Enemy &enemy);
	bool update();
	void move(int id);
	Point &getPoint() { return point; }

private:
	void resetCounter();

private:
	Point point;
	int counter;
	const Enemy data;
};

class Simulator
{
public:
	static Simulator &getInstance()
	{
		static Simulator simulator;
		return simulator;
	}

	void run(const MapInfo &mapInfo, const Towers &towers, const Enemies &enemies);

private:
	const MapData createMap(const MapInfo &mapInfo, const Towers &towers) const;
	const MapData createRouteMap(const MapData &map, const vector<Point> &starts, const vector<Point> &goals) const;
};
