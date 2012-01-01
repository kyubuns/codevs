#include "Simulator.h"
#include "Datas.h"
#include <queue>
#include <list>
using namespace std;

const static bool DEBUG = false;

namespace dir
{
	static const array<int, 8> x    = {{  1,  1,  0, -1, -1, -1,  0,  1}};
	static const array<int, 8> y    = {{  0, -1, -1, -1,  0,  1,  1,  1}};
	static const array<int, 8> cost = {{  2,  3,  2,  3,  2,  3,  2,  3}};
};

namespace rdir
{
	static const array<int, 8> x    = {{  1,  0, -1, -1, -1,  0,  1,  1}};
	static const array<int, 8> y    = {{  1,  1,  1,  0, -1, -1, -1,  0}};
	static const array<int, 8> cost = {{  3,  2,  3,  2,  3,  2,  3,  2}};
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

bool Simulator::canMove(const MapData &route, const Point &o, int i) const
{
	using namespace mark;
	if(rdir::cost[i] == 3)
	{
		//斜め移動できるか確認
		int n = i+1; if(n>7) n-=8;
		int b = i-1; if(b<0) b+=8;

		Point np(o.x + rdir::x[n], o.y + rdir::y[n]);
		Point bp(o.x + rdir::x[b], o.y + rdir::y[b]);
		if(route[np.x][np.y] != EMPTY || route[bp.x][bp.y] != EMPTY) return false;
	}
	Point p(o.x + rdir::x[i], o.y + rdir::y[i]);
	if(route[p.x][p.y] != EMPTY) return false;
	return true;
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
	for(int i=0; i<goals.size(); ++i) que.push(make_pair(make_pair(0, GOAL), goals[i]));

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
			if(!canMove(route, o, i)) continue;
			int tmp = 7-(i+4)%8;
			Point p(o.x + rdir::x[i], o.y + rdir::y[i]);
			que.push(make_pair(make_pair(cost+rdir::cost[i], tmp), p));
		}
	}
	return route;
}

