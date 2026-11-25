#pragma once
#include <string>
#include <vector>
struct Connection { unsigned long pid; std::string remote; };
struct Snapshot { std::vector<Connection> connections; unsigned processes=0; bool valid=false; };
Snapshot sample();
