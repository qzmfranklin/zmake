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

void dag_node::print_node_debug() const noexcept
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
 * Only use BLACK(visited) and WHITE(unvisited)
 */
static void _dag_dfs_from_node(dag_node *u, ::std::stack<dag_node*> *s)
{
	printf("\n");
	s->push(u);
	u->_status = dag_node::BLACK;
	while (!s->empty()) {
		u = s->top();
		s->pop();
		u->print_node();
		for (auto &v: u->_out_list)
			if (v->_status == dag_node::WHITE) {
				s->push(v);
				v->_status = dag_node::BLACK;
			}
	}
}
void dag::dfs()
{
	//fprintf(stderr,"dag::dfs()\n");
	_bleach();
	::std::stack<dag_node*> s;
	for (auto &u: _node_list)
		if (u.second->_status == dag_node::WHITE) {
			dag_node *v = _source_of(u.second);
			_dag_dfs_from_node(v,&s);
		}
}

static dag_node *_first_grey_child(dag_node *p)
{
	if (p->_out_list.empty())
		return NULL;

	auto &tmp = p->_out_itr;
	while (tmp != p->_out_list.end())
		if ((*tmp)->_status == dag_node::GREY)
			break;
		else
			++tmp;

	return (tmp == p->_out_list.end())  ?  NULL  :  *tmp;
}

static dag_node *_stack_peek(::std::stack<dag_node*> *s)
{
	return (s->empty())  ?  NULL  :  s->top();
}

//const dag_node *tmp = _first_non_black(v);

static dag_node *_first_non_black_child(dag_node *p)
{
	auto &tmp = p->_out_itr;
	while (tmp != p->_out_list.end())
		if ((*tmp)->_status != dag_node::BLACK)
			break;
		else
			++tmp;
	return *tmp;
}

static bool _is_dag_first_while(dag_node *p,
		::std::stack<dag_node*> *s1,
		::std::stack<dag_node*> *s2)
{
	if (!p)
		return true;
	while (!p->_out_list.empty()) {
		s1->push(p);
		p->_status = dag_node::GREY;
		/*
		 * Push the first non-visited child of p to s1.
		 * Note that if any child of p is GREY, the
		 * graph is cyclic.
		 */
		dag_node *tmp = _first_non_black_child(p);
		if (!tmp)
			break;
		if (tmp->_status == dag_node::GREY)
			return false;
		else if (tmp->_status == dag_node::WHITE)
			p = tmp;
		/*
		 * Push all but the first non-visited children
		 * to s2. Also watch out for GREY children.
		 */
		for (auto &v = ++(p->_out_itr);
				v != tmp->_out_list.end(); ++v)
			if ((*v)->_status == dag_node::WHITE)
				s2->push(*v);
			else if ((*v)->_status == dag_node::GREY)
				return false;
	}
	return true;
}

/*
 * FIXME: not working...
 * Use two stacks s1 and s2 to do the DFS. At any time, s1 is the same as the
 * stack one sees in the naive recursive implementation. s1 is always GREY, s2
 * is always WHITE.
 */
static bool _from_node_stack2(dag_node *p)
{
	::std::stack<dag_node*> s1, s2;

	while (1) {
		if (!_is_dag_first_while(p,&s1,&s2))
			return false;

		/*
		 * s2 cannot be empty without first emptying s1
		 */
		if (s1.empty())
			break;

		p = s1.top();
		s1.pop();
		dag_node *tmp = _first_grey_child(p);
		/*
		 * If p has at a GREY child that is on top of s2, push that
		 * child to s1 and pop it from s2
		 */
		if (tmp  &&  tmp == _stack_peek(&s2)) {
			s1.push(p);
			p = s2.top();
			s2.pop();
		} else {
			p->print_node();
			p->_status = dag_node::BLACK;
			p = NULL;
		}
	}

	return true;
}

static bool _from_node_recursive(dag_node *u)
{
	bool retval = true;
	u->_status = dag_node::GREY;
	for (auto &v: u->_out_list) {
		if (v->_status == dag_node::WHITE)
			retval &= _from_node_recursive(v);
		else if (v->_status == dag_node::GREY)
			retval = false;
	}
	u->_status = dag_node::BLACK;
	return retval;
}

static bool _from_node(dag_node *p)
{
	return _from_node_stack2(p);
	//return _from_node_recursive(p);
}

bool dag::is_dag()
{
	_bleach();
	_reset_iterators();

	/*
	 * Find a white node. Locate node's root source p. Do a DFS from p.
	 * If node is white, it cannot have any black ancestor.
	 * Otherwise it would have been visited in a previous iteration.
	 */
	for (auto &u: _node_list)
		if (u.second->_status == dag_node::WHITE) {
			dag_node *p = _source_of(u.second);
			if (_from_node(p) == false)
				return false;
		}

	return true;
}

void dag::_bleach() noexcept
{
	for (auto &node: _node_list)
		node.second->_status = dag_node::WHITE;
}

void dag::_reset_iterators() noexcept
{
	for (auto &node: _node_list) {
		dag_node *p = node.second;
		p->_in_itr  = p->_in_list.begin();
		p->_out_itr = p->_out_list.begin();
	}
}

dag_node *dag::_source_of(dag_node *p) const noexcept
{
	dag_node *retval = p;
	while (!retval->_in_list.empty())
		retval = *(retval->_in_list.begin());
	return retval;
}
