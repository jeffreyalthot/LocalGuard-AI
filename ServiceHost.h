#pragma once
#include <windows.h>
#include <atomic>
int runService(int (*worker)(bool), std::atomic<bool>& running);
