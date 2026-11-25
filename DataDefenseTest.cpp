#include "DataDefense.h"
#include <iostream>
int main(){unsigned failures=0; auto check=[&](bool b){if(!b)++failures;};
 for(unsigned i=0;i<10000;++i){DataDefense d; for(int j=0;j<20;++j)check(!d.update({5,i%4,true,false,false,false}).review); auto a=d.update({50,20+i%30,true,false,false,false});check(a.review&&!a.urgent&&a.freeze);auto b=d.update({5,0,false,true,false,false});check(b.urgent&&b.degraded&&b.freeze);for(int j=0;j<12;++j)d.update({5,0,true,false,false,false});check(!d.update({5,0,true,false,false,false}).review);}
 DataDefense d;check(d.update({0,0,false,false,true,false}).review);std::cout<<"Sequences=10000 failures="<<failures<<'\n';return failures?1:0;}
