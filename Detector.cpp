#include "Detector.h"
#include <algorithm>
#include <unordered_map>
std::vector<std::string> Detector::inspect(const Snapshot& s){
 std::vector<std::string> alerts;if(!s.valid)return alerts;
 double p=s.processes,n=s.connections.size();
 if(count>=20){
 if(p>proc+std::max(20.0,proc*0.4))alerts.push_back("Process count spike");
 if(n>net+std::max(30.0,net*1.0))alerts.push_back("TCP connection count spike");
 }
 std::unordered_map<unsigned long,unsigned> per;
 for(auto& c:s.connections)++per[c.pid];
 for(auto& e:per)if(e.second>=100)alerts.push_back("High TCP count PID "+std::to_string(e.first)+": "+std::to_string(e.second));
 if(alerts.empty() || count==0){double a=count<20?1.0/(count+1):0.02;proc+=a*(p-proc);net+=a*(n-net);if(count<20)++count;}
 return alerts;
}
