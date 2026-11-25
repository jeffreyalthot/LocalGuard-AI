#include "Fusion.h"
int main(){Fusion f; if(f.update(0,false,false,false,true).review)return 1; if(!f.update(25,false,false,false,true).review)return 2; if(!f.update(0,false,true,false,false).review)return 3; if(!f.update(0,false,false,false,false).degraded)return 4; if(f.update(100,true,true,true,true).score!=100)return 5; if(f.update(0,false,false,false,true).score!=0)return 6;return 0;}
