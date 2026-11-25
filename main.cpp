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
int main(){WSADATA w{};if(WSAStartup(MAKEWORD(2,2),&w))return 1;SetConsoleCtrlHandler(stop,TRUE);Detector d; if(d.load("profile.dat"))log("Profile loaded");else log("No valid profile; learning from local activity"); Events events; bool etw=events.start(); log(etw?"ETW enabled":"ETW unavailable; polling fallback");log("LocalGuard started; observation only");unsigned tick=0;
 while(running){if(etw && !events.healthy()){log("WARNING: ETW stopped; polling fallback");etw=false;} unsigned n=etw?events.drain():0; auto s=sample();if(!s.valid)log("WARNING: incomplete telemetry");else{for(auto& a:d.inspect(s,n))log("ALERT: "+a);if(d.risk()>0)log("Risk score: "+std::to_string(d.risk()));if(tick%12==0 && !d.save("profile.dat"))log("WARNING: profile save failed");if(tick%12==0)log("Heartbeat: processes="+std::to_string(s.processes)+" TCP="+std::to_string(s.connections.size())+" UDP="+std::to_string(s.udp));}if(tick%12==0 || (s.valid && d.risk()>=75)) log(responsePlan(d.risk(),s.valid));++tick;
 for(int i=0;i<50&&running;++i)std::this_thread::sleep_for(std::chrono::milliseconds(100));}
 if(!d.save("profile.dat"))log("WARNING: profile save failed");log("LocalGuard stopped");WSACleanup();}
