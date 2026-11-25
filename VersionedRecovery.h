#pragma once
#include "RecoveryCopy.h"
namespace recovery {
inline bool generationName(const std::string& s){return !s.empty()&&s.size()<=48&&s.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_")==std::string::npos;}
inline fs::path generation(const fs::path& root,const std::string& id){require(generationName(id),"invalid generation ID");safePath(root);require(fs::is_directory(root),"repository must exist");return root/("generation-"+id);}
inline std::string versionBackup(const fs::path& src,const fs::path& root,const std::string& id){auto dir=generation(root,id);auto digest=recovery::copy(src,dir);std::ofstream out(dir/"digest.partial",std::ios::binary);out<<digest<<'\n';out.close();require(bool(out),"manifest write failed");fs::rename(dir/"digest.partial",dir/"digest.sha256");return digest;}
inline std::string versionVerify(const fs::path& root,const std::string& id,const std::string& trusted){require(validDigest(trusted),"external trusted digest required");auto dir=generation(root,id);safePath(dir/"digest.sha256");require(fs::is_regular_file(dir/"digest.sha256")&&fs::file_size(dir/"digest.sha256")==65,"invalid manifest");std::ifstream in(dir/"digest.sha256",std::ios::binary);std::string digest;std::getline(in,digest);require(digest==trusted&&stream(dir/"payload.bin")==trusted,"generation verification failed");return digest;}
inline std::string versionRestore(const fs::path& root,const std::string& id,const fs::path& dest,const std::string& trusted){versionVerify(root,id,trusted);return copy(generation(root,id)/"payload.bin",dest,trusted);}
}
