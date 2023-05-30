#ifndef _UTILS_H_
#define _UTILS_H_

// io, exception
#include <vector>
#include <iostream>
#include <iomanip>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

using std::setw;
using std::setfill;
using std::left;
using std::internal;
using std::right;

using std::exception;

// file stream
#include <fstream>

// file system
#include <filesystem>

using std::filesystem::exists;
using std::filesystem::create_directory;

// shared_ptr
#include <memory>
#include <mutex>

using std::shared_ptr;
using std::make_shared;
using std::mutex;
using std::lock_guard;

// assert
#include <cassert>

// functional
#include <functional>

using std::function;
using std::bind;

// unordered_set
#include <unordered_set>

using std::unordered_set;

string getSuffix(string);
string get_content_type(string);
vector<string> splitSite(string);
bool isSiteValid(string);
string trimString(string);
string trim_address(const string);
string add_img_path_prefix(string);

#endif // Utils.h