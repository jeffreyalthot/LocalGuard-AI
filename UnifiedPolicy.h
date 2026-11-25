#pragma once
#include "CyclePolicy.h"
struct UnifiedInput {
 CycleInput sensors;
 bool evidence, urgent, adaptiveFreeze, resilienceFreeze, incidentHeld, stateOK, profileOK, activeEnabled;
};
struct UnifiedDecision { bool healthy, learn, scan, persistProfile; unsigned interval; };
class UnifiedPolicy {
 CyclePolicy cycle;
public:
 UnifiedDecision update(UnifiedInput x) noexcept {
  // Any trust or learning gate failure resets the quiet window.
  x.sensors.trusted=x.sensors.trusted&&x.stateOK&&x.profileOK;
  x.sensors.urgent=x.evidence||x.urgent||x.adaptiveFreeze||x.resilienceFreeze||x.incidentHeld;
  auto c=cycle.update(x.sensors);
  bool healthy=c.healthy&&x.stateOK&&x.profileOK;
  bool learn=healthy&&c.allowLearning&&!x.evidence&&!x.urgent&&!x.adaptiveFreeze&&!x.resilienceFreeze&&!x.incidentHeld;
  return {healthy,learn,healthy&&c.allowScan&&x.urgent&&x.activeEnabled,learn,healthy&&(x.evidence||x.urgent)?c.interval:5u};
 }
};
