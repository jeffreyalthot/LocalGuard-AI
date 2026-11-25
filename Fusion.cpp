#include "Fusion.h"
#include <algorithm>
FusionResult Fusion::update(int global,bool behavior,bool integrity,bool registry,bool valid){
 if(!valid){history.fill(0);return {0,true,false};}
 int score=std::clamp(global,0,100); score=std::min(100,score+(behavior?25:0)+(integrity?35:0)+(registry?30:0));
 history[pos++%history.size()]=score; unsigned elevated=0;for(int x:history)if(x>=35)++elevated;
 if(score>=35&&elevated>=2)score=std::min(100,score+15);
 return {score,false,score>=60};
}
