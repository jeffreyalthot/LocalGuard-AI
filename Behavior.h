#pragma once
#include "Monitor.h"
#include <map>
#include <set>
class Behavior {
 struct Entry {double mean=0; unsigned samples=0;};
 std::map<std::string,Entry> profiles;
public:
 std::vector<std::string> inspect(const Snapshot&, bool learn);
};
