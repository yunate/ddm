#ifndef ddsecurity_h_
#define ddsecurity_h_

#include "g_def.h"
#include <windows.h>
#pragma comment(lib, "Advapi32.lib")

BEG_NSP_DDM

DWORD create_low_sd(SECURITY_ATTRIBUTES& secAttr);

END_NSP_DDM
#endif // 
