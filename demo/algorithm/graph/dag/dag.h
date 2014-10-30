#ifndef _DAG_H_
#define _DAG_H_

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

/*
 * Adjacency list representation of directed acyclici graph
 *
 * dag_t is the only exported userspace type, it is meant to be opaque
 */

struct _st_dag_node {
	::std::string key;
	::std::string recipe;
	::std::unordered_map<::std::string, struct _st_dag_node*> adjacency_list;
};

typedef struct _st_dag {
	::std::unordered_map<::std::string, struct _st_dag_node*> vertex_list;
} dag_t;

#endif /* end of include guard _DAG_H_ */
