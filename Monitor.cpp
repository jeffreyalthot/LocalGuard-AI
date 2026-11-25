#include "Monitor.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include <vector>
Snapshot sample(){
 Snapshot s; HANDLE h=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
 if(h==INVALID_HANDLE_VALUE)return s;
 PROCESSENTRY32W p{}; p.dwSize=sizeof(p);
 if(!Process32FirstW(h,&p)){CloseHandle(h);return s;}
 do{++s.processes; char name[1024]{}; WideCharToMultiByte(CP_UTF8,0,p.szExeFile,-1,name,sizeof(name),nullptr,nullptr); s.identities.push_back({p.th32ProcessID,name});}while(Process32NextW(h,&p)); auto processError=GetLastError(); CloseHandle(h); if(processError!=ERROR_NO_MORE_FILES)return s;
 for(auto family:{AF_INET,AF_INET6}){
 DWORD n=0; GetExtendedTcpTable(nullptr,&n,FALSE,family,TCP_TABLE_OWNER_PID_ALL,0);
 if(!n || n>64*1024*1024)return s;
 std::vector<unsigned char> b(n);
 if(GetExtendedTcpTable(b.data(),&n,FALSE,family,TCP_TABLE_OWNER_PID_ALL,0)!=NO_ERROR)return s;
 char addr[INET6_ADDRSTRLEN]{};
 if(family==AF_INET){auto t=reinterpret_cast<MIB_TCPTABLE_OWNER_PID*>(b.data());
 for(DWORD i=0;i<t->dwNumEntries;++i){auto &r=t->table[i];if(r.dwState==MIB_TCP_STATE_ESTAB && InetNtopA(AF_INET,&r.dwRemoteAddr,addr,sizeof(addr)))s.connections.push_back({r.dwOwningPid,addr});}}
 else {auto t=reinterpret_cast<MIB_TCP6TABLE_OWNER_PID*>(b.data());
 for(DWORD i=0;i<t->dwNumEntries;++i){auto &r=t->table[i];if(r.dwState==MIB_TCP_STATE_ESTAB && InetNtopA(AF_INET6,r.ucRemoteAddr,addr,sizeof(addr)))s.connections.push_back({r.dwOwningPid,addr});}}
 }
 for(auto family:{AF_INET,AF_INET6}){
 DWORD n=0; GetExtendedUdpTable(nullptr,&n,FALSE,family,UDP_TABLE_OWNER_PID,0);
 if(!n || n>64*1024*1024)return s;
 std::vector<unsigned char> b(n);
 if(GetExtendedUdpTable(b.data(),&n,FALSE,family,UDP_TABLE_OWNER_PID,0)!=NO_ERROR)return s;
 if(family==AF_INET)s.udp+=reinterpret_cast<MIB_UDPTABLE_OWNER_PID*>(b.data())->dwNumEntries;
 else s.udp+=reinterpret_cast<MIB_UDP6TABLE_OWNER_PID*>(b.data())->dwNumEntries;
 } s.valid=true;return s;
}
