#include "Decision.h"
Action decide(FusionResult r) noexcept {
 if(r.score>=60)return Action::UrgentReview;
 if(r.review||r.score>0)return Action::Review;
 if(r.degraded)return Action::RestoreVisibility;
 return Action::Observe;
}
const char* actionText(Action a) noexcept {switch(a){case Action::UrgentReview:return "Urgent review: preserve logs, verify with Defender, consider manual isolation if confirmed";case Action::Review:return "Review evidence and verify with Defender";case Action::RestoreVisibility:return "Restore sensor visibility; security status unknown";default:return "Observe";}}
