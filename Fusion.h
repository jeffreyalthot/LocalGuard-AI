#pragma once
#include <array>
struct FusionResult { int score; bool degraded; bool review; };
class Fusion {
 std::array<int,4> history{}; unsigned pos=0;
public:
 FusionResult update(int global,bool behavior,bool integrity,bool registry,bool valid);
};
