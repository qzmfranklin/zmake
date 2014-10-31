#ifndef _DAG_H_
#define _DAG_H_

#include <assert.h>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

/*
 * Adjacency list representation of directed acyclic graph
 */

using ::std::string;
using ::std::unordered_map;
using ::std::set;

/*
 * Do not provide any functionality through class dag_node
 * Do everything in class dag
 */
class dag;
class dag_node {
	private:
		int _status;
		string _recipe;
		set<dag_node*> _to_list;

	public:
		dag_node(string &&recipe = ""):
			_recipe(::std::move(recipe)), _status(WHITE) { };

		const string &get_recipe() const noexcept
		{ return _recipe; }

		void set_recipe(const string &recipe) noexcept
		{ _recipe = recipe; }

		friend class dag;

	private:
		enum {
			WHITE, // untouched
			GREY,  // put on stack/queue
			BLACK  // visited
		};
};

class dag {
	private:
		int _status;
		unordered_map<string, dag_node*> _node_list;
	public:
		dag(const size_t size = 100)
		{
			_status = INIT;
			_node_list.reserve(size);
		}

		~dag()
		{
		}

		// return NULL if no match was found
		dag_node *get_node(const string &key)
		{
			return _node_list.count(key) ? _node_list[key] : NULL;
		}

		/*
		 * return pointer to the node with key, allocate new node if
		 * needed
		 */
		dag_node *add_node(const string &key)
		{
			dag_node *p = get_node(key);
			if (!p) {
				p = new dag_node;
				assert(p);
				_node_list[key] = p;
			}
			return p;
		}

		/*
		 * add node(s) if needed, always succeed if STL does not throw
		 * an exception
		 */
		void add_edge(const string &from, const string &to)
		{
			dag_node *u = add_node(from);
			dag_node *v = add_node(to);
			u->_to_list.insert(v);
		}

		// verify the DAG property using DFS O(V) with time and memory
		bool is_dag() const;

	private:
		enum {
			INIT = 0
		};
};

#endif /* end of include guard _DAG_H_ */
