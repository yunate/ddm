
#ifndef nocopyable_hpp_
#define nocopyable_hpp_ 1

#include "g_def.h"

BEG_NSP_DDM

#define DD_NO_COPY(className)                                   \
    className(const className&) = delete;                       \
    className& operator= (const className&) = delete;           \

#define DD_NO_MOVE(className)                                   \
    className(className&&) = delete;                            \
    className& operator= (className&&) = delete;                \

#define DD_NO_COPY_MOVE(className)                              \
    DD_NO_COPY(className);                                      \
    DD_NO_MOVE(className);                                      \

END_NSP_DDM
#endif // nocopyable_hpp_
