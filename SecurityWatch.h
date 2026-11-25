#pragma once
#include <windows.h>
#include <winevt.h>
#include <cwchar>
#include "IdentityGuard.h"
struct SecuritySignal { bool healthy=false, alert=false; unsigned count=0; IdentitySignal identity; };
class SecurityWatch {
 EVT_HANDLE handles[2]{}, context{}, logonContext{}; HANDLE signals[2]{};
 void subscribe(unsigned i){if(!signals[i])signals[i]=CreateEventW(nullptr,TRUE,FALSE,nullptr);if(!signals[i])return;handles[i]=EvtSubscribe(nullptr,signals[i],i?L"Microsoft-Windows-Windows Defender/Operational":L"Security",i?L"*[System[(EventID=1116 or EventID=5001 or EventID=5007)]]":L"*[System[(EventID=1102 or EventID=4719 or EventID=4720 or EventID=4722 or EventID=4724 or EventID=4726 or EventID=4732 or EventID=4733 or EventID=4625 or EventID=4740 or EventID=4704 or EventID=4705 or EventID=4756 or EventID=4757)]]",nullptr,nullptr,nullptr,EvtSubscribeToFutureEvents);}
public:
 SecurityWatch(){context=EvtCreateRenderContext(0,nullptr,EvtRenderContextSystem);LPCWSTR path=L"Event/EventData/Data[@Name='LogonType']";logonContext=EvtCreateRenderContext(1,&path,EvtRenderContextValues);for(unsigned i=0;i<2;++i)subscribe(i);}
 ~SecurityWatch(){for(auto h:handles)if(h)EvtClose(h);for(auto e:signals)if(e)CloseHandle(e);if(context)EvtClose(context);if(logonContext)EvtClose(logonContext);}
 SecuritySignal poll(){SecuritySignal s;s.healthy=context!=nullptr&&logonContext!=nullptr;for(unsigned j=0;j<2;++j){if(!handles[j]){subscribe(j);s.healthy=false;continue;}for(unsigned i=0;i<32;++i){EVT_HANDLE e{};DWORD n=0;if(!EvtNext(handles[j],1,&e,0,0,&n)){auto err=GetLastError();if(err!=ERROR_TIMEOUT&&err!=ERROR_NO_MORE_ITEMS){s.healthy=false;EvtClose(handles[j]);handles[j]=nullptr;}break;}if(n){++s.count;if(j)s.alert=true;else{alignas(EVT_VARIANT) unsigned char buffer[4096]{};DWORD used=0,props=0;if(context&&EvtRender(context,e,EvtRenderEventValues,sizeof buffer,buffer,&used,&props)&&props>EvtSystemEventID){auto& v=reinterpret_cast<EVT_VARIANT*>(buffer)[EvtSystemEventID];if(v.Type!=EvtVarTypeUInt16)s.healthy=false;else if(v.UInt16Val==4625){++s.identity.failures;alignas(EVT_VARIANT) unsigned char b[256]{};DWORD u=0,p=0;if(logonContext&&EvtRender(logonContext,e,EvtRenderEventValues,sizeof b,b,&u,&p)&&p==1){auto &x=*reinterpret_cast<EVT_VARIANT*>(b);if(x.Type==EvtVarTypeString&&x.StringVal){if(wcscmp(x.StringVal,L"3")==0||wcscmp(x.StringVal,L"10")==0)++s.identity.remoteFailures;}else s.healthy=false;}else s.healthy=false;}else if(v.UInt16Val==4704||v.UInt16Val==4705||v.UInt16Val==4756||v.UInt16Val==4757)++s.identity.privileged;else if(v.UInt16Val==1102||v.UInt16Val==4719)++s.identity.audit;else ++s.identity.changes;}else s.healthy=false;}EvtClose(e);}if(i==31)s.healthy=false;}}s.identity.healthy=s.healthy;return s;}
};
