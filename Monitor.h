#pragma once
#include <string>
#include <cstdint>
#include <vector>
struct Connection { unsigned long pid; std::string remote; };
struct Process { unsigned long pid; std::string name; std::uint64_t created=0; unsigned long parent=0; };
struct Snapshot { std::vector<Connection> connections; std::vector<Process> identities; unsigned processes=0, udp=0; bool valid=false; };
Snapshot sample();
