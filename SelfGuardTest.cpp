#include "SelfGuard.h"
#include <iostream>
int main(){unsigned n=0;for(unsigned a=0;a<8;++a)for(unsigned b=0;b<8;++b){SelfGuard g;auto step=[&](unsigned x){return g.update(x&1,x&2,x&4);};if(step(a)!=(a==7)||step(b)!=(a==7&&b==7))return 1;++n;}std::cout<<n<<" integrity transitions passed\n";}
