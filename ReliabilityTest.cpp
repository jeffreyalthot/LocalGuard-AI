#include "UnifiedPolicy.h"
#include <cstdlib>
#include <iostream>
#include <cstdint>
static void check(bool x){if(!x)std::abort();}
int main(){
 UnifiedInput clean{{true,true,true,true,true,true,true,true,false,0},false,false,false,false,false,true,true,true};
 for(unsigned gate=0;gate<5;++gate){UnifiedPolicy p;for(int i=0;i<12;++i)p.update(clean);check(p.update(clean).learn);auto x=clean;switch(gate){case 0:x.stateOK=false;break;case 1:x.profileOK=false;break;case 2:x.adaptiveFreeze=true;break;case 3:x.resilienceFreeze=true;break;case 4:x.incidentHeld=true;break;}check(!p.update(x).learn);for(int i=0;i<11;++i)check(!p.update(clean).learn);check(p.update(clean).learn);}
 UnifiedPolicy p;std::uint32_t rng=123456789;unsigned quiet=0,overload=0;for(unsigned i=0;i<1000000;++i){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;auto x=clean;unsigned m=rng%32; x.stateOK=m!=0;x.profileOK=m!=1;x.adaptiveFreeze=m==2;x.resilienceFreeze=m==3;x.incidentHeld=m==4;x.evidence=m==5;x.urgent=m==6;x.sensors.telemetry=m!=7;x.sensors.elapsedMs=m==8?501:0;x.activeEnabled=m!=9;bool h=x.stateOK&&x.profileOK&&x.sensors.telemetry;bool suspicious=x.evidence||x.urgent||x.adaptiveFreeze||x.resilienceFreeze||x.incidentHeld;if(x.sensors.elapsedMs>500)overload=3;else if(overload)--overload;if(!h||suspicious||overload)quiet=0;else if(quiet<12)++quiet;auto d=p.update(x);check(d.healthy==h);check(d.learn==(h&&quiet==12));check(d.persistProfile==d.learn);check(d.scan==(h&&!overload&&x.urgent&&x.activeEnabled));check(d.interval==((h&&!overload&&(x.evidence||x.urgent))?2u:5u));}
 std::cout<<"5 recovery gates and 1000000 deterministic mixed cycles passed\n";
}
