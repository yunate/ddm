
#define _CRT_RAND_S
#include "random.h"
#ifdef DD_WINDOW
#include <stdlib.h>
#include <Guiddef.h>
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

bool random::gen_guida(ddstra& guid)
{
    ::GUID g;
    if (S_OK != ::CoCreateGuid(&g)) {
        return false;
    }

    guid.resize(64);
    ::sprintf_s(&(guid[0]), guid.length(),
                "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                g.Data1, g.Data2,
                g.Data3, g.Data4[0],
                g.Data4[1], g.Data4[2],
                g.Data4[3], g.Data4[4],
                g.Data4[5], g.Data4[6],
                g.Data4[7]);
    guid = guid.c_str();
    return true;
}

bool random::gen_guidw(ddstrw& guid)
{
    ::GUID g;
    if (S_OK != ::CoCreateGuid(&g)) {
        return false;
    }

    guid.resize(64);
    ::swprintf_s(&(guid[0]), guid.length(),
                L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                g.Data1, g.Data2,
                g.Data3, g.Data4[0],
                g.Data4[1], g.Data4[2],
                g.Data4[3], g.Data4[4],
                g.Data4[5], g.Data4[6],
                g.Data4[7]);
    guid = guid.c_str();
    return true;
}

END_NSP_DDM
#endif // DD_WINDOW
