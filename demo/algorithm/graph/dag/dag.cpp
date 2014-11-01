#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <string>
#include <utility>
#include "dag.h"

//using ::std::string;
//using ::std::unordered_map;
//using ::std::set;
using ::std::vector;
using ::std::priority_queue;

/*
 * Verify the DAG property using DFS O(V) with time and memory
 * 	WHITE = untouched
 * 	GREY  = pushed to stack, not visited
 * 	BLACK = visited
 * If a BLACK node is visited twice, this is not a DAG
 *
 * FIXME: intrinsically wrong! should use topological ordering (reversed post-order DFS)
 */
bool dag::is_dag()
{
	fprintf(stderr,"dag::is_dag()\n");
	if (_node_list.empty())
		return true;

	::std::stack<dag_node *> s;
	auto itr = _node_list.begin();

	while (1) {
		while (itr != _node_list.end()) {
			if (itr->second->_status == dag_node::WHITE)
				break;
			else
				++itr;
		}
		if (itr == _node_list.end()   &&   s.empty())
			break;
		s.push(itr->second);
		itr->second->_status = dag_node::GREY;
		++itr;
		while (!s.empty()) {
			static int i = 0;
			fprintf(stderr,"i = %d, s->size() = %lu\n",i++,s.size());

			/* edge(u,v) = edge from u to v */
			dag_node *u = s.top();
			s.pop();
			dag_node *v = u->first_white_child();

			u->print_node();

			//if (v)
				//v->print_node();

			if (!v) {
				printf("output u\n");
				u->_status = dag_node::BLACK;
				continue;
			}
			/* Not a DAG is a node is visited twice */
			if (v->_status == dag_node::BLACK)
				goto is_not_dag;
			s.push(v);
			v->_status = dag_node::GREY;
		}
	}

	_bleach();
	return true;
is_not_dag:
	_bleach();
	return false;
}

//priority_queue<dag_node*, vector<dag_node*>,
	//compare_node_order> &&dag::schedule(string &&key)
//{
	//dag_node *dest = get_node(::std::move(key));
	//auto *q = new priority_queue<dag_node*, vector<dag_node*>, compare_node_order>;
	//return ::std::move(*q);
//}
