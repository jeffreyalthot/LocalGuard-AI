#pragma once
#include "FolderWatch.h"
#include "MultiFolderRisk.h"
class MultiFolderWatch {
 std::array<FolderWatch,4> watchers; std::array<bool,4> configured{},started{}; MultiFolderRisk risk;
public:
 bool start(unsigned i,const wchar_t* path){if(i>=4||configured[i])return false;configured[i]=true;return started[i]=watchers[i].start(path);}
 void failed(unsigned i){if(i<4)configured[i]=true;}
 FileRisk poll(){std::array<FileSignals,4> s{};for(unsigned i=0;i<4;++i)if(configured[i]){if(started[i])s[i]=watchers[i].poll();else s[i].healthy=false;}return risk.update(s);}
};
