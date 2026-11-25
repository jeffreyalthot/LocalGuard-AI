#pragma once
#include "RecoveryCopy.h"
#include <map>
namespace components {
inline const std::array<std::string,2> names={"LocalGuard.exe","RecoveryCopy.exe"};
using Manifest=std::map<std::string,std::string>;
inline bool read(const recovery::fs::path& p,Manifest& out){
 try { recovery::safePath(p); if(!recovery::fs::is_regular_file(p)||recovery::fs::file_size(p)>512)return false;
 std::ifstream f(p,std::ios::binary); Manifest m; std::string line;
 while(std::getline(f,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();auto pos=line.find(' ');if(pos!=64)return false;auto name=line.substr(65),digest=line.substr(0,64);if((name!=names[0]&&name!=names[1])||!recovery::validDigest(digest)||!m.emplace(name,digest).second)return false;}
 if(!f.eof()||f.bad()||m.size()!=names.size())return false;out=std::move(m);return true;
 }catch(...){return false;}
}
inline bool verify(const recovery::fs::path& root,const Manifest& m){try{for(auto& name:names){auto it=m.find(name);auto p=root/name;if(it==m.end()||!recovery::fs::is_regular_file(p)||recovery::fs::file_size(p)>64ULL*1024*1024||recovery::stream(p)!=it->second)return false;}return m.size()==names.size();}catch(...){return false;}}
}
