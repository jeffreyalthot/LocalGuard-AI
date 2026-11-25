#include "Integrity.h"
#include <filesystem>
#include <fstream>
#include <cstdint>
std::string integrity(){std::error_code ec;auto n=std::filesystem::file_size("watch.txt",ec);if(ec||n>1048576)return "Integrity: watch.txt absent or oversized";std::ifstream f("watch.txt",std::ios::binary);std::uint64_t h=14695981039346656037ull;char c;unsigned count=0;while(count<1048576&&f.get(c)){h^=static_cast<unsigned char>(c);h*=1099511628211ull;++count;}if(f.bad()||count!=n)return "Integrity: read incomplete";static bool init=false;static std::uint64_t baseline=0;if(!init){baseline=h;init=true;}return h==baseline?"Integrity: unchanged":"Integrity: watch.txt changed";}
