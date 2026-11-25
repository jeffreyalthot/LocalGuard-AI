#pragma once
#include "RansomGuard.h"
#include <array>
class MultiFolderRisk {
 std::array<RansomGuard,4> local{}; RansomGuard combined;
public:
 FileRisk update(const std::array<FileSignals,4>& samples) noexcept {
  FileSignals sum{}; bool alert=false, healthy=true;
  for(unsigned i=0;i<4;++i){auto s=samples[i]; auto r=local[i].update(s); alert|=r.alert; healthy&=r.healthy;
   sum.writes+=s.writes>10000?10000:s.writes; sum.renames+=s.renames>10000?10000:s.renames; sum.deletes+=s.deletes>10000?10000:s.deletes; sum.healthy&=s.healthy;}
  auto r=combined.update(sum); r.alert|=alert; r.healthy&=healthy; return r;
 }
};
