#pragma once
struct ContainmentInput { bool telemetry, attributed, corroborated, confirmed, identityVerified, protectedTarget; unsigned minutes; };
inline bool containmentAllowed(ContainmentInput x) noexcept { return x.telemetry && x.attributed && x.corroborated && x.confirmed && x.identityVerified && !x.protectedTarget && x.minutes>=1 && x.minutes<=15; }
