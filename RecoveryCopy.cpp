#include "RecoveryCopy.h"
#include <cstring>
#ifdef _WIN32
int wmain(int argc,wchar_t** argv){
#else
int main(int argc,char** argv){
#endif
 try{
  if(argc<3)throw std::runtime_error("Usage: RecoveryCopy verify FILE SHA256 | backup FILE NEW_DIR | restore FILE NEW_DIR SHA256");
  auto mode=std::filesystem::path(argv[1]).string();
  if(mode=="verify"&&argc==4){auto expected=std::filesystem::path(argv[3]).string();recovery::require(recovery::validDigest(expected)&&recovery::stream(argv[2])==expected,"verification failed");std::cout<<"Verified\n";}
  else if(mode=="backup"&&argc==4)std::cout<<"SHA256 "<<recovery::copy(argv[2],argv[3])<<'\n';
  else if(mode=="restore"&&argc==5){auto expected=std::filesystem::path(argv[4]).string();recovery::require(recovery::validDigest(expected),"trusted SHA256 required");std::cout<<"SHA256 "<<recovery::copy(argv[2],argv[3],expected)<<'\n';}
  else throw std::runtime_error("invalid arguments");return 0;
 }catch(const std::exception& e){std::cerr<<"Recovery failed: "<<e.what()<<". Do not use partial output.\n";return 1;}
}
