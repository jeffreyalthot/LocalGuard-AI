#pragma once
struct LearningGate { bool priorFreeze, healthy, suspicious; constexpr bool allow() const noexcept {return !priorFreeze && healthy && !suspicious;} };
