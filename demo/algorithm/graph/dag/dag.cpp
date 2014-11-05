#include <stdio.h>
#include <unordered_set>
#include <stack>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <assert.h>
#include "dag.h"

void dag_node::update() const noexcept
{
	if (_recipe.empty())
		return;
	printf("UPDATE %s:    %s\n", _key.c_str(), _recipe.c_str());
}

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
	fprintf(stderr,"%s(%s)",_key.c_str(),
			status_string().c_str());

	fprintf(stderr," :");
	for (auto &tmp: _in_list)
		fprintf(stderr," %s(%s)",tmp->_key.c_str(),
				tmp->status_string().c_str());

	fprintf(stderr,"      =>");
	for (auto &tmp: _out_list)
		fprintf(stderr," %s(%s)",tmp->_key.c_str(),
				tmp->status_string().c_str());

	fprintf(stderr,"\n");

	if (_recipe.empty())
		return;
	printf("\t%s\n",_recipe.c_str());
}

string &&dag_node::status_string() const noexcept
{
	::std::string *retval = new ::std::string;
	switch (_status) {
	case WHITE:
		*retval = "WHITE";
		break;
	case GREY:
		*retval = "GREY";
		break;
	case BLACK:
		*retval = "BLACK";
		break;
	case WHITE | OUTDATED:
		*retval = "WHITE | OUTDATED";
		break;
	case GREY | OUTDATED:
		*retval = "GREY | OUTDATED";
		break;
	case BLACK | OUTDATED:
		*retval = "BLACK | OUTDATED";
		break;
	default:
		*retval = "UNKNOWN STATUS";
		break;
	}
	return ::std::move(*retval);
}

dag_node *dag_node::source_of() noexcept
{
	::std::unordered_set<dag_node*> s;
	s.insert(this);
	dag_node *retval = this;
	while (!retval->_in_list.empty()) {
		dag_node *tmp = *(retval->_in_list.begin());
		if (s.count(tmp))
			return this;
		else {
			s.insert(retval);
			retval = tmp;
		}
	}
	return retval;
}

dag_node *dag_node::sink_of() noexcept
{
	::std::unordered_set<dag_node*> s;
	s.insert(this);
	dag_node *retval = this;
	while (!retval->_out_list.empty()) {
		dag_node *tmp = *(retval->_out_list.begin());
		if (s.count(tmp))
			return this;
		else {
			s.insert(retval);
			retval = tmp;
		}
	}
	return retval;
}

dag_node *dag_node::first_white_parent() const noexcept
{
	dag_node *retval = NULL;
	for (auto &&v_itr: _in_list)
		if (v_itr->_status == WHITE) {
			retval = &(*v_itr);
			break;
		}
	return retval;
}

dag_node *dag_node::first_white_child() const noexcept
{
	dag_node *retval = NULL;
	for (auto &&v_itr: _out_list)
		if (v_itr->_status == WHITE) {
			retval = &(*v_itr);
			break;
		}
	return retval;
}

dag_node *dag_node::first_grey_child() const noexcept
{
	dag_node *retval = NULL;
	for (auto &&v_itr: _out_list)
		if (v_itr->_status == GREY) {
			retval = &(*v_itr);
			break;
		}
	return retval;
}

dag_node *dag_node::first_non_black_child() const noexcept
{
	dag_node *retval = NULL;
	for (auto &&v_itr: _out_list)
		if (v_itr->_status != BLACK) {
			retval = &(*v_itr);
			break;
		}
	return retval;
}

