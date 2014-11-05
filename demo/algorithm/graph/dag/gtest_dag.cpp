#include "dag.h"
#include "gtest/gtest.h"

TEST(Dag, Trivial) {
	dag g;
	g.add_edge("utils.cpp", "utils.o"  );
	g.add_edge("utils.h"  , "utils.o"  );
	g.add_edge("utils.h"  , "utils.cpp");
	g.add_edge("stdlib.h" , "utils.cpp");
	g.add_edge("stdlib.h" , "utils.h"  );
	EXPECT_TRUE(g.is_dag());
}

TEST(Dag, Construct) {
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

	g.remove_node("stdlib.h");
	EXPECT_FALSE(g.is_dag());

	g.remove_edge("utils.o","utils.cpp");
	EXPECT_TRUE(g.is_dag());
}

TEST(Dag, Schedule) {
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

	g.set_recipe("test_heapsort.exe", "g++ -o test_heapsort.exe"
			" test_heapsort.o heapsort.o utils.o");
	g.set_recipe("test_heapsort.o", "g++ -o test_heapsort.o test_heapsort.cpp");
	g.set_recipe("heapsort.o", "g++ -o heapsort.o heapsort.cpp");
	g.set_recipe("utils.o", "g++ -o utils.o utils.cpp");

	EXPECT_TRUE(g.is_dag());

	g.schedule("test_heapsort.exe");
}
