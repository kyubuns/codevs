#include "Simulator.h"
#include "Datas.h"
#include <queue>
#include <list>
using namespace std;

namespace dir
{
	static const array<int, 8> x    = {  1,  1,  0, -1, -1, -1,  0,  1};
	static const array<int, 8> y    = {  0, -1, -1, -1,  0,  1,  1,  1};
	static const array<int, 8> cost = {  2,  3,  2,  3,  2,  3,  2,  3};
};

namespace rdir
{
	static const array<int, 8> x    = {  1,  0, -1, -1, -1,  0,  1,  1};
	static const array<int, 8> y    = {  1,  1,  1,  0, -1, -1, -1,  0};
	static const array<int, 8> cost = {  3,  2,  3,  2,  3,  2,  3,  2};
};

const MapData Simulator::createMap(const MapInfo &mapInfo, const Towers &towers) const
{
	MapData map(mapInfo.width, vector<char>(mapInfo.height));
	for(int h=0; h<mapInfo.height; ++h)
		for(int w=0; w<mapInfo.width; ++w)
			map[w][h] = mapInfo.data[w][h];

	for(int i=0; i<towers.size(); ++i) map[towers[i].point.x][towers[i].point.y] = mark::TOWER;

	return map;
}

const MapData Simulator::createRouteMap(const MapData &map, const vector<Point> &starts, const vector<Point> &goals) const
{
	using namespace mark;
	static const char WALL = 'x';

	MapData route(map);
	for(int h=0; h<route[0].size(); ++h)
		for(int w=0; w<route.size(); ++w)
			if(map[w][h] == START) route[w][h] = EMPTY;
			else if(map[w][h] != EMPTY) route[w][h] = WALL;

	//全てのゴールからスタートに向けてたどっていく
	typedef pair<pair<int,int>, Point> NODE;
	priority_queue<NODE, vector<NODE>, greater<NODE>> que;
	for(int i=0; i<goals.size(); ++i)
	{
		que.push(make_pair(make_pair(0, 0), goals[i]));
		route[goals[i].x][goals[i].y] = WALL;
	}

	while(!que.empty())
	{
		NODE node = que.top(); que.pop();
		int cost = node.first.first;
		Point o = node.second;
		cout << "*";
		o.print();

		for(int i=0;i<8;++i)
		{
			Point p(o.x + rdir::x[i], o.y + rdir::y[i]);
			cout << "->";
			p.print();
			if(route[p.x][p.y] != EMPTY) continue;
			route[p.x][p.y] = 7 - (i+4)%8;
			cout << " = " << 7 - (i+4)%8 << endl;
			que.push(make_pair(make_pair(cost+rdir::cost[i], i), p));
		}
	}
	return route;
}

void Simulator::run(const MapInfo &mapInfo, const Towers &towers, const Enemies &enemies)
{
	const MapData map(createMap(mapInfo, towers));
	const MapData route(createRouteMap(map, mapInfo.starts, mapInfo.goals));

	cout << endl;
	int W = mapInfo.width;
	int H = mapInfo.height;
	for(int h=0;h<H;++h)
	{
		for(int w=0;w<W;++w)
		{
			if(map[w][h] != mark::EMPTY) cout << map[w][h];
			else cout << " ";
		}
		cout << endl;
	}

	for(int h=0;h<H;++h)
	{
		for(int w=0;w<W;++w)
		{
			if(route[w][h] < 10)cout << (char)(route[w][h] + '0');
			else cout << route[w][h];
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;

	//• 出現時刻になっていた場合、敵が所定の出現マスに出現します。
	//• 敵が移動可能な場合、隣のマスに移動します。
	//• 全ての攻撃可能なタワーが所定の規則で攻撃対象を決定します。
	//• 全ての攻撃可能なタワーが攻撃を行います。
	//• ライフが 1 以上の敵が防衛マスにたどり着いていた場合 (行動不能時間中である場合も含み ます)、プレイヤーのライフが 1 減り敵が消滅します。
	//• ゲームの終了条件を満たしている場合ゲームを終了します。
	//• 全ての敵が出現し、全ての敵が消滅している場合、クリアして次のレベルに進みます。
}
