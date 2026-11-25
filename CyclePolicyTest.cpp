#include "CyclePolicy.h"
#include <iostream>
#include <cstdlib>
void check(bool b){if(!b)std::abort();}
int main(){unsigned cases=0;for(unsigned mask=0;mask<512;++mask)for(auto ms:{0ull,500ull,501ull,100000000000ull}){CyclePolicy p;CycleInput x{bool(mask&1),bool(mask&2),bool(mask&4),bool(mask&8),bool(mask&16),bool(mask&32),bool(mask&64),bool(mask&128),bool(mask&256),ms};for(int i=0;i<12;++i){auto d=p.update(x);bool h=(mask&255)==255;check(d.healthy==h);check(d.allowScan==(h&&ms<=500));check(d.allowLearning==(h&&!(mask&256)&&ms<=500&&i==11));check(d.interval==(ms<=500&&(mask&256)?2:5));}++cases;}
 CyclePolicy p;CycleInput x{true,true,true,true,true,true,true,true,false,0};for(int i=0;i<12;++i)p.update(x);x.elapsedMs=501;check(!p.update(x).allowScan);x.elapsedMs=0;check(!p.update(x).allowScan);check(!p.update(x).allowScan);check(p.update(x).allowScan);for(int i=0;i<1000000;++i){auto d=p.update(x);if(i>=11)check(d.allowLearning);}x.attribution=false;check(!p.update(x).allowLearning);std::cout<<cases<<" combinations and 1000000 steady cycles passed\n";}
