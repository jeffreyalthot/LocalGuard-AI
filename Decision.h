#pragma once
#include "Fusion.h"
enum class Action { Observe, RestoreVisibility, Review, UrgentReview };
Action decide(FusionResult r) noexcept;
const char* actionText(Action a) noexcept;
