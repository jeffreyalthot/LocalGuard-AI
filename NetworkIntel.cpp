#include <algorithm>
#include "NetworkIntel.h"
NetworkReport NetworkIntel::update(const Snapshot& s){
 NetworkReport r;
 if(!s.valid || s.connections.size()>16384 || s.identities.size()>8192){previous.clear();ready=false;return r;}
 std::unordered_map<unsigned long,Entry> current;
 for(const auto& p:s.identities) if(p.pid && !p.name.empty()) current.emplace(p.pid,Entry{p.name,{},p.created});
 bool complete=true;
 for(const auto& c:s.connections){auto it=current.find(c.pid);if(it==current.end()||c.remote.empty()){complete=false;continue;}it->second.peers.insert(c.remote);}
 if(!complete){previous.clear();ready=false;return r;}
 r.healthy=true;
 for(auto& kv:current){auto& e=kv.second;auto old=previous.find(kv.first);unsigned novel=0;
 if(ready && old!=previous.end() && old->second.name==e.name && e.created!=0 && old->second.created==e.created){for(const auto& p:e.peers)if(!old->second.peers.count(p))++novel; e.churn=old->second.churn>2?old->second.churn-2:0; e.churn=std::min(60u,e.churn+novel);}
 if(e.peers.size()>=40 || novel>=12 || e.churn>=24)r.findings.push_back({kv.first,e.name,static_cast<unsigned>(e.peers.size()),novel,e.churn>=24});
 }
 previous=std::move(current);ready=true;return r;
}
