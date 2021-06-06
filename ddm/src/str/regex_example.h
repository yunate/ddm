
#ifndef regex_example_h_
#define regex_example_h_ 1

//////////////////////////////////////////////////////////////////////////
//                         一些常用的正则表达式                             //
//                         仅供参考！！！                                  //
//////////////////////////////////////////////////////////////////////////

#include "g_def.h"
BEG_NSP_DDM

/** 数字：
*/
static const ddchar* s_regex_number = _DDT(R"([0-9]*)");

/** n 位的数字
*/
static const ddchar* s_regex_n_number = _DDT(R"(\d{n})");

/** 至少 n 位的数字
*/
static const ddchar* s_regex_np_number = _DDT(R"(\d{n,})");

/** m-n 位的数字：
*/
static const ddchar* s_regex_nm_number = _DDT(R"(\d{n,m})");

/** 零开头的数字
*/
static const ddchar* s_regex_0_number = _DDT(R"((0[0-9]*))");

/** 非零开头的数字
*/
static const ddchar* s_regex_n0_number = _DDT(R"(([1-9][0-9]*))");

/** 非零开头的最多带m位小数的数字
*/
static const ddchar* s_regex_n0_mdcml_number = _DDT(R"(([1-9][0-9]*)+(\.[0-9]{1,m})?)");

/** 最多带m位小数的负数
*/
static const ddchar* s_regex_nativ_mdcml_number = _DDT(R"((\-)?\d+(\.\d{1,m})?)");

/** 有理数
*/
static const ddchar* s_regex_q_number = _DDT(R"((\-|\+)?\d+(\.\d+)?)");

/** 汉字
*/
static const ddchar* s_regex_chinese = _DDT(R"([\u4e00-\u9fa5]+)");

/** 英文和数字
*/
static const ddchar* s_regex_comm_char = _DDT(R"([A-Za-z0-9]+)");

/** Email 地址：?
*/
static const ddchar* s_regex_email = _DDT(R"([a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+)");

/** 域名：?
*/
static const ddchar* s_regex_domain_name = _DDT(R"(\w+(\.\w)+)";

END_NSP_DDM
#endif // regex_example_h_
