#pragma once
#include <cstdint>
struct CycleInput { bool telemetry, attribution, security, files, integrity, registry, journal, trusted, urgent; std::uint64_t elapsedMs; };
struct CycleDecision { bool healthy, allowLearning, allowScan; unsigned interval; };
class CyclePolicy {
 unsigned quiet=0, overload=0;
public:
 CycleDecision update(CycleInput x) noexcept {
  bool healthy=x.telemetry&&x.attribution&&x.security&&x.files&&x.integrity&&x.registry&&x.journal&&x.trusted;
  if(x.elapsedMs>500) overload=3; else if(overload) --overload;
  if(!healthy||x.urgent||overload) quiet=0; else if(quiet<12) ++quiet;
  return {healthy,healthy&&quiet==12,healthy&&!overload,overload?5u:x.urgent?2u:5u};
 }
};
