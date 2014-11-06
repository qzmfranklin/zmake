#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <unordered_set>
#include <stack>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <assert.h>
#include <thread>
#include <mutex>
#include <time.h>
#include <string.h>
#include <list>
#include "dag.h"

void dag_node::print_recipe() const noexcept
{
	if (_recipe.empty())
		return;
	printf("%s\n",_recipe.c_str());
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
 *
 * This naive implementation of non-recursive DFS cannot be used for detecting
 * loops in the graph. In short, the stack of this implementation is not the
 * same as the stack formed by a recursive DFS implementation
 *
 * dag::is_dag() and dag::schedule() are correct implementations of
 * non-recursive DFS with only a single stack that can detect loops
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
 * The problem of DAG task scheduling with multiple workders is NP complete.
 * There is no need to find the best scheduler for all cases. This method
 * does a post-order depth first traversal from the target node and pushes the
 * output nodes into _task_list. _task_list is a feasible queue for a single
 * worker thread and should serve as the starting point for multi worker
 * scheduling methods
 */
bool dag::add_to_task_list(string &&key)
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
			_task_list.push_back(p);
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

/*
 * Return true if the last modified time (lmt) of lhs is newer than that of the rhs
 * Return false otherwise
 *
 * If lhs is not a file, return false
 * If rhs is not a file, return true
 * If neither lhs nor rhs is a file, return true
 */
static bool _lmt_newer_than(const dag_node *restrict lhs, const dag_node *restrict rhs)
{
	struct stat lstat;
	struct stat rstat;
	memset(&lstat,0,sizeof(struct stat));
	memset(&rstat,0,sizeof(struct stat));

	if (stat(rhs->_key.c_str(),&rstat))
		return true;
	if (stat(lhs->_key.c_str(),&lstat))
		return false;

	const double diff = difftime(lstat.st_mtime,rstat.st_mtime);
	return (diff > 0.0) ? true : false;
}

static time_t _last_modified_time(dag_node *p)
{
	struct stat pstat;
	memset(&pstat,0,sizeof(struct stat));
	if (stat(p->_key.c_str(),&pstat)) {
		perror("dag.cpp: _last_modified_time");
		fprintf(stderr,"stat(%p,%p) error\n",p->_key.c_str(),&pstat);
		exit(1);
	}
	/* The field name might be architecture dependent */
	return pstat.st_mtime;
}

/*
 * Preprocessing stage: (master thread only)
 *                 Iterate through _task_list with iterator &p
 *     * Use stat() to retrieve the last modified time (_lmt) of file (_key). If
 *       file does not exist, abort execution
 *     * If p has no parents, i.e., is a source, mark p as WHITE
 *     * If p has any non-white parents, mark p as BLACK, and set _wait_black to
 *       the number of non-white parents
 *     * If p only has white parents (at least one), but the last modified date
 *       of at least one of p's parents is newer than the last modified date of
 *       p, and p has a non-null recipe, then mark p as GREY
 *     * Otherwise, mark p as WHITE
 */
static void _schedule_first_stage(::std::list<dag_node *> &list)
{
	for (auto &u: list) {
	}
}

/*
 * Two pass dynamic scheduling using n worker threads
 *
 * Complexity:
 *         O(N) in memory
 *         o(N^2) in time
 *
 * The flags WHITE, GREY, and BLACK are reused with different meaning:
 *         WHITE: up to date
 *         GREY:  outdated, ready for update
 *         BLACK: outdated, has pending dependence
 *
 *
 * Preprocessing stage: (master thread only)
 *                 Iterate through _task_list with iterator &p
 *     * Use stat() to retrieve the last modified time (_lmt) of file (_key). If
 *       file does not exist, abort execution
 *     * If p has no parents, i.e., is a source, mark p as WHITE
 *     * If p has any non-white parents, mark p as BLACK, and set _wait_black to
 *       the number of non-white parents
 *     * If p only has white parents (at least one), but the last modified date
 *       of at least one of p's parents is newer than the last modified date of
 *       p, and p has a non-null recipe, then mark p as GREY
 *     * Otherwise, mark p as WHITE
 *
 * After the first stage, the color of any node is at least as dark as its
 * darkest parent's color
 *
 *
 * Scheduling stage:
 *
 *                 master thread (scheduler, producer)
 *     * Initial setup:
 *           * Remove WHITE nodes from the top of _task_list
 *           * Allocate a task queue (q),a mutex (q_mtx), and a conditoinal
 *             variable (q_cond)
 *           * Spawn n worker threads
 *     * Iterate until both _task_list are empty
 *           * Lock _task_list_mtx
 *           * Iterate _task_list with p
 *                 * If p is GREY
 *                       * Lock q_mtx
 *                       * Push p onto q
 *                       * Unlock q_mtx
 *                       * Remove p from _task_list
 *                       * Signal q_cond to one worker thread
 *                 * If p is BLACK, break
 *           * Unlock _task_list_mtx
 *     * Join all worker threads
 *
 *                 worker threads (worker, consumer)
 *     * Loop until both _task_list and q are empty
 *           * Lock q_mtx
 *           * Wait on q_cond for !q.empty()
 *           * Let p = q.front()
 *           * q.pop()
 *           * Unlock q_mtx
 *           * Invoke p's recipe
 *           * Mark p as WHITE
 *           * Iterate through p's children with u (note that u must be BLACK)
 *                 * Lock u->_mtx
 *                 * decrease u->_wait_black by 1
 *                 * If u->_wait_black == 0
 *                       * Mark u as GREY
 *                       * Lock _task_list_mtx
 *                       * Move u to the front of _task_list
 *                       * Unlock _task_list_mtx
 *                 * Unlock u->_mtx
 *
 * It is possible that a file exists during the first pass but is later deleted
 * during the second pass.
 */
void dag::schedule(const int n) noexcept
{
	if (_task_list.size() == 0)
		return;

	_schedule_first_stage(_task_list);
	::std::vector<::std::thread> worker_list(n);
}

void dag::_bleach() noexcept
{
	for (auto &node: _node_list)
		node.second->_status = dag_node::WHITE;
}
