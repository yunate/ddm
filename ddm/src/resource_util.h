#ifndef resource_util_h_
#define resource_util_h_ 1

#include "g_def.h"
#include <windows.h>
BEG_NSP_DDM

class resource_util
{
    // �ͷ���Դ�ļ�
    // hModule �� ģ����������wWinMain( _In_ HINSTANCE hInstance,...) ��hInstance
    // target �� ����·��
    bool release_res(HMODULE hModule, DWORD resId, const ddstr& resTy, const ddstr& target);
};

END_NSP_DDM
#endif // resource_util_h_

