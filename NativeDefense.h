#pragma once
#include <string>
#include <windows.h>
// Fixed command, no untrusted arguments; nonblocking and at most one child.
class NativeDefense {
 HANDLE child=nullptr;
public:
 ~NativeDefense(){if(child)CloseHandle(child);}
 int poll(){if(!child)return -1; if(WaitForSingleObject(child,0)!=WAIT_OBJECT_0)return -1; DWORD code=1;GetExitCodeProcess(child,&code);CloseHandle(child);child=nullptr;return code==0?0:1;}
 bool scan(){if(child)return false; wchar_t dir[MAX_PATH]{}; auto n=GetSystemDirectoryW(dir,MAX_PATH);if(!n||n>=MAX_PATH)return false; std::wstring exe=std::wstring(dir)+L"\\WindowsPowerShell\\v1.0\\powershell.exe"; std::wstring cmd=L"\""+exe+L"\" -NoProfile -NonInteractive -Command \"$ErrorActionPreference='Stop'; Start-MpScan -ScanType QuickScan\""; STARTUPINFOW si{};si.cb=sizeof(si);PROCESS_INFORMATION pi{};if(!CreateProcessW(exe.c_str(),cmd.data(),nullptr,nullptr,FALSE,CREATE_NO_WINDOW,nullptr,nullptr,&si,&pi))return false;CloseHandle(pi.hThread);child=pi.hProcess;return true;}
};
