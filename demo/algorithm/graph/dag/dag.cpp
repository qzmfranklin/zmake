#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stack>
#include <map>
#include <unordered_map>
#include <string>
#include "dag.h"

/*
 * Verify the DAG property using DFS O(V) with time and memory
 * 	WHITE = untouched
 * 	GREY  = pushed to stack, not visited
 * 	BLACK = visited
 * If a BLACK node is visited twice, this is not a DAG
 */
bool dag::is_dag()
{
	//fprintf(stderr,"dag::is_dag()\n");
	if (_node_list.empty())
		return true;

	::std::stack<dag_node *> s;
	auto itr = _node_list.begin();

	while (1) {
		/*
		 * Find a white node (later, u) to start with. If all nodes are
		 * black, search is done
		 */
		while (itr != _node_list.end()) {
			if (itr->second->_status == dag_node::WHITE)
				break;
			else
				++itr;
		}
		if (itr == _node_list.end())
			break;
		s.push(itr->second);
		itr->second->_status = dag_node::GREY;
		++itr;
		while (!s.empty()) {
			/* edge(u,v) = edge from u to v */
			dag_node *u = s.top();
			s.pop();
			dag_node *v = u->first_white_child();
			if (!v) {
				u->_status = dag_node::BLACK;
				continue;
			}
			/* Not a DAG is a node is visited twice */
			if (v->_status == dag_node::BLACK)
				return false;
			s.push(v);
			v->_status = dag_node::GREY;
		}
	}

	return true;
}

priority_queue<dag_node*, vector<dag_node*>,
	compare_node_priority> &&dag::schedule(const string &key)
{
}
