
#ifndef win_class_register_h_
#define win_class_register_h_

#include "base/g_def.h"
#include "base/nocopyable.hpp"
#include <windows.h>
BEG_NSP_DDM

class win_class_register : public nocopyable
{
public:
    // 如果想要新的class style 自己依次添加即可
    static bool regist_window_class1(HINSTANCE hInst, const ddstr& className);
    static bool regist_window_class2(HINSTANCE hInst, const ddstr& className);

    static void regist_all(HINSTANCE hInst);
};

END_NSP_DDM
#endif // win_class_register_h_
