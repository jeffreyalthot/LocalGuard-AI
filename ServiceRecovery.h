#pragma once
#include <array>
constexpr std::array<unsigned,3> serviceRecoverySeconds{{60,300,0}};
constexpr unsigned serviceRecoveryResetSeconds=86400;
constexpr unsigned serviceExitCode(bool requestedStop,int workerResult) noexcept {
 return workerResult==0 && requestedStop ? 0u : 31u; // ERROR_GEN_FAILURE
}
