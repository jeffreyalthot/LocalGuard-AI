#pragma once
#include <cstdint>
struct Incident {
 unsigned remaining=0;
 bool update(bool evidence, bool healthy) noexcept {
  if(evidence) remaining=30;
  else if(healthy && remaining) --remaining;
  return remaining!=0;
 }
};
