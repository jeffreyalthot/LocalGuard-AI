#include "IdentityGuard.h"
#include <iostream>
int main(){unsigned cases=0;for(unsigned r=0;r<40;++r)for(unsigned p=0;p<2;++p)for(unsigned h=0;h<2;++h){IdentityGuard g;auto x=g.update({0,0,0,bool(h),p,r});if(x.alert!=(r>=5||p)||x.urgent!=(r>=15)||x.healthy!=bool(h))return 1;++cases;}IdentityGuard g;for(int i=0;i<5;++i)if(g.update({1,0,0,true,0,1}).alert!=(i==4))return 2;for(int i=0;i<6;++i)g.update({});if(g.update({}).alert)return 3;std::cout<<cases<<" session combinations and expiry passed\n";}
