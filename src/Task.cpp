#include "Task.h"
#include "Datas.h"
#include <iostream>
#include <vector>
using namespace std;

void TaskList::addTask(const Task &task)
{
	m_tasks.push_back(task);
}

void TaskList::output()
{
	cout << m_tasks.size() << endl;
	for(auto it=m_tasks.begin(), end=m_tasks.end(); it!=end; ++it)
	{
		const Task &task = *it;
		cout << task.point.x << " " << task.point.y << " " << task.level << " " << task.kind << endl;
	}
}

