#pragma once
#include "Monitor.h"
class Detector {
 unsigned count=0; double proc=0,net=0;
public:
 std::vector<std::string> inspect(const Snapshot& s);
};
