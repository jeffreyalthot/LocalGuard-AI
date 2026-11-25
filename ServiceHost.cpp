#include "ServiceHost.h"
#include <mutex>
namespace {
SERVICE_STATUS_HANDLE handle=nullptr;
SERVICE_STATUS status{};
std::mutex lock;
std::atomic<bool>* alive=nullptr;
int (*work)(bool)=nullptr;
void report(DWORD state,DWORD error=0){
 std::lock_guard<std::mutex> guard(lock);
 if(status.dwCurrentState==SERVICE_STOP_PENDING && state==SERVICE_RUNNING)return;
 status.dwServiceType=SERVICE_WIN32_OWN_PROCESS;
 status.dwCurrentState=state;
 status.dwControlsAccepted=state==SERVICE_RUNNING?SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN:0;
 status.dwWin32ExitCode=error;
 status.dwWaitHint=state==SERVICE_STOP_PENDING?30000:0;
 status.dwCheckPoint=state==SERVICE_STOP_PENDING?1:0;
 SetServiceStatus(handle,&status);
}
DWORD WINAPI control(DWORD code,DWORD,void*,void*){
 if(code==SERVICE_CONTROL_STOP||code==SERVICE_CONTROL_SHUTDOWN){report(SERVICE_STOP_PENDING);alive->store(false);return NO_ERROR;}
 if(code==SERVICE_CONTROL_INTERROGATE)return NO_ERROR;
 return ERROR_CALL_NOT_IMPLEMENTED;
}
void WINAPI entry(DWORD,LPWSTR*){
 handle=RegisterServiceCtrlHandlerExW(L"LocalGuard",control,nullptr);if(!handle)return;
 report(SERVICE_RUNNING);
 int rc=1;try{rc=work(false);}catch(...){rc=1;}
 report(SERVICE_STOPPED,rc?ERROR_GEN_FAILURE:NO_ERROR);
}
}
int runService(int (*worker)(bool),std::atomic<bool>& running){
 work=worker;alive=&running;
 wchar_t name[]=L"LocalGuard";
 SERVICE_TABLE_ENTRYW table[]={{name,entry},{nullptr,nullptr}};
 return StartServiceCtrlDispatcherW(table)?0:1;
}
