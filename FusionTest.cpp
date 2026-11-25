#include "Fusion.h"
int main(){Fusion f;if(f.update(0,false,false,false,true).score!=0)return 1;if(f.update(25,true,false,false,true).score!=50)return 2;if(!f.update(25,true,false,false,true).review)return 3;if(f.update(100,true,true,true,true).score!=100)return 4;if(!f.update(100,true,true,true,false).degraded)return 5;return 0;}
