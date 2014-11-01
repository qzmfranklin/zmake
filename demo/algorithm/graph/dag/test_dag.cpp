#include <stdio.h>
#include "dag.h"

int main(int argc, char const* argv[])
{
	dag g;

	g.add_edge("utils.o","utils.cpp");
	g.add_edge("utils.o","utils.h");
	g.add_edge("utils.cpp","utils.h");
	g.add_edge("utils.cpp","stdlib.h");
	g.add_edge("utils.h","stdlib.h");

	g.add_edge("heapsort.o","heapsort.cpp");
	g.add_edge("heapsort.o","heapsort.h");
	g.add_edge("heapsort.h","utils.h");
	g.add_edge("heapsort.h","stdlib.h");

	g.add_edge("test_heapsort.exe","test_heapsort.o");
	g.add_edge("test_heapsort.exe","heapsort.o");
	g.add_edge("test_heapsort.exe","utils.o");
	g.add_edge("test_heapsort.o","test_heapsort.cpp");
	g.add_edge("test_heapsort.o","heapsort.h");
	g.add_edge("test_heapsort.o","utils.h");
	g.add_edge("test_heapsort.cpp","heapsort.h");
	g.add_edge("test_heapsort.cpp","utils.h");
	g.add_edge("test_heapsort.cpp","stdlib.h");

	/* Fail case */
	g.add_edge("utils.cpp","utils.o");

	printf("number of nodes   = %lu\n",g.num_node());
	printf("number of edges   = %lu\n",g.num_edge());
	printf("is_dag            = %s\n", g.is_dag() ? "true" : "false");

	using ::std::vector;
	using ::std::priority_queue;
	priority_queue<dag_node*, vector<dag_node*>, compare_node_priority>
		flow = g.schedule("test_heapsort.exe");

	return 0;
}
