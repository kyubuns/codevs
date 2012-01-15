#include <iostream>
#include <queue>
#include <climits>
#include <set>
#include <random>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "Loader.h"
#include "Solver.h"
#include "OldSolver.h"
#include "Logger.h"
#include "RandSolver.h"
#include "Simulator.h"
using namespace std;
using namespace boost;

void threadMain(RandSolver *solver, pair<int,int> *result, bool buildflag)
{
	*result = solver->run(buildflag);
}

void callOldSolver(int s)
{
	const StageData stageData = Loader::LoadStage();
	for(int l=0; l<stageData.level; ++l)
	{
		const LevelData levelData = Loader::LoadLevel(s+1, l+1);
		OldSolver solver(stageData, levelData);
		solver.run();
		solver.result();
	}
}

void callRandSolver(int s)
{
	int memoMoney = 0;
	int stageKind = 0;
	const StageData stageData = Loader::LoadStage();
	for(int l=0; l<stageData.level; ++l)
	{
		const LevelData levelData = Loader::LoadLevel(s+1, l+1);

		if(l==0)
		{
			array<RandSolver, 2> stageTest =
			{
				RandSolver(stageData, levelData, INT_MAX, 0),
				RandSolver(stageData, levelData, INT_MAX, 1),
			};
			int a = stageTest[0].score();
			Logger::write("----\n");
			int b = stageTest[1].score();
			array<int, 2> score =
			{
				a,b
			};
			if(score[0] < score[1]) stageKind = 1;
			else stageKind = 0;
		}

		array<RandSolver, 6> solvers =
		{
			RandSolver(stageData, levelData, stageKind),
			RandSolver(stageData, levelData, stageKind),
			RandSolver(stageData, levelData, stageKind),
			RandSolver(stageData, levelData, stageKind),
			RandSolver(stageData, levelData, stageKind),
			RandSolver(stageData, levelData, stageKind),
		};
		Logger::write(lexical_cast<string>(s+1) + "-" + lexical_cast<string>(l+1) + "\n");
		Logger::write("Life :" + lexical_cast<string>(levelData.life) + "\n");
		Logger::write("Money:" + lexical_cast<string>(levelData.money) + "\n");

		vector<pair<int,int>> results(solvers.size());
		vector<thread> threads(solvers.size());
		set<pair<pair<int,int>,int>> score;
		int point = Simulator::getInstance().run(stageData.map, levelData.towers, levelData.enemies).size();
		Logger::write(boost::lexical_cast<string>(point) + "," + boost::lexical_cast<string>(0) + "\n");
		if(point==0)
		{
			cout << "0" << endl;
			cout.flush();
			continue;
		}
		threads[0] = thread(bind(threadMain, &solvers[0], &results[0], false));
		threads[1] = thread(bind(threadMain, &solvers[1], &results[1], false));
		threads[2] = thread(bind(threadMain, &solvers[2], &results[2], false));
		threads[3] = thread(bind(threadMain, &solvers[3], &results[3], true));
		threads[4] = thread(bind(threadMain, &solvers[4], &results[4], true));
		threads[5] = thread(bind(threadMain, &solvers[5], &results[5], true));
		score.insert(make_pair(make_pair(point, 0), -1));
		if(point < levelData.life && l+1 >= 20)
		{
			if(s+1 < 75)
				score.insert(make_pair(make_pair(0, point*1500), -1));
			if(s+1 >= 75)
				score.insert(make_pair(make_pair(0, point*500), -1));
		}
		for(int i=0;i<solvers.size();++i)
		{
			threads[i].join();

			auto tmp = make_pair(results[i],i);
			score.insert(tmp);
			Logger::write(boost::lexical_cast<string>(tmp.first.first) + "," + boost::lexical_cast<string>(tmp.first.second) + "\n");
		}
		int no = score.begin()->second;
		if(no==-1)
		{
			cout << "0" << endl;
			cout.flush();
			Logger::write("hoge!\n");
			continue;
		}
		if(score.begin()->first.first) Logger::write("Damage:"+boost::lexical_cast<string>(score.begin()->first.first)+"\n");
		Logger::write("\n");
	}
}

int main()
{
	ios_base::sync_with_stdio(false);

	int s=0,S;
	cin >> S;

	//40ステージ目まではOldSolver
	for(; s<40; ++s) callOldSolver(s);
	
	//以降はRandSolver
	for(;s<S; ++s) callRandSolver(s);

	Logger::out();
	return 0;
}
