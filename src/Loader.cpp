#include "Loader.h"
#include "Datas.h"
#include <iostream>
#include <vector>
using namespace std;

const StageData Loader::LoadStage()
{
	int W,H;
	cin >> W >> H;
	MapData mapData(W, vector<char>(H));
	Points starts;
	Points goals;
	for(int h=0;h<H;++h)
	{
		for(int w=0;w<W;++w)
		{
			char input;
			cin >> input;
			
			mapData[w][h] = input;
			if(input == mark::START)		starts.push_back(Point(w,h));
			else if(input == mark::GOAL)	goals.push_back(Point(w,h));
		}
	}

	//レベル数
	int L;
	cin >> L;

	//チェック
	string check;
	cin >> check;
	if(check != "END")
	{
		cerr << "LoadStage : check is not END" << endl;
		throw "check is not END";
	}

	return StageData(MapInfo(W, H, mapData, starts, goals), L);
}

const LevelData Loader::LoadLevel(int stage, int level)
{
	//ライフ, 資金, タワーの数, 敵の数
	int Lp, M, T, E;
	cin >> Lp >> M >> T >> E;

	//タワー
	Towers towers;
	towers.reserve(T);
	for(int i=0; i<T; ++i)
	{
		int Xt, Yt, At, Ct;
		cin >> Xt >> Yt >> At >> Ct;
		towers.push_back(Tower(Point(Xt, Yt), At, Ct));
	}

	//エネミー
	Enemies enemies;
	enemies.reserve(E);
	for(int i=0; i<E; ++i)
	{
		int Xe, Ye, Te, Le, Se;
		cin >> Xe >> Ye >> Te >> Le >> Se;
		enemies.push_back(Enemy(Point(Xe, Ye), Te, Le, Se));
	}

	//チェック
	string check;
	cin >> check;
	if(check != "END")
	{
		cerr << "LoadLevel : check is not END" << endl;
		throw "check is not END";
	}

	return LevelData(stage, level, Lp, M, towers, enemies);
}

