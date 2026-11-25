#include "Events.h"
#include <evntcons.h>
#include <vector>
#include <cstring>
// Microsoft-Windows-Kernel-Process, event 1 = process start.
static const GUID provider={0x22fb2cd6,0x0e7b,0x422b,{0xa0,0xc7,0x2f,0xad,0x1f,0xd0,0xe7,0x16}};
static std::vector<unsigned char> properties(const std::wstring& name){
 std::vector<unsigned char> b(sizeof(EVENT_TRACE_PROPERTIES)+(name.size()+1)*sizeof(wchar_t));
 auto p=reinterpret_cast<EVENT_TRACE_PROPERTIES*>(b.data());
 p->Wnode.BufferSize=static_cast<ULONG>(b.size()); p->Wnode.Flags=WNODE_FLAG_TRACED_GUID; p->Wnode.ClientContext=1;
 p->LogFileMode=EVENT_TRACE_REAL_TIME_MODE; p->LoggerNameOffset=sizeof(EVENT_TRACE_PROPERTIES);
 p->BufferSize=64; p->MinimumBuffers=4; p->MaximumBuffers=16; p->FlushTimer=1;
 std::memcpy(b.data()+p->LoggerNameOffset,name.c_str(),(name.size()+1)*sizeof(wchar_t)); return b;
}
void WINAPI Events::callback(EVENT_RECORD* r){if(r->EventHeader.EventDescriptor.Id==1 && IsEqualGUID(r->EventHeader.ProviderId,provider))static_cast<Events*>(r->UserContext)->starts.fetch_add(1,std::memory_order_relaxed);}
bool Events::start(){
 name=L"LocalGuard-"+std::to_wstring(GetCurrentProcessId()); auto b=properties(name);
 if(StartTraceW(&session,name.c_str(),reinterpret_cast<EVENT_TRACE_PROPERTIES*>(b.data()))!=ERROR_SUCCESS){session=0;return false;}
 if(EnableTraceEx2(session,&provider,EVENT_CONTROL_CODE_ENABLE_PROVIDER,TRACE_LEVEL_INFORMATION,0x10,0,0,nullptr)!=ERROR_SUCCESS)return false;
 EVENT_TRACE_LOGFILEW f{}; f.LoggerName=name.data(); f.ProcessTraceMode=PROCESS_TRACE_MODE_REAL_TIME|PROCESS_TRACE_MODE_EVENT_RECORD; f.EventRecordCallback=callback; f.Context=this;
 consumer=OpenTraceW(&f); if(consumer==INVALID_PROCESSTRACE_HANDLE)return false;
 worker=std::thread([this]{ProcessTrace(&consumer,1,nullptr,nullptr);ended=true;});return true;
}
Events::~Events(){if(session){auto b=properties(name);ControlTraceW(session,name.c_str(),reinterpret_cast<EVENT_TRACE_PROPERTIES*>(b.data()),EVENT_TRACE_CONTROL_STOP);}if(consumer!=INVALID_PROCESSTRACE_HANDLE)CloseTrace(consumer);if(worker.joinable())worker.join();}
