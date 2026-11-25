#pragma once
#include <array>
#include <cstdint>
struct IdentitySignal { unsigned failures=0, changes=0, audit=0; bool healthy=true; unsigned privileged=0, remoteFailures=0; };
struct IdentityResult { bool alert=false, urgent=false, healthy=true; unsigned failures=0; unsigned remoteFailures=0; };
class IdentityGuard {
 std::array<unsigned,6> history{}, remote{}; unsigned pos=0;
public:
 IdentityResult update(IdentitySignal s) {
  history[pos]=s.failures>1000?1000:s.failures; remote[pos]=s.remoteFailures>1000?1000:s.remoteFailures; pos=(pos+1)%history.size();
  unsigned total=0; for(auto n:history)total+=n;
  unsigned r=0; for(auto n:remote)r+=n;
  return {total>=10||s.changes>0||s.audit>0||s.privileged>0||r>=5, total>=30||s.audit>0||r>=15,s.healthy,total,r};
 }
};
