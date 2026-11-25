#include "LearningGate.h"
#include <iostream>
int main(){for(unsigned i=0;i<8;++i){LearningGate g{bool(i&1),bool(i&2),bool(i&4)};if(g.allow()!=(i==2))return 1;}std::cout<<"8 gate combinations checked\n";}
