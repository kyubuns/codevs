#pragma once
#include "Datas.h"

class Simulator
{
public:
	static Simulator &getInstance()
	{
		static Simulator simulator;
		return simulator;
	}

	void run(const MapInfo &mapInfo, const Towers &towers, const Enemies &enemies);
};
