#pragma once
#include <string>
#include <vector>
struct Connection { unsigned long pid; std::string remote; };
struct Process { unsigned long pid; std::string name; };
struct Snapshot { std::vector<Connection> connections; std::vector<Process> identities; unsigned processes=0, udp=0; bool valid=false; };
Snapshot sample();
