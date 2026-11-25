#pragma once
struct ProtectionInput { bool network=false, persistence=false, integrity=false, valid=false, enabled=false; unsigned score=0; };
// A reversible, user-authorized containment request, never an attribution.
inline bool requestContainment(ProtectionInput x) noexcept { return x.enabled && x.valid && x.network && (x.persistence || x.integrity) && x.score>=60; }
