#pragma once
struct ResilienceInput { bool telemetry, etw, integrity, registry, journal; bool suspicious; unsigned cycleMs; };
struct ResiliencePlan { bool freeze, degraded, urgent; unsigned interval; };
class Resilience {
 unsigned quiet=0;
public:
 ResiliencePlan update(ResilienceInput x) noexcept {
  bool degraded=!(x.telemetry&&x.etw&&x.integrity&&x.registry&&x.journal);
  if(degraded||x.suspicious) quiet=0; else if(quiet<12) ++quiet;
  return {quiet<12,degraded,x.suspicious, x.cycleMs>500?5u:x.suspicious?2u:5u};
 }
};
