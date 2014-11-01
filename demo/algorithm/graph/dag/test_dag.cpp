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
	g.add_edge("heapsort.h","stdlib.h");
	g.add_edge("heapsort.h","utils.h");

	g.add_edge("test_heapsort.exe","test_heapsort.o");
	g.add_edge("test_heapsort.exe","heapsort.o");
	g.add_edge("test_heapsort.exe","utils.o");
	g.add_edge("test_heapsort.o","test_heapsort.cpp");
	g.add_edge("test_heapsort.o","heapsort.h");
	g.add_edge("test_heapsort.o","utils.h");

	printf("sizeof(class node)= %lu\n",sizeof(class dag_node));
	printf("sizeof(class dag) = %lu\n",sizeof(class dag));
	printf("number of nodes   = %lu\n",g.num_node());
	printf("number of edges   = %lu\n",g.num_edge());
	printf("is_dag            = %s\n", g.is_dag() ? "true" : "false");

	return 0;
}
