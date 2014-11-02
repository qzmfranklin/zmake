#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stack>
#include "dag.h"

void dag_node::print_node() const noexcept
{
	printf("%s",_key.c_str());

	printf(" :");
	for (auto &tmp: _in_list)
		printf(" %s",tmp->_key.c_str());

	printf("      =>");
	for (auto &tmp: _out_list)
		printf(" %s",tmp->_key.c_str());

	printf("\n");
}

dag_node *dag::add_node(string &&key)
{
	dag_node *p = get_node(::std::forward<string>(key));
	if (!p) {
		p = new dag_node(::std::forward<string>(key));
		assert(p);
		_node_list[key] = p;
	}
	return p;
}

void dag::add_edge(string &&from, string &&to)
{
	dag_node *u = add_node(::std::forward<string>(from));
	dag_node *v = add_node(::std::forward<string>(to));
	u->_out_list.insert(v);
	v->_in_list.insert(u);
}

void dag::remove_edge(string &&from, string &&to)
{
	dag_node *u = get_node(::std::forward<string>(from));
	dag_node *v = get_node(::std::forward<string>(to));
	if (!u  ||  !v)
		return;
	u->_out_list.erase(v);
	v->_in_list.erase(u);
}

/*
 * Modified from
 * http://www.inf.ed.ac.uk/teaching/courses/inf2b/algnotes/note10.pdf
 */
bool dag::_dfs_one_node(dag_node *u, ::std::stack<dag_node*> *s)
{
	printf("\n");
	s->push(u);
	u->_status = dag_node::GREY;
	bool retval = true;
	while (!s->empty()) {
		u = s->top();
		s->pop();

		u->print_node();
		u->_status = dag_node::BLACK;

		for (auto &v: u->_out_list)
			if (v->_status == dag_node::WHITE) {
				s->push(v);
				v->_status = dag_node::GREY;
			} else if (v->_status == dag_node::GREY) {
				// FIXME
				printf("LOOP DETECTED ON:\n\t");
				v->print_node();
				retval = false;
			}
	}
	return retval;
}

bool dag::dfs()
{
	//fprintf(stderr,"dag::dfs()\n");
	_bleach();
	::std::stack<dag_node*> s;
	bool retval = true;
	for (auto &u: _node_list)
		if (u.second->_status == dag_node::WHITE) {
			dag_node *v = u.second;
			while (!v->_in_list.empty())
				v = *v->_in_list.begin();
			retval &= _dfs_one_node(v,&s);
		}
	return retval;
}
