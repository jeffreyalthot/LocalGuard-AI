#include "Protection.h"
#include <iostream>
int main(){unsigned n=0;for(unsigned b=0;b<32;++b)for(unsigned s=0;s<=100;++s){ProtectionInput x{bool(b&1),bool(b&2),bool(b&4),bool(b&8),bool(b&16),s};bool expected=(b&25)==25 && (b&6)!=0 && s>=60;if(requestContainment(x)!=expected)return 1;++n;}std::cout<<n<<" policy combinations passed\n";}
