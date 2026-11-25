#include "Agents.h"
#include <unordered_map>
std::string agents(const Snapshot& s){if(!s.valid)return "Agents: unavailable";std::unordered_map<unsigned long,unsigned> counts;unsigned max=0;for(auto& c:s.connections){auto n=++counts[c.pid];if(n>max)max=n;}return "Agents: network max TCP/PID="+std::to_string(max)+" process count="+std::to_string(s.processes);}