dag_node *dag::get_node(string &&key)
{
	return _node_list.count(::std::forward<string>(key))
		? _node_list[::std::forward<string>(key)] : NULL;
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

void dag::remove_node(string &&key)
{
	dag_node *p = get_node(::std::forward<string>(key));
	if (!p)
		return;
	for (auto &parent: p->_in_list)
		parent->_out_list.erase(p);
	for (auto &child: p->_out_list)
		child->_in_list.erase(p);
	_node_list.erase(::std::forward<string>(key));
	delete p;
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

void dag::set_recipe(string &&key, string &&recipe)
{
	dag_node *p = get_node(::std::forward<string>(key));
	if (!p)
		return;
	p->_recipe = ::std::move(recipe);
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
			dag_node *v = u.second->source_of();
			_dag_dfs_from_node(v,&s);
		}
}

static dag_node *_stack_peek(::std::stack<dag_node*> *s)
{
	return (s->empty())  ?  NULL  :  s->top();
}

static bool _is_dag_first_while(dag_node *p,
		::std::stack<dag_node*> *s1,
		::std::stack<dag_node*> *s2)
{
	if (!p)
		return true;
	while (!p->_out_list.empty()) {
		//fprintf(stderr,"\n");
		//fprintf(stderr,"PUSH\t");
		//p->print_node_debug();
		s1->push(p);
		p->_status = dag_node::GREY;
		/*
		 * Push the first non-black child of p to s1.
		 * Note that if any child of p is GREY, the
		 * graph is cyclic.
		 */
		/*
		 * Push all but the first non-visited children
		 * to s2. Also watch out for GREY children.
		 */
		for (auto &v: p->_out_list) {
			if (v == *(p->_out_list.begin()))
				continue;
			//v->print_node_debug();
			if (v->_status == dag_node::WHITE)
				s2->push(&(*v));
			else if (v->_status == dag_node::GREY) {
				fprintf(stderr,"LOOP DETECTED\n");
				v->print_node_debug();
				return false;
			}
		}
		dag_node *tmp = p->first_non_black_child();
		if (!tmp)
			break;
		//fprintf(stderr,"FIRST NON-BLACK CHILD\n\t\t");
		//tmp->print_node_debug();
		if (tmp->_status == dag_node::WHITE)
			p = tmp;
		else if (tmp->_status == dag_node::GREY) {
			fprintf(stderr,"LOOP DETECTED\n");
			tmp->print_node_debug();
			return false;
		}
	}
	return true;
}

/*
 * Use two stacks s1 and s2 to do the DFS. At any time, s1 is the same as the
 * stack one sees in the naive recursive implementation. s1 is always GREY, s2
 * is always WHITE.
 */
static bool _from_node_stack2(dag_node *p)
{
	//::std::stack<dag_node*> ;
	auto s1 = new ::std::stack<dag_node*>;
	auto s2 = new ::std::stack<dag_node*>;
	while (1) {
		if (!_is_dag_first_while(p,s1,s2)) {
			delete s1;
			delete s2;
			return false;
		}

		if (s1->empty())
			break;

		p = s1->top();
		s1->pop();
		dag_node *tmp = p->first_grey_child();
		/*
		 * If p has a GREY child that is on top of s2, push that
		 * child to s1 and pop it from s2
		 */
		if (tmp  &&  tmp == _stack_peek(s2)) {
			s1->push(p);
			p = s2->top();
			s2->pop();
		} else {
			//p->print_node_debug();
			p->_status = dag_node::BLACK;
			p = NULL;
		}
	}

	delete s1;
	delete s2;
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

	/*
	 * Find a white node. Locate node's root source p. Do a DFS from p.
	 * If node is white, it cannot have any black ancestor.
	 * Otherwise it would have been visited in a previous iteration.
	 */
	for (auto &u: _node_list)
		if (u.second->_status == dag_node::WHITE) {
			dag_node *p = u.second->source_of();
			if (_from_node(p) == false)
				return false;
		}

	return true;
}

/*
 * dag::schedule() assumes that the graph is a DAG but does not check it. The
 * user should call dag::is_dag() before calling dag::schedule(). Failing to do
 * so might lead to undefined behavior.
 *
 * Impelmentation details:
 *
 * This is a two-pass implementation. The first pass does a post-order DFS and
 * pushes the nodes to a queue (q). The second pass reads q and invoke recipes
 * in the proper order.
 *
 * If the DAG is free of forward edges (edges that point to an indirect
 * ancestor), recipes can be invoked in the order that they appear in q. But if
 * there is any forward edges in the DAG, an extra validity check must be
 * performed. If the node (p) in front of q has any outdated (WHITE) parent,
 * then p is pushed to the end of q, awaiting updates on its parents.
 *
 * This is not the fast implementation of all times. But it works just fine.
 */
void dag::schedule(string &&key)
{
	dag_node *p = get_node(::std::forward<string>(key));
	if (!p)
		return;

	_bleach();
	auto *s = new ::std::stack<dag_node*>;
	auto *q = new ::std::queue<dag_node*>;

	s->push(p);
	p->_status = dag_node::BLACK;
	while (1) {
		if (p)
			for (auto &u: p->_in_list)
				if (u->_status == dag_node::WHITE) {
					s->push(&(*u));
					u->_status = dag_node::BLACK;
				}
		if (s->empty())
			break;
		p = s->top();
		dag_node *tmp = p->first_white_parent();
		if (!tmp) {
			s->pop();
			q->push(p);
			p = NULL;
		}
	}

	_bleach();
	while (!q->empty()) {
		p = q->front();
		q->pop();
		dag_node *tmp = p->first_white_parent();
		if (!tmp) {
			p->update();
			//p->print_node_debug();
			p->_status = dag_node::BLACK;
		} else {
			q->push(p);
		}
	}

	delete q;
	delete s;
}

void dag::_bleach() noexcept
{
	for (auto &node: _node_list)
		node.second->_status = dag_node::WHITE;
}
