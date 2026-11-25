#pragma once
#include <array>
#include <cstdint>
struct IdentitySignal { unsigned failures=0, changes=0, audit=0; bool healthy=true; };
struct IdentityResult { bool alert=false, urgent=false, healthy=true; unsigned failures=0; };
class IdentityGuard {
 std::array<unsigned,6> history{}; unsigned pos=0;
public:
 IdentityResult update(IdentitySignal s) {
  history[pos]=s.failures>1000?1000:s.failures; pos=(pos+1)%history.size();
  unsigned total=0; for(auto n:history)total+=n;
  return {total>=10||s.changes>0||s.audit>0, total>=30||s.audit>0,s.healthy,total};
 }
};
