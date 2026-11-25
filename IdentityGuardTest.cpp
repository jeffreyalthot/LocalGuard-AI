#include "IdentityGuard.h"
#include <iostream>
int main(){unsigned cases=0;for(unsigned f=0;f<=40;++f)for(unsigned c=0;c<2;++c)for(unsigned a=0;a<2;++a)for(unsigned h=0;h<2;++h){IdentityGuard g;auto r=g.update({f,c,a,bool(h)});if(r.alert!=(f>=10||c||a)||r.urgent!=(f>=30||a)||r.healthy!=bool(h))return 1;++cases;}IdentityGuard g;for(int i=0;i<5;++i)if(g.update({2,0,0,true}).alert!=(i==4))return 2;for(int i=0;i<6;++i)g.update({});if(g.update({}).alert)return 3;std::cout<<cases<<" identity combinations and rolling window passed\n";}
