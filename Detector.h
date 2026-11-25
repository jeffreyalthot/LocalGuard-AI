#pragma once
#include "Monitor.h"
#include <array>
#include <filesystem>
class Detector {
 unsigned count=0; std::array<double,4> mean{}, variance{};
 int risk_=0;
public:
 std::vector<std::string> inspect(const Snapshot& s, unsigned starts=0, bool learn=true);
 int risk() const {return risk_;}
 unsigned learned() const {return count;}
 bool save(const std::filesystem::path&) const;
 bool load(const std::filesystem::path&);
};
