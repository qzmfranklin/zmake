#ifndef _DAG_H_
#define _DAG_H_

#include <assert.h>
#include <list>
#include <set>
#include <stack>
#include <vector>
#include <map>
#include <string>
#include <mutex>

/*
 * Adjacency list representation of directed acyclic graph
 */

using ::std::string;
using ::std::map;
using ::std::set;

class dag_node {
	public:
		// TODO: change orders to optimize storage
		int _type;           // NORMAL | PHONY
		int _status;         // WHITE  | GREY  | BLACK
		int _wait_black;     // used in schedule()
		const string &_key;
		string _recipe;
		set<dag_node*> _in_list;
		set<dag_node*> _out_list;
		::std::mutex _mtx;   // used in schedule()
		time_t _lmt;         // last modified time

	public:
		/*
		 * Do not create a dag_node on your own. It is designed to be
		 * constructed by dag methods
		 */
		dag_node(string &&key): _key(::std::move(key)), _recipe("") {}

		/*
		 * If _recipe == "", do not print anything
		 */
		void print_recipe() const noexcept;

		void print_node() const noexcept;
		void print_node_debug() const noexcept;

		/*
		 * Return this if loop is detected
		 */
		dag_node *source_of() noexcept;
		dag_node *sink_of() noexcept;

		string &&status_string() const noexcept;

		bool has_grey_child(const dag_node *p) const noexcept;

		/*
		 * Return the pointer if a match was found
		 * Return NULL if none was found
		 */
		dag_node *first_white_parent() const noexcept;
		dag_node *first_white_child() const noexcept;
		dag_node *first_grey_child() const noexcept;
		dag_node *first_non_black_child() const noexcept;

		bool update_last_modified_time();

		friend class dag;

	public:
		enum {
			NORMAL, // normal target
			PHONY   // phony target
		};

		enum {
			//        traversal                task scheduling
			WHITE, // untouched                up to date
			GREY,  // put on stack/queue       outdated, ready for update
			BLACK, // visited/ outdated        outdated, pending dependence
		};
};

class dag {
	public:
		int _status;
		map<string, dag_node*> _node_list;
		::std::list<dag_node*> _task_list;
		::std::mutex _task_list_mtx;

	public:
		dag(): _status(INIT) {}

		~dag()
		{
			for (auto &tmp: _node_list)
				delete(tmp.second);
		}

		/* Return NULL if no match was found */
		dag_node *get_node(string &&key);

		/*
		 * Return pointer to the node with key, allocate new node if
		 * needed
		 */
		dag_node *add_node(string &&key);

		/*
		 * Add node(s) if needed, always succeed if STL does not throw
		 * an exception
		 */
		void add_edge(string &&from, string &&to);

		/* 
		 * Delete node(key) when it exists, otherwise do nothing
		 */
		void remove_node(string &&key);

		/*
		 * Delete edge(from,to) when it exists, otherwise do nothing
		 */
		void remove_edge(string &&from, string &&to);

		void set_recipe(string &&key, string &&recipe);

		void remove_recipe(string &&key)
		{ set_recipe(::std::forward<string>(key),""); }

		size_t num_node() const noexcept { return _node_list.size(); }

		size_t num_edge() const noexcept
		{
			size_t retval = 0;
			for (auto &tmp: _node_list)
				retval += tmp.second->_out_list.size();
			return retval;
		}

		/*
		 * Print graph in the Depth First Search (DFS)
		 * Return true/false if the graph is/isn't a DAG
		 */
		void dfs();

		/*
		 * Non-recursive O(V+E) implementation of verifying DAG
		 */
		bool is_dag();

		/*
		 * Pushes nodes into _task_list in a proper order for use in
		 * schedule()
		 *
		 * Return false if key is not in the graph or if a cyclic
		 * dependence if detected
		 *
		 * Must call bleach() before the first call to
		 * add_to_task_list()
		 *
		 * You can add multiple targets by having multiple calls to
		 * add_to_task_list(). schedule() will figure out a most
		 * efficient way for building all targets using potentially
		 * multiple threads
		 */
		bool add_to_task_list(string &&key);
		void clear_task_list() noexcept { _task_list.clear(); }

		/*
		 * Schedule tasks in _task_list with n threads in a most
		 * efficient way. Must call add_to_task_list() at least once
		 * before calling schedule()
		 */
		void schedule(const int n = 1) noexcept;

		/*
		 * Set all nodes' _status to dag_node::WHITE
		 */
		void bleach() noexcept;

	public:
		enum {
			INIT,
			IS_DAG,
			NOT_DAG,
		};
};

#endif /* end of include guard _DAG_H_ */
