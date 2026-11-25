#pragma once
#include <filesystem>
#include <fstream>
#include <array>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#else
#include <openssl/evp.h>
#endif
namespace recovery {
namespace fs=std::filesystem;
inline void require(bool b,const char* m){if(!b)throw std::runtime_error(m);}
inline bool validDigest(const std::string& s){return s.size()==64&&s.find_first_not_of("0123456789abcdef")==std::string::npos;}
inline void safePath(const fs::path& p){
 for(auto q=fs::absolute(p);!q.empty();){
  auto st=fs::symlink_status(q); require(!fs::is_symlink(st),"symbolic link refused");
#ifdef _WIN32
  auto a=GetFileAttributesW(q.c_str()); require(a!=INVALID_FILE_ATTRIBUTES&&!(a&FILE_ATTRIBUTE_REPARSE_POINT),"unavailable path or reparse point");
#endif
  auto parent=q.parent_path(); if(parent==q)break; q=parent;
 }
}
class Hash {
#ifdef _WIN32
 BCRYPT_ALG_HANDLE alg=nullptr; BCRYPT_HASH_HANDLE h=nullptr;
#else
 EVP_MD_CTX* h=nullptr;
#endif
public:
 Hash(){
#ifdef _WIN32
  if(BCryptOpenAlgorithmProvider(&alg,BCRYPT_SHA256_ALGORITHM,nullptr,0)<0)throw std::runtime_error("hash provider");
  if(BCryptCreateHash(alg,&h,nullptr,0,nullptr,0,0)<0){BCryptCloseAlgorithmProvider(alg,0);throw std::runtime_error("hash creation");}
#else
  h=EVP_MD_CTX_new(); if(!h||EVP_DigestInit_ex(h,EVP_sha256(),nullptr)!=1){EVP_MD_CTX_free(h);throw std::runtime_error("hash creation");}
#endif
 }
 Hash(const Hash&)=delete; Hash& operator=(const Hash&)=delete;
 ~Hash(){
#ifdef _WIN32
  BCryptDestroyHash(h); BCryptCloseAlgorithmProvider(alg,0);
#else
  EVP_MD_CTX_free(h);
#endif
 }
 void add(char* p,unsigned n){
#ifdef _WIN32
  require(BCryptHashData(h,reinterpret_cast<PUCHAR>(p),n,0)>=0,"hash update");
#else
  require(EVP_DigestUpdate(h,p,n)==1,"hash update");
#endif
 }
 std::string finish(){unsigned char d[32]{};
#ifdef _WIN32
  require(BCryptFinishHash(h,d,32,0)>=0,"hash finish");
#else
  unsigned n=0;require(EVP_DigestFinal_ex(h,d,&n)==1&&n==32,"hash finish");
#endif
  std::string s; for(auto b:d){s+="0123456789abcdef"[b>>4];s+="0123456789abcdef"[b&15];}return s;
 }
};
constexpr std::uintmax_t maxBytes=8ULL*1024*1024*1024;
inline std::string stream(const fs::path& src,std::ostream* out=nullptr){
 safePath(src);require(fs::is_regular_file(src),"regular file required");auto size=fs::file_size(src);auto stamp=fs::last_write_time(src);require(size<=maxBytes,"size limit exceeded");
 std::ifstream in(src,std::ios::binary);require(bool(in),"open input failed");Hash hash;std::array<char,65536> buf{};std::uintmax_t total=0;
 while(in){in.read(buf.data(),buf.size());auto n=in.gcount();if(n){total+=n;require(total<=size,"source grew");hash.add(buf.data(),static_cast<unsigned>(n));if(out){out->write(buf.data(),n);require(bool(*out),"write failed");}}}
 require(in.eof()&&!in.bad()&&total==size&&fs::file_size(src)==size&&fs::last_write_time(src)==stamp,"source changed or read failed");return hash.finish();
}
inline std::string copy(const fs::path& src,const fs::path& dir,const std::string& expected={}){
 require(expected.empty()||validDigest(expected),"invalid expected digest");safePath(src);require(fs::is_regular_file(src),"regular file required");auto parent=fs::absolute(dir).parent_path();safePath(parent);require(fs::is_directory(parent),"destination parent required");
 require(fs::file_size(src)<=maxBytes,"size limit exceeded");require(fs::space(parent).available>=fs::file_size(src),"insufficient free space");
 require(fs::create_directory(dir),"destination must be a new directory");auto partial=dir/"payload.partial";std::ofstream out(partial,std::ios::binary|std::ios::out);require(bool(out),"open destination failed");auto digest=stream(src,&out);out.close();require(bool(out),"close failed");
 require(expected.empty()||digest==expected,"trusted digest mismatch");require(stream(partial)==digest,"copy verification failed");fs::rename(partial,dir/"payload.bin");return digest;
}
}
