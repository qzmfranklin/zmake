#include <condition_variable>
#include <mutex>
#include <unique_lock>
#include "task_list.h"
#include "dag.h"

using ::std::unique_lock;
using ::std::mutex;
using ::std::condition_variable;

void task_list::add_node(dag_node *p)
{
	task_node *tmp = new task_node(p);
	if (!_size) {
		_head = tmp;
		_tail = tmp;
	} else {
		_tail->next = tmp;
		tmp->prev = _tail;
		_tail = tmp;
	}
	_size++;
}

void task_list::remove_node(task_node *p)
{
}

void task_list::bring_to_top(task_node *p)
{
}
