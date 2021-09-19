#include "base_native_window.h"
#include "win_class_register.h"
BEG_NSP_DDM

LRESULT base_native_window::win_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 获得实列对象，处理窗口事件
    base_native_window* pThis = (base_native_window*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (pThis != NULL) {
        if (pThis->win_proc_chain(hWnd, uMsg, wParam, lParam)) {
            return S_OK;
        }
    } else if (WM_CREATE == uMsg) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (DWORD_PTR)pCreate->lpCreateParams);

        // 将WM_CREATE消息抛给下一次消息循环
        return ::SendMessage(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

END_NSP_DDM
