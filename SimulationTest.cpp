#include "Detector.h"
#include "Behavior.h"
#include "Fusion.h"
#include <iostream>
#include <random>
int main(){std::mt19937 rng(217);unsigned misses=0, falseAlerts=0, incidents=0, normals=0;
for(unsigned trial=0;trial<1000;++trial){Detector d;Behavior b;Fusion f;Snapshot s;s.valid=true;s.processes=200;s.udp=50;s.identities.push_back({1,"test.exe"});
for(int i=0;i<25;++i){s.connections.assign(5+rng()%5,{1,"synthetic"});d.inspect(s,0);b.inspect(s,true);}
for(unsigned kind=0;kind<8;++kind){Snapshot x=s;unsigned starts=0;bool file=false,reg=false;switch(kind){case 1:x.processes+=100;break;case 2:x.connections.assign(120,{1,"synthetic"});break;case 3:x.udp+=100;break;case 4:starts=40;break;case 5:file=true;break;case 6:reg=true;break;case 7:file=true;x.valid=false;break;default:break;}
Detector test=d;test.inspect(x,starts);auto a=b.inspect(x,false);auto r=f.update(test.risk(),!a.empty(),file,reg,x.valid);if(kind){++incidents;if(!r.review)++misses;}else{++normals;if(r.review)++falseAlerts;}}
}std::cout<<"Synthetic incidents: "<<incidents<<" missed: "<<misses<<"\nSynthetic normal cases: "<<normals<<" false alerts: "<<falseAlerts<<'\n';return misses||falseAlerts?1:0;}
