#pragma once
#include <filesystem>
// A corrupt existing profile must never silently become a fresh baseline.
struct StateGuard {
 bool trusted=true;
 bool inspect(bool exists,bool readable,bool ioError=false) noexcept {
  trusted=trusted&&!ioError&&(!exists||readable); return trusted;
 }
 bool allow() const noexcept {return trusted;}
};
inline bool checkProfile(StateGuard& g,const std::filesystem::path& p,bool loaded){
 std::error_code ec; bool exists=std::filesystem::exists(p,ec);
 return g.inspect(exists,loaded,!!ec);
}
