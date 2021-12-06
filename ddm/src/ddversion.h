#ifndef ddversion_h_
#define ddversion_h_

#include "g_def.h"
#include "str/str_utils.h"

BEG_NSP_DDM

// v0.v1.v2.v3
struct ddversion
{
    u32 v[4];

    ddversion(u32 v0, u32 v1, u32 v2, u32 v3)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
    }

    u32& operator[](int i)
    {
        return v[i];
    }

    inline bool empty()
    {
        return (v[0] == 0) && (v[1] == 0) && (v[2] == 0) && (v[3] == 0);
    }


    std::string str()
    {
        return str_utils::str_format("%d.%d.%d.%d", v[0], v[1], v[2], v[3]);
    }

    std::wstring wstr()
    {
        return str_utils::str_format(L"%d.%d.%d.%d", v[0], v[1], v[2], v[3]);
    }
};

#define DDEMPTY_VERSION ddversion(0, 0, 0, 0)

inline bool operator<=(const ddversion& l, const ddversion& r)
{
    for (int i = 0; i < 4; ++i) {
        if (l.v[i] > r.v[i]) {
            return false;
        }
    }

    return true;
}

inline bool operator>=(const ddversion& l, const ddversion& r)
{
    for (int i = 0; i < 4; ++i) {
        if (l.v[i] < r.v[i]) {
            return false;
        }
    }

    return true;
}

inline bool operator<(const ddversion& l, const ddversion& r)
{
    return !(l >= r);
}

inline bool operator>(const ddversion& l, const ddversion& r)
{
    return !(l <= r);
}
END_NSP_DDM
#endif

