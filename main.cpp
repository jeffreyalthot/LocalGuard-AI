#include "Fusion.h"
#include "Runtime.h"
#include "Behavior.h"
#include "Coordinator.h"
#include "Budget.h"
#include "Integrity.h"
#include "Exchange.h"
#include "Agents.h"
#include "Health.h"
#include "Response.h"
#include <winsock2.h>
#include <windows.h>
#include "Detector.h"
#include "Events.h"
#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <ctime>
#include <iomanip>
#include <filesystem>
std::atomic<bool> running{true};
BOOL WINAPI stop(DWORD t){if(t==CTRL_C_EVENT||t==CTRL_BREAK_EVENT){running=false;return TRUE;}return FALSE;}
void log(const std::string& msg){
 std::error_code ec; auto size=std::filesystem::file_size("guard.log",ec);
 if(!ec && size>1024*1024){std::filesystem::remove("guard.old.log",ec);ec.clear();std::filesystem::rename("guard.log","guard.old.log",ec);if(ec){std::cerr<<"Log rotation failed\n";return;}}
 std::ofstream f("guard.log",std::ios::app);auto now=std::time(nullptr);std::tm tm{};localtime_s(&tm,&now);
 std::cout<<msg<<std::endl;if(f)f<<std::put_time(&tm,"%F %T")<<" "<<msg<<'\n';else std::cerr<<"Log unavailable\n";
}
int main(){if(!setupRuntime()){std::cerr<<"Data directory unavailable\n";return 1;} HANDLE mutex=CreateMutexW(nullptr,FALSE,L"Local\\LocalGuardV20");if(!mutex||GetLastError()==ERROR_ALREADY_EXISTS){if(mutex)CloseHandle(mutex);return 2;} Behavior behavior; Fusion fusion; WSADATA w{};if(WSAStartup(MAKEWORD(2,2),&w))return 1;SetConsoleCtrlHandler(stop,TRUE);Detector d; Events events; bool etw=events.start(); std::string profile=etw?"profile-etw.dat":"profile-poll.dat"; if(d.load(profile))log("Profile loaded");else log("No valid profile; learning from local activity"); log(etw?"ETW enabled":"ETW unavailable; polling fallback");log("LocalGuard started; observation only");unsigned tick=0; bool fileAlert=false, registryAlert=false;
 while(running){auto cycleStart=std::chrono::steady_clock::now();if(etw && !events.healthy()){log("WARNING: ETW stopped; polling fallback");d.save(profile);etw=false;profile="profile-poll.dat";d=Detector{};d.load(profile);} unsigned n=etw?events.drain():0; auto s=sample(); bool behaviorAlert=false; if(tick%12==0){auto a=integrity();fileAlert=a.rfind("ALERT:",0)==0;log(a);auto b=startupChanges();registryAlert=b.rfind("ALERT:",0)==0;log(b);}if(!s.valid)log("WARNING: incomplete telemetry");else{for(auto& a:d.inspect(s,n))log("ALERT: "+a);auto ba=behavior.inspect(s,d.risk()==0 && !fileAlert && !registryAlert);behaviorAlert=!ba.empty();for(auto& a:ba)log("ALERT: "+a);if(d.risk()>0)log("Risk score: "+std::to_string(d.risk()));if(tick%12==0 && !d.save(profile))log("WARNING: profile save failed");if(tick%12==0)log("Heartbeat: processes="+std::to_string(s.processes)+" TCP="+std::to_string(s.connections.size())+" UDP="+std::to_string(s.udp));}if(tick%12==0 || (s.valid && d.risk()>=75)) log(responsePlan(d.risk(),s.valid));if(tick%12==0 || (s.valid && d.risk()>=75)) log(health(etw,s.valid));if(tick%12==0 || (s.valid && d.risk()>=75)) log(agents(s));if(tick%12==0 || (s.valid && d.risk()>=75)) log(exchange(d.risk(),s.valid));if(tick%12==0 || (s.valid && d.risk()>=75)) log(budget(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-cycleStart).count()));if(tick%12==0 || (s.valid && d.risk()>=75)) log(coordinate(d.risk(),s.valid,etw));if(tick%12==0){log(resources());}auto result=fusion.update(d.risk(),behaviorAlert,fileAlert,registryAlert,s.valid);if(result.degraded)log("Fusion: incomplete telemetry; score unavailable");else if(result.score||tick%12==0)log("Fusion risk: "+std::to_string(result.score)+(result.review?"; manual review recommended":""));++tick;
 for(int i=0;i<50&&running;++i)std::this_thread::sleep_for(std::chrono::milliseconds(100));}
 if(!d.save(profile))log("WARNING: profile save failed");log("LocalGuard stopped");WSACleanup();CloseHandle(mutex); }
