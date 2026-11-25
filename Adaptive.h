#pragma once
#include <array>
struct Evidence { int risk=0; bool behavior=false, file=false, registry=false, valid=true, etw=true; unsigned starts=0; };
struct Plan { int score; unsigned interval; bool freeze, urgent, degraded; };
class Adaptive {
 std::array<unsigned,12> history{}; unsigned pos=0, quiet=0; bool frozen=false;
public: Plan update(Evidence e) noexcept;
};
