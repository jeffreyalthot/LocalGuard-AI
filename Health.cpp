#include "Health.h"
std::string health(bool etw,bool valid){return !valid?"Health: incomplete":etw?"Health: ETW active":"Health: polling only";}
