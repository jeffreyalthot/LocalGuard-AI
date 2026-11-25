#include "ServiceRecovery.h"
#include <cstdlib>
int main(){
 if(serviceRecoverySeconds[0]!=60 || serviceRecoverySeconds[1]!=300 || serviceRecoverySeconds[2]!=0 || serviceRecoveryResetSeconds!=86400) return 1;
 for(int rc=-10;rc<=10;++rc) for(bool stop:{false,true})
 if((serviceExitCode(stop,rc)==0)!=(stop&&rc==0)) return 2;
 return 0;
}
