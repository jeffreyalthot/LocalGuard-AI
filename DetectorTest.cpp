#include "Detector.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
void check(bool b){if(!b)throw std::runtime_error("test failed");}
int main(){Detector d;Snapshot s;s.valid=true;s.processes=100;for(int i=0;i<20;i++)check(d.inspect(s).empty());check(d.learned()==20);s.processes=200;check(!d.inspect(s).empty()&&d.risk()>0);s.processes=100;check(d.inspect(s).empty());for(int i=0;i<100;i++)s.connections.push_back({42,"127.0.0.1"});check(d.inspect(s).size()>=2);check(d.risk()>=50);s.connections.clear();s.valid=false;check(d.inspect(s).empty());s.valid=true;auto p=std::filesystem::path("test-profile.dat");check(d.save(p));check(d.save(p));Detector e;check(e.load(p)&&e.learned()==20);s.processes=200;check(!e.inspect(s).empty());std::ofstream(p)<<"broken";check(!e.load(p)&&e.learned()==20);std::filesystem::remove(p);std::cout<<"Tests OK\n";}
