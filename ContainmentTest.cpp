#include "Containment.h"
#include <iostream>
int main(){unsigned n=0;for(unsigned m=0;m<64;++m)for(unsigned t=0;t<=16;++t){ContainmentInput x{bool(m&1),bool(m&2),bool(m&4),bool(m&8),bool(m&16),bool(m&32),t};if(containmentAllowed(x)!=(m==31&&t>=1&&t<=15))return 1;++n;}std::cout<<n<<" policy combinations checked\n";}
