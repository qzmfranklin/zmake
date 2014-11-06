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

	/*
	 *if (_recipe.empty())
	 *        return;
	 *printf("\t%s\n",_recipe.c_str());
	 */
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

bool dag_node::has_grey_child(const dag_node *p) const noexcept
{
	if (!p)
		return false;
	for (auto &u: _out_list)
		if (u->_status == GREY)
			if (&(*u) == p)
				return true;
	return false;
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

/*
 * Use two stacks s1 and s2 to do the DFS. At any time, s1 is the same as the
 * stack one sees in the naive recursive implementation. s1 is always GREY, s2
 * is always WHITE.
 *
 * This is an POST-ORDER traversal
 */
static bool _from_node_stack2(dag_node *p)
{
	auto s = new ::std::stack<dag_node*>;
	while (1) {
		while (p) {
			s->push(p);
			p->_status = dag_node::GREY;
			for (auto &u: p->_out_list)
				if (u->_status == dag_node::GREY) {
					fprintf(stderr,"LOOP DETECTED\n");
					p->print_node_debug();
					goto fail_exit;
				}
			p = p->first_white_child();
		}

		if (s->empty())
			break;

		p = s->top();
		dag_node *tmp = p->first_white_child();
		if (tmp) {
			p = tmp;
		} else {
			s->pop();
			p->_status = dag_node::BLACK;
			p = NULL;
		}
	}
	delete s;
	return true;

fail_exit:
	delete s;
	return false;
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
 * Helper routine for dag::schedule()
 *
 * The similar helper routine for dag::is_dag() is _is_dag_first_while()
 */
static bool _schedule_first_while(dag_node *p,
		::std::stack<dag_node*> *s1,
		::std::stack<dag_node*> *s2)
{
	if (!p)
		return true;
	while (!p->_in_list.empty()) {
		s1->push(p);
		p->_status = dag_node::GREY;
		int num_white_children = 0;
		for (auto &v: p->_in_list) {
			if (v->_status == dag_node::WHITE) {
				if (num_white_children++)
					s2->push(&(*v));
				else {
					p = &(*v);
					break;
				}
			} else if (v->_status == dag_node::GREY) {
				fprintf(stderr,"LOOP DETECTED\n");
				v->print_node_debug();
				return false;
			}
		}
		if (!num_white_children)
			break;
	}
	return true;
}

/*
 * Post order traversal of this DAG from the target point. When a node is ready
 * for update, issue its recipe to a queue and wake up one worker thread.
 */
bool dag::schedule(string &&key)
{
	dag_node *p = get_node(::std::forward<string>(key));
	if (!p)
		return false;

	_bleach();
	auto s = new ::std::stack<dag_node*>;
	while (1) {
		while (p) {
			s->push(p);
			p->_status = dag_node::GREY;
			for (auto &u: p->_in_list)
				if (u->_status == dag_node::GREY) {
					fprintf(stderr,"LOOP DETECTED\n");
					u->print_node_debug();
					goto fail_exit;
				}
			p = p->first_white_parent();
		}

		if (s->empty())
			break;

		p = s->top();
		dag_node *tmp = p->first_white_parent();
		if (tmp) {
			p = tmp;
		} else {
			s->pop();
			p->print_node_debug();
			p->_status = dag_node::BLACK;
			p = NULL;
		}
	}
	delete s;
	return true;

fail_exit:
	delete s;
	return false;
}

void dag::_bleach() noexcept
{
	for (auto &node: _node_list)
		node.second->_status = dag_node::WHITE;
}
