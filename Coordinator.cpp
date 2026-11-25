#include "Coordinator.h"
std::string coordinate(int risk,bool valid,bool etw){if(!valid)return "Coordinator: degraded; review sensors";if(risk>=75)return "Coordinator: urgent human review";return etw?"Coordinator: observing":"Coordinator: limited visibility";}
