#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include "Incident.h"
// Missing state is a first-run condition; corrupt state freezes learning.
inline bool loadIncident(const std::filesystem::path& p, Incident& i) {
 std::error_code ec; bool exists=std::filesystem::exists(p,ec); i.remaining=30;
 if(ec)return false; if(!exists)return true;
 std::ifstream f(p); std::string magic,extra; unsigned n=0;
 if(!(f>>magic>>n)||magic!="LGI1"||n>30||(f>>extra)||!f.eof())return false;
 i.remaining=n;return true;
}
inline bool saveIncident(const std::filesystem::path& p,const Incident& i){
 auto tmp=p;tmp+=".tmp";std::ofstream f(tmp,std::ios::trunc);if(!f)return false;
 f<<"LGI1 "<<i.remaining<<'\n';f.close();if(!f)return false;
 std::error_code ec;std::filesystem::rename(tmp,p,ec);return !ec;
}
