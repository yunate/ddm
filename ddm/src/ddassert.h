#ifndef ddassert_h_
#define ddassert_h_
#include "g_def.h"

#ifdef DD_WINDOW
BEG_NSP_DDM
#undef DD_ASSERT
#ifdef _DEBUG
#include <assert.h>
#define DD_ASSERT(e) assert(e)
#include <crtdbg.h>
#define DD_ASSERT_FMTW(expr, format, ...) \
    (void) ((!!(expr)) || \
    (1 != ::_CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, format, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))

#define DD_ASSERT_FMTA(expr, format, ...) \
    (void) ((!!(expr)) || \
    (1 != ::_CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, format, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))
#else
#define DD_ASSERT(x) (x);
#define DD_ASSERT_FMTW(expr, format, ...) (expr);
#define DD_ASSERT_FMTA(expr, format, ...) (expr);
#endif

#ifdef _UNICODE 
#define DD_ASSERT_FMT    DD_ASSERT_FMTW
#else
#define DD_ASSERT_FMT    DD_ASSERT_FMTA
#endif

END_NSP_DDM
#endif
#endif
