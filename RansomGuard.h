#pragma once
#include <array>
#include <cstdint>
struct FileSignals { unsigned writes=0, renames=0, deletes=0; bool healthy=true; };
struct FileRisk { bool alert, healthy; unsigned writes, renames, deletes; };
class RansomGuard {
 std::array<FileSignals,6> history{}; unsigned pos=0;
public:
 FileRisk update(FileSignals s) noexcept {
  history[pos]=s; pos=(pos+1)%history.size();
  unsigned w=0,r=0,d=0; bool ok=true;
  for(auto x:history){w+=x.writes>10000?10000:x.writes;r+=x.renames>10000?10000:x.renames;d+=x.deletes>10000?10000:x.deletes;ok=ok&&x.healthy;}
  return {w>=300 || r>=60 || d>=100 || (w>=80 && r+d>=20),ok,w,r,d};
 }
};
