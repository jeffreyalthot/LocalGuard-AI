#include "Response.h"
std::string responsePlan(int risk,bool valid){if(!valid)return "Telemetry degraded; no action";if(risk>=75)return "Review incident; consider scoped firewall block manually";return "Observe";}
