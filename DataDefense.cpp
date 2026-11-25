#include "DataDefense.h"
#include <algorithm>
DataPlan DataDefense::update(DataSignals s) noexcept {
 bool fanout=s.valid && (s.newPeers>=20 || (learned>=20 && s.newPeers>=8 && s.newPeers>baseline*4+4));
 unsigned bits=(fanout?1u:0u)|(s.persistence?2u:0u)|(s.integrity?4u:0u)|(s.valid&&s.anomaly?8u:0u);
 history[pos++%history.size()]=bits; unsigned seen=0; for(auto b:history)seen|=b;
 unsigned score=(seen&1?30:0)+(seen&2?30:0)+(seen&4?35:0)+(seen&8?25:0);
 if(s.valid && !seen){ if(learned<20){baseline=(baseline*learned+s.newPeers)/(learned+1);++learned;}else baseline=baseline*.995+s.newPeers*.005; }
 return {std::min(100u,score),score>0,score>=60,score>0||!s.valid,!s.valid};
}
