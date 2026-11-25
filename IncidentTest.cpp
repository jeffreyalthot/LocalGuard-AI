#include "Incident.h"
#include <iostream>
int main(){unsigned cases=0;for(unsigned mask=0;mask<65536;++mask){Incident i;unsigned expected=0;for(unsigned j=0;j<16;++j){bool e=(mask>>j)&1, h=(j%3)!=0;if(e)expected=30;else if(h&&expected)--expected;if(i.update(e,h)!=(expected!=0)||i.remaining!=expected)return 1;++cases;}}Incident i;i.update(true,true);for(int j=0;j<100;++j)if(!i.update(false,false))return 2;for(int j=0;j<29;++j)if(!i.update(false,true))return 3;if(i.update(false,true))return 4;std::cout<<cases<<" transitions checked\n";}
