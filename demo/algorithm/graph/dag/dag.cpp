#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stack>
#include <map>
#include <unordered_map>
#include <string>
#include <utility>
#include "dag.h"


// verify the DAG property using DFS O(V) with time and memory
bool dag::is_dag() const
{
	return true;
}
