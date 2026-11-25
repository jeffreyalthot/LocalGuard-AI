#pragma once
#include <atomic>
#include <thread>
#include <string>
#include <windows.h>
#include <evntrace.h>
#include <evntcons.h>
class Events {
 TRACEHANDLE session=0, consumer=INVALID_PROCESSTRACE_HANDLE;
 std::wstring name;
 std::thread worker;
 std::atomic<unsigned> starts{0};
 std::atomic<bool> ended{false};
 static void WINAPI callback(EVENT_RECORD* record);
public:
 bool start();
 unsigned drain(){return starts.exchange(0);}
 bool healthy() const {return !ended.load();}
 ~Events();
};
