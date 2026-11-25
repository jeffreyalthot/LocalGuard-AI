#include "Budget.h"
std::string budget(long long millis){return "Cycle processing ms="+std::to_string(millis)+(millis>1000?"; over target":"");}
