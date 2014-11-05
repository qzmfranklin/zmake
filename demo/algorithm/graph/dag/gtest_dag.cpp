#include "dag.h"
#include "gtest/gtest.h"

TEST(Dag, Trivial) {
	dag g;
	g.add_edge("utils.cpp", "utils.o"  );
	g.add_edge("utils.h"  , "utils.o"  );
	EXPECT_TRUE(g.is_dag());
}

TEST(Dag, IsDag) {
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
	EXPECT_TRUE(g.is_dag());

	g.add_edge("utils.o","utils.cpp");
	EXPECT_FALSE(g.is_dag());
}
