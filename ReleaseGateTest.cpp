#include "UnifiedPolicy.h"
#include "Recovery.h"
#include <cstdlib>
#include <iostream>
static void require(bool v){if(!v) std::abort();}
int main(){
 for(unsigned mask=0;mask<65536;++mask){
  auto bit=[&](unsigned n){return (mask&(1u<<n))!=0;};
  UnifiedInput x{{bit(0),bit(1),bit(2),bit(3),bit(4),bit(5),bit(6),bit(7),false,bit(15)?501u:0u},bit(8),bit(9),bit(10),bit(11),bit(12),bit(13),bit(14),true};
  UnifiedPolicy p; Recovery r;
  bool healthy=(mask&255)==255&&bit(13)&&bit(14);
  bool scan=healthy&&bit(9)&&!bit(15);
  bool learn=healthy&&!(mask&0x1f00)&&!bit(15);
  for(unsigned i=0;i<12;++i){auto d=p.update(x);require(d.healthy==healthy);require(d.scan==scan);require(d.learn==(learn&&i==11));require(d.persistProfile==d.learn);require(r.update({x.urgent,d.scan,true})==(scan&&i==2));}
 }
 std::cout<<"786432 integrated policy cycles passed\n";
}
