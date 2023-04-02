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

string getSuffix(string);
string get_content_type(string);
vector<string> splitSite(string);
bool isSiteValid(string);
string trimString(string);

#endif // Utils.h