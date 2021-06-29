#ifndef ddmacro_hpp
#define ddmacro_hpp

// 逗号
#define DD_COMMA() ,

// num
#define DD_0 0
#define DD_1 1
#define DD_2 2
#define DD_3 3
#define DD_4 4
#define DD_5 5
#define DD_6 6
#define DD_7 7
#define DD_8 8
#define DD_9 9
#define DD_10 10
#define DD_11 11
#define DD_12 12
#define DD_13 13
#define DD_14 14
#define DD_15 15
#define DD_16 16
#define DD_17 17
#define DD_18 18
#define DD_19 19
#define DD_20 20

// ++ 
#define DD_INC_0  1
#define DD_INC_1  2
#define DD_INC_2  3
#define DD_INC_3  4
#define DD_INC_4  5
#define DD_INC_5  6
#define DD_INC_6  7
#define DD_INC_7  8
#define DD_INC_8  9
#define DD_INC_9  10
#define DD_INC_10 11
#define DD_INC_11 12
#define DD_INC_12 13
#define DD_INC_13 14
#define DD_INC_14 15
#define DD_INC_15 16
#define DD_INC_16 17
#define DD_INC_17 18
#define DD_INC_18 19
#define DD_INC_19 20
#define DD_INC_20 0

// --
#define DD_DEC_0  20
#define DD_DEC_1  0
#define DD_DEC_2  1
#define DD_DEC_3  2
#define DD_DEC_4  3
#define DD_DEC_5  4
#define DD_DEC_6  5
#define DD_DEC_7  6
#define DD_DEC_8  7
#define DD_DEC_9  8
#define DD_DEC_10 9
#define DD_DEC_11 10
#define DD_DEC_12 11
#define DD_DEC_13 12
#define DD_DEC_14 13
#define DD_DEC_15 14
#define DD_DEC_16 15
#define DD_DEC_17 16
#define DD_DEC_18 17
#define DD_DEC_19 18
#define DD_DEC_20 19

#define DD_INC(N) DD_CONCAT(DD_, N)

// expend
#define DD_EXPEND(...) __VA_ARGS__

// concat
#define _DD_CONCAT(A, B) A##B
#define DD_CONCAT(A, B) DD_EXPEND(_DD_CONCAT(A, B))

// or
#define _DD_OR_00 0
#define _DD_OR_01 1
#define _DD_OR_10 1
#define _DD_OR_11 1
#define DD_OR(A, B) DD_CONCAT(_DD_OR_, DD_CONCAT(A, B))

// not
#define _DD_NOT_0 1
#define _DD_NOT_1 0
#define DD_NOT(N) DD_CONCAT(_DD_NOT_, N)

// if
#define _DD_IF_1(THEN, ELSE) THEN
#define _DD_IF_0(THEN, ELSE) ELSE
#define DD_IF(B, THEN, ELSE) DD_CONCAT(_DD_IF_, B)(THEN, ELSE)

// get n
#define _DD_GET_0(_0, ...) _0
#define _DD_GET_1(_0, _1, ...) _1
#define _DD_GET_2(_0, _1, _2, ...) _2
#define _DD_GET_3(_0, _1, _2,  _3, ...) _3
#define _DD_GET_4(_0, _1, _2,  _3, _4, ...) _4
#define _DD_GET_5(_0, _1, _2,  _3, _4, _5, ...) _5
#define _DD_GET_6(_0, _1, _2,  _3, _4, _5, _6, ...) _6
#define _DD_GET_7(_0, _1, _2,  _3, _4, _5, _6, _7, ...) _7
#define _DD_GET_8(_0, _1, _2,  _3, _4, _5, _6, _7, _8, ...) _8
#define _DD_GET_9(_0, _1, _2,  _3, _4, _5, _6, _7, _8, _9, ...) _9
#define _DD_GET_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define _DD_GET_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define _DD_GET_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define _DD_GET_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define _DD_GET_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define _DD_GET_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define _DD_GET_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define _DD_GET_17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) _17
#define _DD_GET_18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) _18
#define _DD_GET_19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) _19
#define _DD_GET_20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, ...) _20
#define DD_GET_N(N, ...) DD_EXPEND(DD_CONCAT(_DD_GET_, N)(__VA_ARGS__))

// has COMMA 是否有逗号
#define DD_HAS_COMMA(...) DD_GET_N(20, __VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0)

// 是否有且仅有一个参数
#define _DD_ONLY_ONE(...) ,
#define DD_ONLY_ONE(...) DD_EXPEND(DD_NOT(DD_HAS_COMMA(_DD_ONLY_ONE __VA_ARGS__())))

// 是否为空
#define DD_IS_EMPTY(...) DD_NOT(DD_OR(DD_HAS_COMMA(__VA_ARGS__), DD_ONLY_ONE(__VA_ARGS__)))

// C20 __VA_OPT__(,)
#define  DD_OPT(OPT, ...) DD_IF(DD_IS_EMPTY(__VA_ARGS__),,OPT)

// 参数个数
#define DD_ARG_CNT(...)  DD_GET_N(20, __VA_ARGS__ DD_OPT(DD_COMMA(), __VA_ARGS__) 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// for each example:
// #define OPT(a, idx) int a;
// DD_EACH(OPT, a, b, c)
#define _DD_EACH_0(opt,  a)
#define _DD_EACH_1(opt,  idx, a)      opt(a, idx)
#define _DD_EACH_2(opt,  idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_1(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_3(opt,  idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_2(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_4(opt,  idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_3(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_5(opt,  idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_4(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_6(opt,  idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_5(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_7(opt,  idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_6(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_8(opt,  idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_7(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_9(opt,  idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_8(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_10(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_9(opt, DD_CONCAT(DD_INC_, idx), __VA_ARGS__))
#define _DD_EACH_11(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_10(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_12(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_11(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_13(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_12(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_14(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_13(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_15(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_14(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_16(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_15(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_17(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_16(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_18(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_17(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_19(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_18(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define _DD_EACH_20(opt, idx, a, ...) opt(a, idx) DD_EXPEND(_DD_EACH_19(opt, DD_CONCAT(DD_INC_, idx),__VA_ARGS__))
#define  DD_EACH(opt, ...) DD_EXPEND(DD_CONCAT(_DD_EACH_, DD_ARG_CNT(__VA_ARGS__))(opt, 0,  __VA_ARGS__))
#endif
