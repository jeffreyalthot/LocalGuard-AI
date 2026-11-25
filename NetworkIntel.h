#pragma once
#include "Monitor.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
struct NetworkFinding { unsigned long pid; std::string name; unsigned peers, novel; bool sustained=false; };
struct NetworkReport { bool healthy=false; std::vector<NetworkFinding> findings; };
class NetworkIntel {
 using Peers=std::unordered_set<std::string>;
 struct Entry { std::string name; Peers peers; std::uint64_t created; unsigned churn=0; };
 std::unordered_map<unsigned long,Entry> previous;
 bool ready=false;
public:
 NetworkReport update(const Snapshot& s);
};
