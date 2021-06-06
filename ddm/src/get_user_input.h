
#ifndef get_user_input_h_
#define get_user_input_h_ 1

#include "g_def.h"
#include "color_print.h"

BEG_NSP_DDM
/** �û�����һ�У��س�����
@param [out] line �û������һ��
*/
inline void get_user_input_linew(ddstrw& line)
{
    color_printf(Gray, "input>>");
    line = L"";

    while (true) {
        ddcharw wc = ::getwchar();

        if (wc == L'\n') {
            break;
        } else {
            line.append(1, wc);
        }
    }
}

/** �û�����һ�У��س�����
@param [out] line �û������һ��
*/
inline void get_user_input_linea(ddstra& line)
{
    color_printf(Gray, "input>>");
    line = "";

    while (true) {
        ddchara c = ::getchar();

        if (c == '\n')
        {
            break;
        } else {
            line.append(1, c);
        }
    }
}

#ifdef _UNICODE
#define get_user_input_line get_user_input_linew
#else
#define get_user_input_line get_user_input_linea
#endif // 

END_NSP_DDM
#endif // get_user_input_h_
