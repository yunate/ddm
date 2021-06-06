
#ifndef nocopyable_hpp_
#define nocopyable_hpp_ 1

#include "g_def.h"

BEG_NSP_DDM

// ½ûÖ¹¸´ÖÆ»ùÀà
class nocopyable
{
protected:
    nocopyable() {}
    ~nocopyable() {}
    nocopyable(const nocopyable&) = delete;
    nocopyable(nocopyable&&) = delete;
    nocopyable& operator= (const nocopyable&) = delete;
    nocopyable& operator= (nocopyable&&) = delete;
};

END_NSP_DDM
#endif // nocopyable_hpp_
