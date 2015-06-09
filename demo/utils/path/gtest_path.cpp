#include "path.h"

#include "gtest/gtest.h"
#include <array>
#include <string>

/*
 *  path         dirname    basename
 *  "/usr/lib"    "/usr"    "lib"
 *  "/usr/"       "/"       "usr"
 *  "usr"         "."       "usr"
 *  "/"           "/"       "/"
 *  "."           "."       "."
 *  ".."          "."       ".."
 */
TEST(PATH, BaseDirName) {
	const size_t num = 6;
	const std::array<std::string, num> path_list = {
		"/usr/lib", "/usr/", "usr", "/", ".", ".."
	};
	const std::array<std::string, num> dirname_list = {
		"/usr", "/", ".", "/", ".", "."
	};
	const std::array<std::string, num> basename_list = {
		"lib", "usr", "usr", "/", ".", ".."
	};
	for (size_t i = 0; i < num; i++) {
		EXPECT_EQ(path::dir_name (path_list[i]), dirname_list [i]);
		EXPECT_EQ(path::base_name(path_list[i]), basename_list[i]);
	}
}

TEST(PATH, RealPath) {
}
