#include <winsock2.h>
#include <windows.h>
#include "Detector.h"
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
int main(){WSADATA w{};if(WSAStartup(MAKEWORD(2,2),&w))return 1;SetConsoleCtrlHandler(stop,TRUE);Detector d;log("LocalGuard started; observation only");unsigned tick=0;
 while(running){auto s=sample();if(!s.valid)log("WARNING: incomplete telemetry");else{for(auto& a:d.inspect(s))log("ALERT: "+a);if(tick%12==0)log("Heartbeat: processes="+std::to_string(s.processes)+" TCP="+std::to_string(s.connections.size()));}++tick;
 for(int i=0;i<50&&running;++i)std::this_thread::sleep_for(std::chrono::milliseconds(100));}
 log("LocalGuard stopped");WSACleanup();}
