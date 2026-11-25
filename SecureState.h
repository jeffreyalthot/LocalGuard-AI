#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <wincrypt.h>
// DPAPI user-bound authenticated envelope. No plaintext temporary files.
inline bool secureWrite(const std::filesystem::path& p,const std::string& text){
 if(text.size()>65536)return false;
 DATA_BLOB in{static_cast<DWORD>(text.size()),reinterpret_cast<BYTE*>(const_cast<char*>(text.data()))},out{};
 if(!CryptProtectData(&in,L"LocalGuard state",nullptr,nullptr,nullptr,CRYPTPROTECT_UI_FORBIDDEN,&out))return false;
 auto tmp=p;tmp+=L".tmp"; std::ofstream f(tmp,std::ios::binary|std::ios::trunc);f.write(reinterpret_cast<const char*>(out.pbData),out.cbData);f.close();LocalFree(out.pbData);
 if(!f)return false;
 return MoveFileExW(tmp.c_str(),p.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)!=0;
}
inline bool secureRead(const std::filesystem::path& p,std::string& text){
 std::error_code ec;auto size=std::filesystem::file_size(p,ec);if(ec||!size||size>131072)return false;
 std::vector<BYTE> buf(static_cast<size_t>(size));std::ifstream f(p,std::ios::binary);if(!f.read(reinterpret_cast<char*>(buf.data()),buf.size()))return false;
 DATA_BLOB in{static_cast<DWORD>(buf.size()),buf.data()},out{};
 if(!CryptUnprotectData(&in,nullptr,nullptr,nullptr,nullptr,CRYPTPROTECT_UI_FORBIDDEN,&out))return false;
 bool ok=out.cbData<=65536;if(ok)text.assign(reinterpret_cast<char*>(out.pbData),out.cbData);LocalFree(out.pbData);return ok;
}
