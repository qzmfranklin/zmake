#include "gtest/gtest.h"
#include "dag.h"

static bool is_dag_true()
{
	dag g;

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

	return g.is_dag();
}

static bool is_dag_false()
{
	dag g;

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
	g.add_edge("utils.o","utils.cpp");

	return g.is_dag();
}

TEST(DAG, IsDAG) {
	EXPECT_TRUE(is_dag_true());
	EXPECT_FALSE(is_dag_false());
}
