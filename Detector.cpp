#include "Detector.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <unordered_map>
std::vector<std::string> Detector::inspect(const Snapshot& s,unsigned starts,bool learn){
 std::vector<std::string> alerts; risk_=0; if(!s.valid)return alerts;
 std::array<double,4> x{double(s.processes),double(s.connections.size()),double(s.udp),double(starts)};
 const std::array<double,4> floor{20,30,20,10};
 const char* names[]={"Process count spike","TCP connection count spike","UDP endpoint spike","Process creation burst"};
 int score=0;
 if(count>=20)for(size_t i=0;i<x.size();++i){
 double threshold=std::max({floor[i],4*std::sqrt(variance[i]),mean[i]*0.5});
 if(x[i]-mean[i]>=threshold){alerts.emplace_back(names[i]);score+=25;}
 }
 std::unordered_map<unsigned long,unsigned> per;
 for(const auto& c:s.connections)++per[c.pid];
 for(const auto& e:per)if(e.second>=100){alerts.push_back("High TCP count PID "+std::to_string(e.first)+": "+std::to_string(e.second));score=std::max(score,50);break;}
 if(starts>=30){alerts.emplace_back("High process creation rate");score=std::max(score,50);}
 risk_=std::min(100,score);
 if(learn && alerts.empty()){
 double a=count<20?1.0/(count+1):0.01;
 for(size_t i=0;i<x.size();++i){double delta=x[i]-mean[i];mean[i]+=a*delta;variance[i]=(1-a)*(variance[i]+a*delta*delta);}
 if(count<20)++count;
 }
 return alerts;
}
bool Detector::save(const std::filesystem::path& p) const {
 auto tmp=p;tmp+=".tmp";std::ofstream f(tmp,std::ios::trunc);if(!f)return false;
 f<<"LG03 "<<count<<'\n'<<std::setprecision(17);for(size_t i=0;i<4;++i)f<<mean[i]<<' '<<variance[i]<<'\n';f.close();if(!f)return false;
 std::error_code ec;std::filesystem::rename(tmp,p,ec);return !ec;
}
bool Detector::load(const std::filesystem::path& p){
 std::ifstream f(p);std::string magic;unsigned n;std::array<double,4> m{},v{};
 if(!(f>>magic>>n)||magic!="LG03"||n>20)return false;
 for(size_t i=0;i<4;++i)if(!(f>>m[i]>>v[i])||!std::isfinite(m[i])||!std::isfinite(v[i])||m[i]<0||v[i]<0||m[i]>1e9||v[i]>1e18)return false;
 std::string extra;if(f>>extra)return false;count=n;mean=m;variance=v;return true;
}
