#include "path.h"

#include <string>

#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

namespace path {

using std::string;

string abspath(const string &path);

/*
 *  path         dirname    basename
 *  "/usr/lib"    "/usr"    "lib"
 *  "/usr/"       "/"       "usr"
 *  "usr"         "."       "usr"
 *  "/"           "/"       "/"
 *  "."           "."       "."
 *  ".."          "."       ".."
 */
string base_name(const string &path)
{
	static char __buf__[PATH_MAX];
	strcpy(__buf__, path.c_str());
	return basename(__buf__);
}

string dir_name(const string &path)
{
	static char __buf__[PATH_MAX];
	strcpy(__buf__, path.c_str());
	return dirname(__buf__);
}

bool exists(const string &path);
//bool isabs(const string &path);
bool isfile(const string &path);
bool isdir(const string &path);
//bool islink(const string &path);

string real_path(const string &path)
{
	static char __buf__[PATH_MAX];
	realpath(path.c_str(), __buf__);
	return __buf__;
}

//std::pair<string, string> split(const string &path);

} // namespace path
