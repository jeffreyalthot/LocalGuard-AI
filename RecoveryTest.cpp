#include "Recovery.h"
#include <iostream>
int main(){Recovery r; for(int i=0;i<100;++i)if(r.update({true,true,false}))return 1; if(r.update({true,true,true})||r.update({true,true,true})||r.update({true,false,true}))return 2; if(r.update({true,true,true})||r.update({true,true,true})||!r.update({true,true,true}))return 3; for(int i=0;i<899;++i)if(r.update({true,true,true}))return 4; if(!r.update({true,true,true}))return 5; std::cout<<"Recovery policy passed\n";}
