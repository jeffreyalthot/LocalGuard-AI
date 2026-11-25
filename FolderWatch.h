#pragma once
#include "RansomGuard.h"
#include <windows.h>
#include <array>
#include <cstddef>
#include <cstring>
#include <string>
// One overlapped request; no permanent worker thread. Nonrecursive by design.
class FolderWatch {
 HANDLE dir=INVALID_HANDLE_VALUE, event=nullptr; OVERLAPPED ov{};
 alignas(DWORD) std::array<unsigned char,32768> buffer{};
 bool pending=false;
 bool arm(){ResetEvent(event); pending=ReadDirectoryChangesW(dir,buffer.data(),static_cast<DWORD>(buffer.size()),FALSE,FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SIZE,nullptr,&ov,nullptr)!=FALSE;return pending;}
public:
 FolderWatch()=default; FolderWatch(const FolderWatch&)=delete; FolderWatch& operator=(const FolderWatch&)=delete;
 ~FolderWatch(){if(pending){CancelIoEx(dir,&ov);DWORD n;GetOverlappedResult(dir,&ov,&n,TRUE);}if(dir!=INVALID_HANDLE_VALUE)CloseHandle(dir);if(event)CloseHandle(event);}
 bool start(const wchar_t* path){if(dir!=INVALID_HANDLE_VALUE)return false;dir=CreateFileW(path,FILE_LIST_DIRECTORY,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,nullptr,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED|FILE_FLAG_OPEN_REPARSE_POINT,nullptr);if(dir==INVALID_HANDLE_VALUE)return false;BY_HANDLE_FILE_INFORMATION info{};if(!GetFileInformationByHandle(dir,&info)||(info.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return false;event=CreateEventW(nullptr,TRUE,FALSE,nullptr);if(!event)return false;ov.hEvent=event;return arm();}
 FileSignals poll(){FileSignals s{};if(!pending){s.healthy=false;return s;}DWORD n=0;if(!GetOverlappedResult(dir,&ov,&n,FALSE)){if(GetLastError()==ERROR_IO_INCOMPLETE)return s;pending=false;s.healthy=false;arm();return s;}pending=false;if(!n||n>buffer.size())s.healthy=false;else{size_t p=0;while(p<n){constexpr size_t header=offsetof(FILE_NOTIFY_INFORMATION,FileName);if(n-p<header){s.healthy=false;break;}DWORD next,action,len;std::memcpy(&next,buffer.data()+p,4);std::memcpy(&action,buffer.data()+p+4,4);std::memcpy(&len,buffer.data()+p+8,4);if(len>n-p-header||len%2){s.healthy=false;break;}if(action==FILE_ACTION_MODIFIED||action==FILE_ACTION_ADDED)++s.writes;else if(action==FILE_ACTION_REMOVED)++s.deletes;else if(action==FILE_ACTION_RENAMED_NEW_NAME)++s.renames; if(!next)break;if(next<header+len||next>=n-p||next%4){s.healthy=false;break;}p+=next;}}if(!arm())s.healthy=false;return s;}
};
