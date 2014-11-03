#ifndef _DAG_H_
#define _DAG_H_

#include <assert.h>
#include <set>
#include <stack>
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
		int _status;    // WHITE, GREY, BLACK
		int _dfs_order;
		int _bfs_order;
		const string &_key;
		string _recipe;
		set<dag_node*> _in_list;
		set<dag_node*> _out_list;
		set<dag_node*>::iterator _in_itr;
		set<dag_node*>::iterator _out_itr;

	public:
		dag_node(string &&key, string &&recipe = ""):
			_key(::std::move(key)),
			_recipe(::std::move(recipe)),
			_dfs_order(0), _bfs_order(0),
			_status(WHITE) { };

		void print_recipe() const noexcept
		{ printf("%s\n",_recipe.c_str()); }

		string &get_recipe() noexcept
		{ return _recipe; }

		void set_recipe(string &&recipe) noexcept
		{ _recipe = ::std::move(recipe); }

		void print_node() const noexcept;
		void print_node_debug() const noexcept;

		/*
		 * Return first white (not pushed, not visited) child
		 * Return NULL if no child is white
		 */
		dag_node *first_white_child() const noexcept
		{
			dag_node *v = NULL;
			for (auto &&v_itr = _out_list.begin();
					v_itr != _out_list.end(); ++v_itr)
				if ((*v_itr)->_status == WHITE) {
					v = *v_itr;
					break;
				}
			return v;
		}

		friend class dag;
		friend class compare_node_order;

	public:
		enum {
			WHITE, // untouched
			GREY,  // put on stack/queue
			BLACK, // visited
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
		dag_node *get_node(string &&key)
		{ return _node_list.count(key) ? _node_list[key] : NULL; }

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
	private:
		void _bleach() noexcept;
		void _reset_iterators() noexcept;
		dag_node *_source_of(dag_node *p) const noexcept;

	public:
		enum {
			INIT = 0
		};
};

#endif /* end of include guard _DAG_H_ */
