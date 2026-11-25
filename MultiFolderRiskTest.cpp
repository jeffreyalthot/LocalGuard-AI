#include "MultiFolderRisk.h"
#include <cassert>
#include <iostream>
int main(){MultiFolderRisk g; std::array<FileSignals,4> s{}; for(auto& x:s)x.writes=75; assert(g.update(s).alert); s={}; for(int i=0;i<6;++i)g.update(s); assert(!g.update(s).alert); s[2].healthy=false; assert(!g.update(s).healthy); s={}; for(int i=0;i<5;++i)assert(!g.update(s).healthy); assert(g.update(s).healthy); for(int i=0;i<100000;++i){auto r=g.update(s);assert(r.healthy&&!r.alert);} std::cout<<"Multi-folder aggregation, expiry, degraded sensor and 100000 quiet cycles passed\n";}
