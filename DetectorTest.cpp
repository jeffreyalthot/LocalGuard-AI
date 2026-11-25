#include "Detector.h"
#include <cassert>
#include <iostream>
int main(){Detector d;Snapshot s;s.valid=true;s.processes=100;for(int i=0;i<20;i++)assert(d.inspect(s).empty());s.processes=200;assert(!d.inspect(s).empty());s.processes=100;assert(d.inspect(s).empty());for(int i=0;i<100;i++)s.connections.push_back({42,"127.0.0.1"});s.identities.push_back({42,"test.exe"});auto a=d.inspect(s);assert(a.size()>=2);s.valid=false;assert(d.inspect(s).empty());std::cout<<"Tests OK\n";}
