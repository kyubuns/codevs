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
		que.push(make_pair(make_pair(0, GOAL), goals[i]));
	}

	while(!que.empty())
	{
		NODE node = que.top(); que.pop();
		int cost = node.first.first;
		int index = node.first.second;
		Point o = node.second;

		if(route[o.x][o.y] != EMPTY && index != GOAL) continue;
		route[o.x][o.y] = index;

		for(int i=0;i<8;++i)
		{
			if(rdir::cost[i] == 3)
			{
				//斜め移動できるか確認
				int n = i+1; if(n>7) n-=8;
				int b = i-1; if(b<0) b+=8;
			
				Point np(o.x + rdir::x[n], o.y + rdir::y[n]);
				Point bp(o.x + rdir::x[b], o.y + rdir::y[b]);
				if(route[np.x][np.y] != EMPTY || route[bp.x][bp.y] != EMPTY) continue;
			}
			Point p(o.x + rdir::x[i], o.y + rdir::y[i]);
			if(route[p.x][p.y] != EMPTY) continue;
			int tmp = 7-(i+4)%8;
			que.push(make_pair(make_pair(cost+rdir::cost[i], tmp), p));
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
			if(route[w][h] > 100) cout << " ";
			else cout << (int)(route[w][h]);
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;

	int time = 0;
	int damage = 0;
	int restEnemy = enemies.size();
	list<ActEnemy> activeEnemies;
	while(restEnemy > 0 || !activeEnemies.empty())
	{
		cout << "time : " << time << endl;
		//• 出現時刻になっていた場合、敵が所定の出現マスに出現します。
		for(int i=0; i<enemies.size(); ++i)
		{
			const Enemy &enemy = enemies[i];
			if(enemy.time != time) continue;

			//出現時間
			restEnemy--;
			activeEnemies.push_back(ActEnemy(enemy));
		}

		//• 敵が移動可能な場合、隣のマスに移動します。
		for(auto it=activeEnemies.begin(), end=activeEnemies.end(); it != end; ++it)
		{
			if(!it->update()) continue;

			Point p = it->getPoint();
			it->move(route[p.x][p.y]);
			cout << "(" << p.x << "," << p.y << ") => (" << it->getPoint().x << "," << it->getPoint().y << ")" << endl;
		}
		
		//• 攻撃可能な状態の場合、タワーと敵のマスのユークリッド距離が射程範囲と等しい、あるい はそれ未満であるマスにいる敵を列挙します
		//• 射程範囲内に敵が複数いる場合、最も早い時刻に射程範囲内に入った敵(一度出て再度入っ た場合は、遅い時間が採用されます)を列挙します。
		//• 同時に入った敵が複数いる場合は、最も最初にマップに出現した敵を列挙します。
		//• 同時に出現した敵が複数いる場合は、標準入力で先に与えられた敵を対象として選びます。
		//todo
		
		//• 全ての攻撃可能なタワーが攻撃対象を選択した後、全てのタワーが一斉に攻撃対象に攻撃力
		//	の値だけのダメージを与えます。
		//todo
		
		//• ライフが 1 以上の敵が防衛マスにたどり着いていた場合 (行動不能時間中である場合も含み ます)、プレイヤーのライフが 1 減り敵が消滅します。
		for(auto it=activeEnemies.begin(); it != activeEnemies.end();)
		{
			Point &p = it->getPoint();
			if(route[p.x][p.y] != mark::GOAL) { ++it; continue; }

			activeEnemies.erase(it++);
			damage++;
			cout << "DAMAGE!! - " << damage << endl;
		}
		
		time++;
	}
}

ActEnemy::ActEnemy(const Enemy &enemy) : data(enemy), counter(0), point()
{
	resetCounter();
	point = data.point;
}

void ActEnemy::resetCounter()
{
	counter = data.speed;
}

//todo 斜め移動
bool ActEnemy::update()
{
	counter--;
	if(counter > 0) return false;
	return true;
}

void ActEnemy::move(int id)
{
	point.x += dir::x[id];
	point.y += dir::y[id];
	resetCounter();
}
