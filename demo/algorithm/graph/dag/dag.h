#ifndef _DAG_H_
#define _DAG_H_

#include <assert.h>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>
//#include <utility>

/*
 * Adjacency list representation of directed acyclic graph
 */

using ::std::string;
using ::std::map;
//using ::std::unordered_map;
using ::std::set;
//using ::std::vector;
//using ::std::priority_queue;

class dag;
class dag_node {
	private:
		int _status;
		int _order;
		const string &_key;
		string _recipe;
		set<dag_node*> _in_list;
		set<dag_node*> _out_list;

	public:
		dag_node(string &&key, string &&recipe = ""):
			_key(::std::forward<string>(key)),
			_recipe(::std::forward<string>(recipe)),
			_order(0),
			_status(WHITE) { };

		string &get_recipe() noexcept
		{ return _recipe; }

		void set_recipe(string &&recipe) noexcept
		{ _recipe = ::std::move(recipe); }

		void print_node() const noexcept
		{
			printf( "%s:",_key.c_str());
			for (auto &child: _out_list)
				printf(" %s",child->_key.c_str());
			printf("\n");
		}

		void print_recipe() const noexcept
		{ printf("%s\n",_recipe.c_str()); }

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
			// for traversal
			WHITE, // untouched
			GREY,  // put on stack/queue
			BLACK, // visited
		};
};

class compare_node_order {
	public:
		bool operator() (dag_node *&u, dag_node *&v) const noexcept
		{ return u->_order   <   v->_order; }
};

class dag {
	private:
		int _status;
		map<string, dag_node*> _node_list;

	public:
		dag() { _status = INIT; }

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
		dag_node *add_node(string &&key)
		{
			//dag_node *p = get_node(::std::move(key));
			dag_node *p = get_node(::std::move(key));
			if (!p) {
				p = new dag_node(::std::forward<string>(key));
				assert(p);
				_node_list[key] = p;
			}
			return p;
		}

		/*
		 * Add node(s) if needed, always succeed if STL does not throw
		 * an exception
		 */
		void add_edge(string &&from, string &&to)
		{
			dag_node *u = add_node(::std::move(from));
			dag_node *v = add_node(::std::move(to));
			u->_out_list.insert(v);
			v->_in_list.insert(u);
		}

		void remove_node(string &&key);
		void remove_edge(string &&from, string &&to)
		{
			dag_node *u = get_node(::std::forward<string>(from));
			dag_node *v = get_node(::std::forward<string>(to));
			if (!u  ||  !v)
				return;
			u->_out_list.erase(v);
			v->_in_list.erase(u);
		}

		size_t num_node() const { return _node_list.size(); }

		size_t num_edge() const
		{
			size_t retval = 0;
			for (auto &tmp: _node_list)
				retval += tmp.second->_out_list.size();
			return retval;
		}

		/*
		 * Verify the DAG property using DFS O(V) with time and memory
		 */
		bool is_dag();

		/*
		 *priority_queue<dag_node*,vector<dag_node*>,compare_node_order>
		 *        &&schedule(const string &&key);
		 */
	private:
		/*
		 * Recolor all nodes to dag_node::WHITE
		 */
		void _bleach()
		{
			for (auto &node: _node_list)
				node.second->_status = dag_node::WHITE;
		}

	public:
		enum {
			INIT = 0
		};
};

#endif /* end of include guard _DAG_H_ */
