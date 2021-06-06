
#ifndef simple_native_window_h_
#define simple_native_window_h_

#include "base_native_window.h"
BEG_NSP_DDM

class simple_native_window : public base_native_window
{
public:
    static ddstr get_class_name();

public:
    simple_native_window();
    virtual LRESULT CALLBACK win_proc_chain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    virtual bool create(const ddstr& winName, HINSTANCE hInst);

public:
    bool update_win_pos();
    bool update_win_pos(s32 x, s32 y, s32 w, s32 h);

public:
    inline HWND get_wnd() { return m_hWnd; }
    inline s32 get_x() { return m_x; }
    inline s32 get_y() { return m_y; }
    inline s32 get_w() { return m_w; }
    inline s32 get_h() { return m_h; }
    inline void set_x(s32 val) { m_x = val; }
    inline void set_y(s32 val) { m_y = val; }
    inline void set_w(s32 val) { m_w = val; }
    inline void set_h(s32 val) { m_h = val; }

protected:
    HINSTANCE m_inst = NULL;
    HWND m_hWnd = NULL;
    s32 m_x = 0;
    s32 m_y = 0;
    s32 m_w = 0;
    s32 m_h = 0;
};

END_NSP_DDM
#endif // simple_native_window_h_
#pragma once
