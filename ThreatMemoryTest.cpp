#include "ThreatMemory.h"
#include <cassert>
#include <iostream>
int main(){
 for(unsigned a=0;a<64;++a)for(unsigned b=0;b<64;++b){ThreatMemory m;m.update(0,a,true);auto r=m.update(1,b,true);unsigned n=0;for(unsigned v=a|b;v;v>>=1)n+=v&1;assert(r.categories==n);assert(r.correlated==(a!=0&&(b&~a)!=0&&n>=3));}
 ThreatMemory m;assert(!m.update(0,1,true).correlated);assert(!m.update(10,1,true).correlated);assert(!m.update(20,6,false).correlated);assert(m.update(30,6,true).correlated);assert(!m.update(630,0,true).correlated);assert(!m.update(1,7,true).correlated);
 ThreatMemory stress;for(unsigned i=0;i<1000000;++i){auto r=stress.update(i,i%2?1:2,true);assert(r.episodes<=64&&r.categories<=2&&!r.correlated);}
 std::cout<<"4096 mask pairs and 1000000 bounded-history cycles passed\n";
}
