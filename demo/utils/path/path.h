#ifndef _UTILS_PATH_PATH_H_
#define _UTILS_PATH_PATH_H_

#include <stdlib.h>
#include <string>

/*
 * A close mimic of the python os.path functions.
 */
namespace path {

using std::string;
//string abspath(const string &path);
string base_name(const string &path);
string dir_name(const string &path);
bool exists(const string &path);
//bool isabs(const string &path);
bool isfile(const string &path);
bool isdir(const string &path);
//bool islink(const string &path);
string real_path(const string &path);
//std::pair<string, string> split(const string &path);

} // namespace path

#endif /* end of include guard _UTILS_PATH_PATH_H_ */
