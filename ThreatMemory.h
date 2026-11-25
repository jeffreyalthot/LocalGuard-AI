#pragma once
#include <array>
#include <cstdint>
struct ThreatResult { unsigned categories, episodes; bool correlated; };
// Rising edges only; monotonic seconds, bounded history, no personal identifiers.
class ThreatMemory {
 struct Entry { std::uint64_t time=0; unsigned mask=0; };
 std::array<Entry,64> history{};
 unsigned cursor=0, previous=0; std::uint64_t last=0;
public:
 ThreatResult update(std::uint64_t now,unsigned mask,bool healthy) noexcept {
  mask &= 63u;
  if(now<last){history={};cursor=0;previous=0;}
  last=now;
  if(healthy){unsigned edges=mask&~previous;previous=mask;if(edges){history[cursor]={now,edges};cursor=(cursor+1)%history.size();}}
  unsigned combined=0, episodes=0;
  for(auto& e:history){if(e.mask&&now-e.time>=600)e={};if(e.mask){combined|=e.mask;++episodes;}}
  unsigned count=0;for(;combined;combined>>=1)count+=combined&1u;
  return {count,episodes,count>=3&&episodes>=2};
 }
};
