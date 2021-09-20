#include "simple_native_window.h"

BEG_NSP_DDM
ddstr simple_native_window::get_class_name()
{
    return _DDT("ddm_simple_native_window_class");
}

simple_native_window::simple_native_window()
{
}

bool simple_native_window::win_proc_chain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (KB.on_msg(hWnd, uMsg, wParam, lParam)) {
        return true;
    }
    if (MOUSE.on_msg(hWnd, uMsg, wParam, lParam)) {
        return true;
    }
    return false;
}

bool simple_native_window::create(const ddstr& winName, HINSTANCE hInst)
{
    m_hWnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                              get_class_name().c_str(), winName.c_str(),
                              WS_OVERLAPPEDWINDOW,
                              0, 0, 0, 0,
                              0, 0, hInst, this);// 传给WM_CREATE事件响应
    if (m_hWnd == NULL) {
        return false;
    }
    return true;
}
END_NSP_DDM
