#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include "Incident.h"
#include <sstream>
#ifdef _WIN32
#include "SecureState.h"
#endif
// Missing state is a first-run condition; corrupt state freezes learning.
inline bool loadIncident(const std::filesystem::path& p, Incident& i) {
 std::error_code ec; bool exists=std::filesystem::exists(p,ec); i.remaining=30;
 if(ec)return false; if(!exists)return true;
 #ifdef _WIN32
 std::string raw;if(!secureRead(p,raw))return false;std::istringstream f(raw);
#else
 std::ifstream f(p);
#endif
 std::string magic,extra; unsigned n=0;
 if(!(f>>magic>>n)||magic!="LGI1"||n>30||(f>>extra)||!f.eof())return false;
 i.remaining=n;return true;
}
inline bool saveIncident(const std::filesystem::path& p,const Incident& i){
#ifdef _WIN32
 return secureWrite(p,"LGI1 "+std::to_string(i.remaining)+"\n");
#else
 auto tmp=p;tmp+=".tmp";std::ofstream f(tmp,std::ios::trunc);if(!f)return false;
 f<<"LGI1 "<<i.remaining<<'\n';f.close();if(!f)return false;
 std::error_code ec;std::filesystem::rename(tmp,p,ec);return !ec;
#endif
}
