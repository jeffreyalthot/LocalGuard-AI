#include "Behavior.h"
#include <stdexcept>
int main(){Behavior b;Snapshot s;s.valid=true;s.identities.push_back({1,"test.exe"});for(int i=0;i<20;++i)if(!b.inspect(s,true).empty())return 1;for(int i=0;i<40;++i)s.connections.push_back({1,"127.0.0.1"});if(b.inspect(s,true).empty())return 2;s.valid=false;if(!b.inspect(s,true).empty())return 3;return 0;}
