#include "simple_native_window.h"

BEG_NSP_DDM
ddstr simple_native_window::get_class_name()
{
    return _DDT("ddm_simple_native_window_class");
}

simple_native_window::simple_native_window()
{
}

LRESULT CALLBACK simple_native_window::win_proc_chain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return S_FALSE;
}

bool simple_native_window::create(const ddstr& winName, HINSTANCE hInst)
{
    m_inst = hInst;
    m_hWnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                              get_class_name().c_str(), winName.c_str(),
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                              0, 0, m_inst, this);// 传给WM_CREATE事件响应
    if (m_hWnd == NULL) {
        return false;
    }

    set_x(CW_USEDEFAULT);
    set_y(CW_USEDEFAULT);
    set_w(CW_USEDEFAULT);
    set_h(CW_USEDEFAULT);
    return true;
}

bool simple_native_window::update_win_pos(s32 x, s32 y, s32 w, s32 h)
{
    set_x(x);
    set_y(y);
    set_w(w);
    set_h(h);
    return update_win_pos();
}

bool simple_native_window::update_win_pos()
{
    return TRUE == ::SetWindowPos(m_hWnd, HWND_TOP, m_x, m_y, m_w, m_h, SWP_SHOWWINDOW);
}
END_NSP_DDM