vector<int> Simulator::run(const MapInfo &mapInfo, const Towers &towers, const Enemies &enemies)
{
	const MapData map(createMap(mapInfo, towers));
	const MapData route(createRouteMap(map, mapInfo.starts, mapInfo.goals));

	if(DEBUG)
	{
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
				if(route[w][h] > 10) cout << " ";
				else cout << (int)(route[w][h]);
				cout << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	int time = 0;
	int restEnemy = enemies.size();
	vector<int> damage;

	vector<ActEnemy> actEnemies;
	actEnemies.reserve(enemies.size());
	for(int i=0;i<enemies.size();++i) actEnemies.push_back(ActEnemy(i, enemies[i]));

	vector<ActTower> actTowers;
	actTowers.reserve(towers.size());
	for(int i=0;i<towers.size();++i) actTowers.push_back(ActTower(i, towers[i]));

	while(restEnemy > 0)
	{
		if(DEBUG) cout << endl;
		if(DEBUG) cout << "===== time : " << time << "=====" << endl;

		list<int> moveEnemies;
		list<int> deadEnemy;
		for(int i=0; i<actEnemies.size(); ++i)
		{
			//• 出現時刻になっていた場合、敵が所定の出現マスに出現します。
			//• 敵が移動可能な場合、隣のマスに移動します。
			ActEnemy &enemy = actEnemies[i];
			const Point &p = enemy.getPoint();
			const Point copy = enemy.getPoint();
			int result = enemy.update(time);
			if(result == 4)
			{
				//born
				moveEnemies.push_back(enemy.getId());
				continue;
			}
			if(result == -1) continue;//dead
			if(result < dir::cost[route[p.x][p.y]]) continue;

			enemy.move(route[p.x][p.y]);
			moveEnemies.push_back(enemy.getId());
			if(DEBUG) cout << "ENEMY" << enemy.getId() << " - MOVE(" << copy.x << "," << copy.y << ") => (";
			if(DEBUG) cout << p.x << "," << p.y << ")" << endl;
		}
		
		list<pair<int, int>> attackEnemy;
		for(int i=0;i<actTowers.size();++i)
		{
			ActTower &tower = actTowers[i];
			//• タワーと敵のマスのユークリッド距離が射程範囲と等しい、あるい はそれ未満であるマスにいる敵を列挙します
			//• 射程範囲内に敵が複数いる場合、最も早い時刻に射程範囲内に入った敵(一度出て再度入っ た場合は、遅い時間が採用されます)を列挙します。
			//• 同時に入った敵が複数いる場合は、最も最初にマップに出現した敵を列挙します。
			//• 同時に出現した敵が複数いる場合は、標準入力で先に与えられた敵を対象として選びます。
			int r = tower.data.getR();
			r = r*r;
			for(auto it=moveEnemies.begin(); it != moveEnemies.end(); ++it)
			{
				int enemyId = *it;
				ActEnemy &enemy = actEnemies[enemyId];

				int tmpX = enemy.getPoint().x - tower.getPoint().x;
				int tmpY = enemy.getPoint().y - tower.getPoint().y;
				int tmpR = tmpX*tmpX + tmpY*tmpY;
				if(r >= tmpR) tower.enterEnemy(enemyId, time, enemy.data);
				else tower.leaveEnemy(enemyId);
			}

			int enemyId = tower.attack();
			if(enemyId == -1) continue;

			if(DEBUG) cout << "TOWER" << tower.getId() << " Ready" << endl;
			if(DEBUG) cout << "     => Attack" << endl;
			attackEnemy.push_back(make_pair(enemyId, i));
		}
		
		//• 全ての攻撃可能なタワーが攻撃対象を選択した後、全てのタワーが一斉に攻撃対象に攻撃力
		//	の値だけのダメージを与えます。
		for(auto it=attackEnemy.begin(), end=attackEnemy.end(); it != end; ++it)
		{
			int enemyId = it->first;
			ActTower &tower = actTowers[it->second];
			ActEnemy &enemy = actEnemies[enemyId];

			if(!enemy.isActive()) continue;
			if(enemy.damage(tower.data))
			{
				deadEnemy.push_back(enemyId);
				restEnemy--;
			}
		}
		
		//• ライフが 1 以上の敵が防衛マスにたどり着いていた場合 (行動不能時間中である場合も含み ます)、プレイヤーのライフが 1 減り敵が消滅します。
		for(auto it=moveEnemies.begin(); it != moveEnemies.end(); ++it)
		{
			int enemyId = *it;
			ActEnemy &enemy = actEnemies[enemyId];
			if(!enemy.isActive()) continue;

			const Point &p = enemy.getPoint();
			if(route[p.x][p.y] == mark::GOAL)
			{
				enemy.kill();
				deadEnemy.push_back(enemyId);
				restEnemy--;
				damage.push_back(enemyId);
				if(DEBUG) cout << "DAMAGE!! - " << enemyId << "("<< damage.size() << ")" << endl;
			}
		}
		
		for(auto it=deadEnemy.begin(); it != deadEnemy.end(); ++it)
		{
			for(int i=0;i<actTowers.size();++i)
			{
				ActTower &tower = actTowers[i];
				tower.leaveEnemy(*it);
			}
		}

		time++;
	}
	return damage;
}

ActEnemy::ActEnemy(int id, const Enemy &enemy) : data(enemy), point(), id(id), active(false), dead(false)
{
	point = data.point;
	life = data.life;
	resetCounter();
}

void ActEnemy::resetCounter()
{
	counter[0] = data.speed;
	counter[1] = data.speed * 14 / 10;
}

int ActEnemy::update(int time)
{
	//• 出現時刻になっていた場合、敵が所定の出現マスに出現します。
	//• 敵が移動可能な場合、隣のマスに移動します。
	bool bornFlag = false;
	if(time == data.time)
	{
		if(DEBUG) cout << "Add Enemy : " << getId() << endl;
		if(DEBUG) cout << "    Point : " << "(" << getPoint().x << "," << getPoint().y << ")" << endl;
		if(DEBUG) cout << "    Life  : " << life << endl;
		if(DEBUG) cout << "    Speed : " << data.speed << endl;
		active = true;
		bornFlag = true;
	}
	if(!isActive()) return -1;

	counter[0]--;
	counter[1]--;
	if(bornFlag)       return 4;
	if(counter[0] > 0) return 1;
	if(counter[1] > 0) return 2;
	return 3;
}

void ActEnemy::move(int id)
{
	point.x += dir::x[id];
	point.y += dir::y[id];
	resetCounter();
}

bool ActEnemy::damage(const Tower &data)
{
	if(DEBUG) cout << "    ENEMY" << getId() << " - Life : " << life;
	life -= data.getPower();
	if(DEBUG) cout << " => " << life << endl;

	//フリーズ処理
	if(data.kind == 2)
	{
		int tmp = (data.getSpeed()/10);
		counter[0] += tmp;
		counter[1] += tmp;
	}

	if(life < 0)
	{
		kill();
		return true;
	}
	return false;
}

void ActEnemy::kill()
{
	dead = true;
	if(DEBUG) cout << "ENEMY" << getId() << " - DEAD!" << endl;
}

ActTower::ActTower(int id, const Tower &tower) : data(tower), counter(0), id(id)
{
	if(DEBUG) cout << "Add Tower : " << id << endl;
	if(DEBUG) cout << "    Point : " << "(" << getPoint().x << "," << getPoint().y << ")" << endl;
	if(DEBUG) cout << "    Power : " << data.getPower() << endl;
	if(DEBUG) cout << "    Speed : " << data.getSpeed() << endl;
	if(DEBUG) cout << "    R     : " << data.getR() << endl;
}

void ActTower::resetCounter()
{
	counter = data.getSpeed() + 1;
}

void ActTower::enterEnemy(int id, int time, const Enemy &data)
{
	//• タワーと敵のマスのユークリッド距離が射程範囲と等しい、あるい はそれ未満であるマスにいる敵を列挙します
	//• 射程範囲内に敵が複数いる場合、最も早い時刻に射程範囲内に入った敵(一度出て再度入っ た場合は、遅い時間が採用されます)を列挙します。
	//• 同時に入った敵が複数いる場合は、最も最初にマップに出現した敵を列挙します。
	//• 同時に出現した敵が複数いる場合は、標準入力で先に与えられた敵を対象として選びます。
	for(auto it=targets.begin(), end=targets.end(); it!=end; ++it)
	{
		//既にリストにいたらスルー
		if(it->second == id) return;
	}

	targets.insert(make_pair(make_pair(time, data.time), id));
}

void ActTower::leaveEnemy(int id)
{
	for(auto it=targets.begin(), end=targets.end(); it!=end; ++it)
	{
		if(it->second != id) continue;
		targets.erase(it);
		return;
	}
}

int ActTower::getTargetId()
{
	if(targets.empty()) return -1;
	return targets.begin()->second;
}

int ActTower::attack()
{
	counter--;
	if(counter > 0) return -1;

	//cout << "TOWER" << id << " Ready" << endl;
	int result = getTargetId();
	if(result != -1) resetCounter();
	return result;
}

