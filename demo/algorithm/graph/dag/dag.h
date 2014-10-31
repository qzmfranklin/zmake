#ifndef _DAG_H_
#define _DAG_H_

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

/*
 * Adjacency list representation of directed acyclici graph
 *
 * dag_t and dag_node_t are opaque types
 */

using ::std::string;
using ::std::unordered_map;

typedef struct _st_dag_node {
	int status;
	string key;
	string recipe;
	unordered_map<string, struct _st_dag_node*> to_list;
} *dag_node_t;

typedef struct _st_dag {
	int status;
	unordered_map<string, struct _st_dag_node*> node_list;
} *dag_t;

dag_t dag_create();

void dag_add_node(dag_t g, const string &key);
void dag_add_edge(dag_t g, const string &from, const string &to);
void dag_add_recipe(dag_t g, const string &key, const string &recipe);

void dag_remove_node(dag_t g, const string &key);
void dag_remove_edge(dag_t g, const string &from, const string &to);

dag_node_t dag_find(dag_t g, const string &key);

bool dag_is_dag(dag_t g);
void dag_destroy(dag_t g);


#endif /* end of include guard _DAG_H_ */
