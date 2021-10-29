
#ifndef win_class_register_h_
#define win_class_register_h_

#include "g_def.h"
#include "nocopyable.hpp"
#include <windows.h>
BEG_NSP_DDM

class win_class_register
{
    DD_NO_COPY_MOVE(win_class_register);
public:
    win_class_register()
    {
        regist_all(::GetModuleHandle(NULL));
    }
    // 如果想要新的class style 自己依次添加即可
    static bool regist_window_class1(HINSTANCE hInst, const ddstr& className);
    static bool regist_window_class2(HINSTANCE hInst, const ddstr& className);
    static void regist_all(HINSTANCE hInst);
};

static win_class_register g_win_class_register;
END_NSP_DDM
#endif // win_class_register_h_
