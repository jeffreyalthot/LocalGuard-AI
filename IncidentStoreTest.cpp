#include "IncidentStore.h"
#include <iostream>
int main(){auto p=std::filesystem::path("incident-test.dat");std::filesystem::remove(p);Incident a,b;
 if(!loadIncident(p,a)||a.remaining!=30)return 1;
 for(unsigned n=0;n<=30;++n){a.remaining=n;if(!saveIncident(p,a)||!loadIncident(p,b)||b.remaining!=n)return 2;}
 for(auto s:{"LGI1 31","LGI1 -1","LGI1 0 extra","broken",""}){std::ofstream(p)<<s;if(loadIncident(p,b)||b.remaining!=30)return 3;}
 std::filesystem::remove(p);std::cout<<"31 round trips and 5 corrupt states checked\n";}
