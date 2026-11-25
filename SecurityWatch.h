#pragma once
#include <windows.h>
#include <winevt.h>
#include <string>
struct SecuritySignal { bool healthy=false, alert=false; unsigned count=0; };
class SecurityWatch {
 EVT_HANDLE handles[2]{}; HANDLE signals[2]{};
public:
 SecurityWatch(){for(auto& e:signals)e=CreateEventW(nullptr,TRUE,FALSE,nullptr);if(signals[0])handles[0]=EvtSubscribe(nullptr,signals[0],L"Security",L"*[System[(EventID=1102 or EventID=4720 or EventID=4732 or EventID=4625)]]",nullptr,nullptr,nullptr,EvtSubscribeToFutureEvents);if(signals[1])handles[1]=EvtSubscribe(nullptr,signals[1],L"Microsoft-Windows-Windows Defender/Operational",L"*[System[(EventID=1116 or EventID=5001 or EventID=5007)]]",nullptr,nullptr,nullptr,EvtSubscribeToFutureEvents);}
 ~SecurityWatch(){for(auto h:handles)if(h)EvtClose(h);for(auto e:signals)if(e)CloseHandle(e);}
 SecuritySignal poll(){SecuritySignal s;s.healthy=true;for(auto h:handles){if(!h){s.healthy=false;continue;} for(unsigned i=0;i<32;++i){EVT_HANDLE e{};DWORD n=0;if(!EvtNext(h,1,&e,0,0,&n)){auto err=GetLastError();if(err!=ERROR_TIMEOUT&&err!=ERROR_NO_MORE_ITEMS)s.healthy=false;break;}if(n){EvtClose(e);++s.count;s.alert=true;}if(i==31)s.healthy=false;}}return s;}
};
