#pragma once
struct RecoveryInput { bool urgent, healthy, enabled; };
class Recovery {
 unsigned streak=0, cooldown=0;
public:
 bool update(RecoveryInput x) noexcept {
  if(cooldown) --cooldown;
  if(!x.urgent||!x.healthy||!x.enabled){streak=0;return false;}
  if(streak<3) ++streak;
  if(streak==3&&!cooldown){cooldown=900;streak=0;return true;}
  return false;
 }
};
