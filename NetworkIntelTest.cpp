#include "NetworkIntel.h"
#include <iostream>
int main(){NetworkIntel n;Snapshot s;s.valid=true;s.identities={{1,"one",100},{2,"two",200}};if(!n.update(s).healthy)return 1;
 for(unsigned i=0;i<12;++i)s.connections.push_back({1,std::to_string(i)});
 auto r=n.update(s);if(!r.healthy||r.findings.size()!=1||r.findings[0].pid!=1||r.findings[0].novel!=12)return 2;
 if(!n.update(s).findings.empty())return 3;
 s.connections.push_back({3,"unknown"});if(n.update(s).healthy)return 4;s.connections.pop_back();if(!n.update(s).findings.empty())return 5;
 for(unsigned i=12;i<40;++i)s.connections.push_back({1,std::to_string(i)});
 r=n.update(s);if(r.findings.size()!=1||r.findings[0].peers!=40)return 6;
 s.valid=false;if(n.update(s).healthy)return 7;
 s.valid=true;s.connections.resize(12);s.identities[0].name="changed";if(!n.update(s).findings.empty())return 8;
 s.connections.resize(16385);if(n.update(s).healthy)return 9;
 s.connections.clear(); s.identities[0].created=300; n.update(s); for(unsigned i=0;i<12;++i)s.connections.push_back({1,std::to_string(i)}); s.identities[0].created=400; if(!n.update(s).findings.empty())return 10; s.connections.clear(); s.identities[0].created=0; n.update(s); for(unsigned i=0;i<12;++i)s.connections.push_back({1,std::to_string(i)}); if(!n.update(s).findings.empty())return 11;
 std::cout<<"11 network attribution checks passed\n";
}
