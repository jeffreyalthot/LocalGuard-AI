#include "Behavior.h"
#include <algorithm>
#include <cctype>
std::vector<std::string> Behavior::inspect(const Snapshot& s,bool learn){
 std::vector<std::string> out; if(!s.valid)return out;
 std::map<unsigned long,std::string> names; std::map<std::string,unsigned> counts;
 for(auto& p:s.identities){auto n=p.name;for(auto& c:n)c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));names[p.pid]=n;counts[n]=0;}
 for(auto& c:s.connections){auto i=names.find(c.pid);if(i!=names.end())++counts[i->second];}
 for(auto& c:counts){auto i=profiles.find(c.first);if(i!=profiles.end()&&i->second.samples>=20&&c.second>std::max(20.0,i->second.mean*3+10))out.push_back("Behavior: unusual TCP count for "+c.first+" ("+std::to_string(c.second)+")");}
 if(learn&&out.empty())for(auto& c:counts){if(profiles.size()>=2048&&!profiles.count(c.first))continue;auto& e=profiles[c.first];double a=e.samples<20?1.0/(e.samples+1):0.005;e.mean+=a*(c.second-e.mean);if(e.samples<20)++e.samples;}
 return out;
}
