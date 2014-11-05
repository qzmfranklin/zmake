#ifndef _DAG_H_
#define _DAG_H_

#include <assert.h>
#include <set>
#include <stack>
#include <vector>
#include <map>
#include <string>

/*
 * Adjacency list representation of directed acyclic graph
 */

using ::std::string;
using ::std::map;
using ::std::set;

class dag;
class dag_node {
	public:
		int _status;    // [WHITE, GREY, BLACK] | (OUTDATED)
		const string &_key;
		string _recipe;
		set<dag_node*> _in_list;
		set<dag_node*> _out_list;

	public:
		dag_node(string &&key, string &&recipe = ""):
			_status(OUTDATED),
			_key(::std::move(key)),
			_recipe(::std::move(recipe))
			{}

		void print_recipe() const noexcept
		{ printf("%s\n",_recipe.c_str()); }

		void set_recipe(string &&recipe) noexcept
		{ _recipe = ::std::move(recipe); }

		void print_node() const noexcept;
		void print_node_debug() const noexcept;

		dag_node *source_of() noexcept;
		dag_node *sink_of() noexcept;
		string &&status_string() const noexcept;

		/*
		 * Return the pointer to first white/grey/non-black child if any
		 * Return NULL if none
		 */
		dag_node *first_white_child() const noexcept;
		dag_node *first_grey_child() const noexcept;
		dag_node *first_non_black_child() const noexcept;

		friend class dag;

	public:
		enum {
			WHITE   = 0x0, // untouched
			GREY,  // put on stack/queue
			BLACK, // visited

			OUTDATED = 0x1<<8 // bit mask, if not, up to date
		};
};

class dag {
	public:
		int _status;
		map<string, dag_node*> _node_list;

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
		 * Non-recursive O(V+E) implementation of verifying DAG using
		 * DFS
		 */
		bool is_dag();

		::std::vector<dag_node*> &&schedule(string &&key);

	private:
		void _bleach() noexcept;

	public:
		enum {
			INIT = 0
		};
};

#endif /* end of include guard _DAG_H_ */
