#include "Adaptive.h"
#include <algorithm>
Plan Adaptive::update(Evidence e) noexcept {
 unsigned bits=(e.valid&&e.risk>0?1u:0u)|(e.valid&&e.behavior?2u:0u)|(e.file?4u:0u)|(e.registry?8u:0u)|(e.etw&&e.starts>=30?16u:0u);
 history[pos++%history.size()]=bits; unsigned seen=0; for(auto b:history)seen|=b;
 int score=std::max(e.valid?std::clamp(e.risk,0,100):0, bits?25:0);
 if(seen&2)score+=20; if(seen&4)score+=35; if(seen&8)score+=30; if(seen&16)score+=25;
 if(seen&1)score=std::max(score,25); score=std::min(score,100);
 bool degraded=!e.valid||!e.etw;
 if(bits||!e.valid){frozen=true;quiet=0;}else if(quiet<12&&++quiet==12)frozen=false;
 return {score,score>=60?2u:5u,frozen||!e.valid,score>=60,degraded};
}
