#include "Incident.h"
#include "Recovery.h"
#include "Resilience.h"
#include "Protection.h"
#include "DataDefense.h"
#include <unordered_set>
#include "Adaptive.h"
#include "Decision.h"
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
#include "NativeDefense.h"
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
bool journalOK=true;
BOOL WINAPI stop(DWORD t){if(t==CTRL_C_EVENT||t==CTRL_BREAK_EVENT){running=false;return TRUE;}return FALSE;}
void log(const std::string& msg){
 std::error_code ec; auto size=std::filesystem::file_size("guard.log",ec);
 if(!ec && size>1024*1024){std::filesystem::remove("guard.old.log",ec);ec.clear();std::filesystem::rename("guard.log","guard.old.log",ec);if(ec){journalOK=false;std::cerr<<"Log rotation failed\n";return;}}
 std::ofstream f("guard.log",std::ios::app);auto now=std::time(nullptr);std::tm tm{};localtime_s(&tm,&now);
 std::cout<<msg<<std::endl;if(f){f<<std::put_time(&tm,"%F %T")<<" "<<msg<<'\n';f.flush();} if(!f){journalOK=false;std::cerr<<"Log unavailable\n";}
}
int main(int argc,char** argv){bool autoScan=argc==2 && std::string(argv[1])=="--auto-scan"; Recovery recovery; Incident incident; NativeDefense native;if(!setupRuntime()){std::cerr<<"Data directory unavailable\n";return 1;} HANDLE mutex=CreateMutexW(nullptr,FALSE,L"Local\\LocalGuardV20");if(!mutex||GetLastError()==ERROR_ALREADY_EXISTS){if(mutex)CloseHandle(mutex);return 2;} Behavior behavior; Fusion fusion; Adaptive adaptive; DataDefense dataDefense; std::unordered_set<std::string> previousPeers; bool peersReady=false; bool freeze=true; Resilience resilience; bool integrityOK=false, registryOK=false; WSADATA w{};if(WSAStartup(MAKEWORD(2,2),&w))return 1;SetConsoleCtrlHandler(stop,TRUE);Detector d; Events events; bool etw=events.start(); std::string profile=etw?"profile-etw.dat":"profile-poll.dat"; if(d.load(profile))log("Profile loaded");else log("No valid profile; learning from local activity"); log(etw?"ETW enabled":"ETW unavailable; polling fallback");log(autoScan?"LocalGuard started; automatic Defender quick scan enabled":"LocalGuard started; observation only");unsigned tick=0; bool fileAlert=false, registryAlert=false;
 while(running){auto cycleStart=std::chrono::steady_clock::now();if(etw && !events.healthy()){log("WARNING: ETW stopped; polling fallback");d.save(profile);etw=false;profile="profile-poll.dat";d=Detector{};d.load(profile);} unsigned n=etw?events.drain():0; auto s=sample(); bool behaviorAlert=false; fileAlert=false;registryAlert=false;if(tick%12==0){auto a=integrity();fileAlert=a.rfind("ALERT:",0)==0; integrityOK=fileAlert||a=="Integrity: SHA256 unchanged"||a=="Integrity: SHA256 baseline initialized";log(a);auto b=startupChanges();registryAlert=b.rfind("ALERT:",0)==0; registryOK=registryAlert||b=="Registry: startup values unchanged"||b=="Registry: startup baseline initialized";log(b);}if(!s.valid)log("WARNING: incomplete telemetry");else{for(auto& a:d.inspect(s,n,!freeze && journalOK && etw && integrityOK && registryOK && !fileAlert && !registryAlert))log("ALERT: "+a);auto ba=behavior.inspect(s,!freeze && journalOK && etw && integrityOK && registryOK && d.risk()==0 && !fileAlert && !registryAlert);behaviorAlert=!ba.empty();for(auto& a:ba)log("ALERT: "+a);if(d.risk()>0)log("Risk score: "+std::to_string(d.risk()));if(tick%12==0 && !d.save(profile))log("WARNING: profile save failed");if(tick%12==0)log("Heartbeat: processes="+std::to_string(s.processes)+" TCP="+std::to_string(s.connections.size())+" UDP="+std::to_string(s.udp));}if(tick%12==0 || (s.valid && d.risk()>=75)) log(responsePlan(d.risk(),s.valid));if(tick%12==0 || (s.valid && d.risk()>=75)) log(health(etw,s.valid));if(tick%12==0 || (s.valid && d.risk()>=75)) log(agents(s));if(tick%12==0 || (s.valid && d.risk()>=75)) log(exchange(d.risk(),s.valid));if(tick%12==0 || (s.valid && d.risk()>=75)) log(budget(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-cycleStart).count()));if(tick%12==0 || (s.valid && d.risk()>=75)) log(coordinate(d.risk(),s.valid,etw));if(tick%12==0){log(resources());}std::unordered_set<std::string> peers; unsigned newPeers=0; bool peerValid=s.valid && s.connections.size()<=16384; if(peerValid){for(const auto& c:s.connections)peers.insert(c.remote); if(peersReady)for(const auto& p:peers)if(!previousPeers.count(p))++newPeers; previousPeers=std::move(peers);peersReady=true;}else{peersReady=false;previousPeers.clear();} auto dp=dataDefense.update({static_cast<unsigned>(previousPeers.size()),newPeers,peerValid,registryAlert,fileAlert,behaviorAlert||(s.valid&&d.risk()>0)}); if(dp.review)log("DataDefense: correlated risk="+std::to_string(dp.score)+(dp.urgent?"; urgent review: verify destinations, preserve logs, isolate manually if confirmed":"; investigate unusual activity")); if(requestContainment({newPeers>=20,registryAlert,fileAlert,peerValid,true,dp.score}))log("CONTAINMENT CANDIDATE: verify executable identity; use contain.cmd only after confirmation"); if(dp.degraded)log("DataDefense: peer visibility degraded"); auto result=fusion.update(d.risk(),behaviorAlert,fileAlert,registryAlert,s.valid);if(result.degraded)log("Fusion: incomplete telemetry; partial evidence only");if(result.score||tick%12==0)log("Fusion risk: "+std::to_string(result.score)+(result.review?"; manual review recommended":""));if(result.review || result.degraded || tick%12==0)log(actionText(decide(result)));auto plan=adaptive.update({d.risk(),behaviorAlert,fileAlert,registryAlert,s.valid,etw,n}); freeze=plan.freeze||dp.freeze; if(dp.urgent)plan.interval=2; bool held=incident.update(plan.urgent||result.review||dp.review||fileAlert||registryAlert,s.valid&&etw&&integrityOK&&registryOK&&journalOK); freeze=freeze||held; auto rp=resilience.update({s.valid,etw,integrityOK,registryOK,journalOK,plan.urgent||result.review||dp.review||behaviorAlert||fileAlert||registryAlert,static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-cycleStart).count())}); freeze=freeze||rp.freeze; plan.interval=rp.interval; if(held && tick%12==0)log("Incident memory: learning held pending healthy cycles"); if(rp.degraded && tick%12==0)log("Resilience: degraded coverage; learning paused"); if(plan.score||tick%12==0)log("Adaptive: score="+std::to_string(plan.score)+" learning="+(freeze?"paused":"enabled")+(plan.urgent?"; urgent manual investigation":"")); int scanResult=native.poll(); if(scanResult>=0)log(scanResult==0?"Defender scan command completed":"WARNING: Defender scan command failed"); if(recovery.update({dp.urgent||plan.urgent,!rp.degraded,autoScan}))log(native.scan()?"Defender quick scan requested":"WARNING: Defender scan could not start"); ++tick;
 for(unsigned i=0;i<plan.interval*10&&running;++i)std::this_thread::sleep_for(std::chrono::milliseconds(100));}
 if(!d.save(profile))log("WARNING: profile save failed");log("LocalGuard stopped");WSACleanup();CloseHandle(mutex); }
