#include "Decision.h"
#include <cstdint>
#include <iostream>
#include <cstdlib>
int main(int argc,char**argv){const uint64_t n=argc>1?std::strtoull(argv[1],nullptr,10):100000000ULL; if(!n||n>100000000ULL)return 2; uint64_t tp=0,tn=0,fp=0,fn=0,wrong=0; Fusion f;
// Unique input records: index is a synthetic case ID; observable signal combinations repeat.
for(uint64_t i=0;i<n;++i){uint64_t x=i+0x9e3779b97f4a7c15ULL;x=(x^(x>>30))*0xbf58476d1ce4e5b9ULL;x=(x^(x>>27))*0x94d049bb133111ebULL;x^=x>>31;
 bool valid=(x%5)!=0; bool b=(x>>8)&1, file=(x>>9)&1, reg=(x>>10)&1; int g=int((x>>16)%101); if(i%4==0){b=file=reg=false;g=0;} if(i%10==0){valid=false;file=reg=false;b=true;g=90;}
 // Oracle is a signal contract, not ground truth about malware.
 bool evidence=file||reg||(valid&&(b||g>0)); auto r=f.update(g,b,file,reg,valid); auto a=decide(r); bool alert=a==Action::Review||a==Action::UrgentReview;
 if(evidence){if(alert)++tp;else ++fn;}else{if(alert)++fp;else ++tn;}
 if((!evidence&&a!=(valid?Action::Observe:Action::RestoreVisibility))||(evidence&&!alert)||(r.score>=60&&a!=Action::UrgentReview)||(r.score<60&&a==Action::UrgentReview))++wrong;
}
std::cout<<"Cases="<<n<<" TP="<<tp<<" TN="<<tn<<" FP="<<fp<<" FN="<<fn<<" policy_errors="<<wrong<<'\n';return fp||fn||wrong?1:0;}
