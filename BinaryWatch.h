#pragma once
#include "SelfGuard.h"
#include "SecureState.h"
#include <bcrypt.h>
#include <array>
#include <iomanip>
#include <sstream>
// Bounded streaming hash, no file modifications. Baseline must be provisioned externally.
inline bool binaryDigest(std::string& result){
 wchar_t path[32768]{}; DWORD n=GetModuleFileNameW(nullptr,path,32768);if(!n||n>=32768)return false;
 HANDLE f=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_SEQUENTIAL_SCAN,nullptr);if(f==INVALID_HANDLE_VALUE)return false;
 BY_HANDLE_FILE_INFORMATION info{}; LARGE_INTEGER size{}; bool ok=GetFileInformationByHandle(f,&info)&&!(info.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY))&&GetFileSizeEx(f,&size)&&size.QuadPart>0&&size.QuadPart<=64LL*1024*1024;
 BCRYPT_ALG_HANDLE alg=nullptr; BCRYPT_HASH_HANDLE hash=nullptr; DWORD len=0,got=0; std::array<BYTE,32> digest{};
 if(ok)ok=BCryptOpenAlgorithmProvider(&alg,BCRYPT_SHA256_ALGORITHM,nullptr,0)>=0;
 if(ok)ok=BCryptGetProperty(alg,BCRYPT_OBJECT_LENGTH,reinterpret_cast<PUCHAR>(&len),sizeof(len),&got,0)>=0&&len>0&&len<=65536;
 std::vector<BYTE> obj(ok?len:0); if(ok)ok=BCryptCreateHash(alg,&hash,obj.data(),len,nullptr,0,0)>=0;
 std::array<BYTE,16384> buf{}; long long total=0;
 while(ok){DWORD read=0;if(!ReadFile(f,buf.data(),static_cast<DWORD>(buf.size()),&read,nullptr)){ok=false;break;}if(!read)break;total+=read;if(total>size.QuadPart){ok=false;break;}ok=BCryptHashData(hash,buf.data(),read,0)>=0;}
 if(ok)ok=total==size.QuadPart&&BCryptFinishHash(hash,digest.data(),static_cast<ULONG>(digest.size()),0)>=0;
 if(hash)BCryptDestroyHash(hash);if(alg)BCryptCloseAlgorithmProvider(alg,0);CloseHandle(f);
 if(!ok)return false;std::ostringstream out;for(auto c:digest)out<<std::hex<<std::setw(2)<<std::setfill('0')<<unsigned(c);result=out.str();return true;
}
inline bool readExpectedDigest(std::string& value){std::ifstream f("binary.sha256",std::ios::binary);if(!f)return false;value.resize(64);if(!f.read(value.data(),64))return false;unsigned extraGuard=0;char c;while(f.get(c)){if(c!='\r'&&c!='\n')return false;if(++extraGuard>2)return false;}return f.eof()&&!f.bad()&&value.find_first_not_of("0123456789abcdef")==std::string::npos;}
