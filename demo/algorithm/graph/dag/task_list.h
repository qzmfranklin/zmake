#ifndef _DAG_LIST_H_
#define _DAG_LIST_H_

#include <condition_variable>
#include <mutex>
#include "dag.h"

struct task_node {
	dag_node *task;
	task_node *next;
	task_node *prev;
	::std::mutex mtx;
	::std::condition_variable cond;
	task_node(dag_node *p): task(p), next(NULL), prev(NULL) {}
};

/*
 * Intented only for use in dag:schedule()
 */
class task_list {
private:
	int _size;
	task_node *_curr;
	task_node *_head;
	task_node *_tail;
public:
	task_list(): _size(0), _curr(NULL), _head(NULL), _tail(NULL) {}

	int size() const noexcept { return _size; }
	task_node *head() const noexcept { return _head; }
	task_node *curr() const noexcept { return _curr; }

	/*
	 * Adding node is serial, no need to use mutex
	 */
	void add_node(dag_node *p);

	/*
	 * Removing node is parallel, mutex protect
	 */
	void remove_node(task_node *p);

	/*
	 * Bring a node to the top
	 * Mutex protect
	 */
	void bring_to_top(task_node *p);
};

#endif /* end of include guard */
