#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <unordered_map>
#include <string>
#include "dag.h"

using ::std::string;
using ::std::unordered_map;
using ::std::stack;

/*
 * Never use typedef'ed types, e.g., dag_t, dag_node_t
 *
 * When adding/removing node/edge, do not test the DAG is still a DAG. Testing
 * is done using a separate call dag_is_dag().
 *
 */

/*
 *typedef struct _st_dag_node {
 *        int status;
 *        string key;
 *        string recipe;
 *        unordered_map<string, struct _st_dag_node*> to_list;
 *} *dag_node_t;
 *
 *typedef struct _st_dag {
 *        int status;
 *        unordered_map<string, struct _st_dag_node*> node_list;
 *} *dag_t;
 */

#define DAG_INIT 0x1 // newly created

dag_t dag_create()
{
	struct _st_dag *g = (struct _st_dag*) malloc(sizeof(struct _st_dag));
	if (!g)
		return NULL;
	g->status = DAG_INIT;
	return g;
}

void dag_add_node(dag_t g, const string &key)
{
	g->node_list.emplace(std::forward(key));
}

void dag_add_edge(dag_t g, const string &from, const string &to);
void dag_add_recipe(dag_t g, const string &key, const string &recipe);

void dag_remove_node(dag_t g, const string &key);
void dag_remove_edge(dag_t g, const string &from, const string &to);


dag_node_t dag_find(dag_t g, const string &key);

bool dag_is_dag(dag_t g);
void dag_destroy(dag_t g)
{
	free(g);
}
