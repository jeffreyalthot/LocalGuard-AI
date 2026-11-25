#include "Fusion.h"
#include <algorithm>
FusionResult Fusion::update(int global,bool behavior,bool integrity,bool registry,bool valid){
 int score=std::clamp(valid?global:0,0,100);
 score=std::min(100,score+(valid&&behavior?25:0)+(integrity?35:0)+(registry?30:0));
 // Every positive signal is reviewable, even when another sensor is unavailable.
 return {score,!valid,score>0};
}
