#pragma once
// Sticky failure: no automatic re-baselining or destructive response.
struct SelfGuard {
 bool trusted=true;
 bool update(bool baselineValid,bool readable,bool matches) noexcept {
  trusted=trusted&&baselineValid&&readable&&matches; return trusted;
 }
};
