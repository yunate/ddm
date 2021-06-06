
#ifndef regex_example_h_
#define regex_example_h_ 1

//////////////////////////////////////////////////////////////////////////
//                         һЩ���õ�������ʽ                             //
//                         �����ο�������                                  //
//////////////////////////////////////////////////////////////////////////

#include "g_def.h"
BEG_NSP_DDM

/** ���֣�
*/
static const ddchar* s_regex_number = _DDT(R"([0-9]*)");

/** n λ������
*/
static const ddchar* s_regex_n_number = _DDT(R"(\d{n})");

/** ���� n λ������
*/
static const ddchar* s_regex_np_number = _DDT(R"(\d{n,})");

/** m-n λ�����֣�
*/
static const ddchar* s_regex_nm_number = _DDT(R"(\d{n,m})");

/** �㿪ͷ������
*/
static const ddchar* s_regex_0_number = _DDT(R"((0[0-9]*))");

/** ���㿪ͷ������
*/
static const ddchar* s_regex_n0_number = _DDT(R"(([1-9][0-9]*))");

/** ���㿪ͷ������mλС��������
*/
static const ddchar* s_regex_n0_mdcml_number = _DDT(R"(([1-9][0-9]*)+(\.[0-9]{1,m})?)");

/** ����mλС���ĸ���
*/
static const ddchar* s_regex_nativ_mdcml_number = _DDT(R"((\-)?\d+(\.\d{1,m})?)");

/** ������
*/
static const ddchar* s_regex_q_number = _DDT(R"((\-|\+)?\d+(\.\d+)?)");

/** ����
*/
static const ddchar* s_regex_chinese = _DDT(R"([\u4e00-\u9fa5]+)");

/** Ӣ�ĺ�����
*/
static const ddchar* s_regex_comm_char = _DDT(R"([A-Za-z0-9]+)");

/** Email ��ַ��?
*/
static const ddchar* s_regex_email = _DDT(R"([a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+)");

/** ������?
*/
static const ddchar* s_regex_domain_name = _DDT(R"(\w+(\.\w)+)";

END_NSP_DDM
#endif // regex_example_h_
