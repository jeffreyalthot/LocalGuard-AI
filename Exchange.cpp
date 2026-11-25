#include "Exchange.h"
#include <fstream>
std::string exchange(int risk,bool valid){if(!valid||risk<75)return "Exchange: no report";std::ofstream f("incident-summary.txt",std::ios::trunc);f<<"LocalGuard summary v1\nrisk="<<risk<<"\n";return f?"Exchange: local summary written; manual review only":"Exchange: write failed";}
