
#ifndef simple_native_window_h_
#define simple_native_window_h_

#include "base_native_window.h"
#include "ddkeyboard.h"
#include "ddmouse.h"
BEG_NSP_DDM

class simple_native_window : public base_native_window
{
public:
    static ddstr get_class_name();

public:
    simple_native_window();
    virtual bool win_proc_chain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    virtual bool create(const ddstr& winName, HINSTANCE hInst);

public:
    inline HWND get_wnd() { return m_hWnd; }
    ddkeyboard KB;
    ddmouse MOUSE;

protected:
    HWND m_hWnd = NULL;
};

END_NSP_DDM
#endif // simple_native_window_h_
#pragma once
