#pragma once
#include <array>
struct DataSignals { unsigned peers=0, newPeers=0; bool valid=true, persistence=false, integrity=false, anomaly=false; };
struct DataPlan { unsigned score=0; bool review=false, urgent=false, freeze=false, degraded=false; };
class DataDefense {
 std::array<unsigned,12> history{}; unsigned pos=0, learned=0; double baseline=0;
public: DataPlan update(DataSignals s, bool learn=true) noexcept;
};
