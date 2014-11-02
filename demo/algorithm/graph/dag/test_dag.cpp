#include <stdio.h>
#include "dag.h"

int main(int argc, char const* argv[])
{
	dag g;

	/* add_edge(prerequisite,target) */
	g.add_edge("utils.cpp", "utils.o"  );
	g.add_edge("utils.h"  , "utils.o"  );
	g.add_edge("utils.h"  , "utils.cpp");
	g.add_edge("stdlib.h" , "utils.cpp");
	g.add_edge("stdlib.h" , "utils.h"  );

	g.add_edge("heapsort.cpp", "heapsort.o");
	g.add_edge("heapsort.h"  , "heapsort.o");
	g.add_edge("heapsort.h"  , "heapsort.cpp");
	g.add_edge("utils.h"     , "heapsort.h");
	g.add_edge("stdlib.h"    , "heapsort.h");

	g.add_edge("test_heapsort.o"  , "test_heapsort.exe");
	g.add_edge("heapsort.o"       , "test_heapsort.exe");
	g.add_edge("utils.o"          , "test_heapsort.exe");
	g.add_edge("test_heapsort.cpp", "test_heapsort.o"  );
	g.add_edge("heapsort.h"       , "test_heapsort.o"  );
	g.add_edge("utils.h"          , "test_heapsort.o"  );
	g.add_edge("heapsort.h"       , "test_heapsort.cpp");
	g.add_edge("utils.h"          , "test_heapsort.cpp");
	g.add_edge("stdlib.h"         , "test_heapsort.cpp");

	// fail case 
	//g.add_edge("utils.o","utils.cpp");

	printf("number of nodes   = %lu\n",g.num_node());
	printf("number of edges   = %lu\n",g.num_edge());

	printf("g %s a DAG\n", g.dfs() ? "is" : "is not");


	return 0;
}
