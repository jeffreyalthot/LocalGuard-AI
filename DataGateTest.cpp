#include "DataDefense.h"
int main(){DataDefense a,b;for(int i=0;i<30;++i){a.update({0,0,true},false);b.update({0,0,true},true);}if(a.update({0,8,true},false).review)return 1;if(!b.update({0,8,true},false).review)return 2;return 0;}
