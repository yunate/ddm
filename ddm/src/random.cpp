
#define _CRT_RAND_S
#include "random.h"
#ifdef DD_WINDOW
#include "windows/ddwin_utils.h"
#include <stdlib.h>
#include <ObjBase.h>

BEG_NSP_DDM
bool random::get_rand_number(int min, int max, int& out)
{
    if (max < min) {
        return false;
    }

    unsigned int tmp = 0;

    if (::rand_s(&tmp) != 0) {
        return false;
    }

    out = tmp % (max - min + 1) + min;
    return true;
}

bool random::gen_guid(ddstra& guid)
{
    ::GUID g;
    if (S_OK != ::CoCreateGuid(&g)) {
        return false;
    }

    return ddguid_str(g, guid);
}

bool random::gen_guid(ddstrw& guid)
{
    ::GUID g;
    if (S_OK != ::CoCreateGuid(&g)) {
        return false;
    }

    return ddguid_str(g, guid);
}

END_NSP_DDM
#endif // DD_WINDOW
