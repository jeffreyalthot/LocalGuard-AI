#include "StateGuard.h"
#include <iostream>
int main(){unsigned n=0;for(unsigned a=0;a<8;++a)for(unsigned b=0;b<8;++b){StateGuard g;auto good=[](unsigned x){return !(x&4)&&(!(x&1)||(x&2));};g.inspect(a&1,a&2,a&4);g.inspect(b&1,b&2,b&4);if(g.allow()!=(good(a)&&good(b)))return 1;++n;}std::cout<<n<<" state transitions checked\n";}
