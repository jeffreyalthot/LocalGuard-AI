#include "Integrity.h"
#include <windows.h>
#include <bcrypt.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <array>
#include <iomanip>
#include <sstream>
std::string integrity(){std::error_code ec;auto n=std::filesystem::file_size("watch.txt",ec);if(ec||n>1048576)return "Integrity: watch.txt absent or oversized";std::ifstream f("watch.txt",std::ios::binary);if(!f)return "Integrity: read failed";BCRYPT_ALG_HANDLE alg=nullptr;BCRYPT_HASH_HANDLE hash=nullptr;DWORD size=0,got=0;std::array<unsigned char,32> digest{};bool ok=BCryptOpenAlgorithmProvider(&alg,BCRYPT_SHA256_ALGORITHM,nullptr,0)>=0; if(ok)ok=BCryptGetProperty(alg,BCRYPT_OBJECT_LENGTH,reinterpret_cast<PUCHAR>(&size),sizeof(size),&got,0)>=0&&size>0&&size<1048576;std::vector<unsigned char> obj(ok?size:0);if(ok)ok=BCryptCreateHash(alg,&hash,obj.data(),size,nullptr,0,0)>=0;std::array<char,8192> buf{};size_t total=0;while(ok&&f.read(buf.data(),buf.size()).gcount()>0){auto k=f.gcount();total+=k;if(total>1048576){ok=false;break;}ok=BCryptHashData(hash,reinterpret_cast<PUCHAR>(buf.data()),static_cast<ULONG>(k),0)>=0;}if(ok)ok=!f.bad()&&total==n&&BCryptFinishHash(hash,digest.data(),digest.size(),0)>=0;if(hash)BCryptDestroyHash(hash);if(alg)BCryptCloseAlgorithmProvider(alg,0);if(!ok)return "Integrity: SHA256 failed";std::ostringstream hex;for(auto c:digest)hex<<std::hex<<std::setw(2)<<std::setfill('0')<<unsigned(c);auto value=hex.str();static std::string baseline;if(baseline.empty()){baseline=value;return "Integrity: SHA256 baseline initialized";}return value==baseline?"Integrity: SHA256 unchanged":"ALERT: watch.txt SHA256 changed";}
