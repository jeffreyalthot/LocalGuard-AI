#include "NetworkIntel.h"
#include <iostream>
int main(){NetworkIntel n; Snapshot s; s.valid=true;s.identities={{1,"test",100}}; if(!n.update(s).healthy)return 1;
for(unsigned t=0;t<6;++t){s.connections.clear();for(unsigned i=0;i<6;++i)s.connections.push_back({1,std::to_string(t*6+i)});auto r=n.update(s);if(!r.healthy)return 2;if(t<5&&!r.findings.empty())return 3;if(t==5&&(r.findings.size()!=1||!r.findings[0].sustained))return 4;}
s.identities[0].created=200;if(!n.update(s).findings.empty())return 5;
for(unsigned t=0;t<10000;++t)if(!n.update(s).findings.empty())return 6;
s.valid=false;if(n.update(s).healthy)return 7;s.valid=true;if(!n.update(s).findings.empty())return 8;
std::cout<<"Sustained churn, identity reset, telemetry reset and 10000 stable cycles passed\n";}
