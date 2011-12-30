#include <vector>
#include <list>
using namespace std;

struct Task;
class TaskList
{
public:
	void addTask(const Task &task);
	void output();

private:
	list<Task> m_tasks;
};
