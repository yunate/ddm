#ifndef wnd_utils_h_
#define wnd_utils_h_

#include "g_def.h"

#include <windows.h>

BEG_NSP_DDM

class wnd_utils 
{
public:
    // 位置/大小
    static bool set_pos(HWND hwnd, s32 x, s32 y);
    static bool set_size(HWND hwnd, s32 w, s32 h);
    static void get_rect(HWND hwnd, RECT& rect);

    // 显示隐藏
    static void show(HWND hwnd, bool show);
    static bool is_show(HWND hwnd);

    // 最大化/最小化
    static void to_max(HWND hwnd);
    static bool is_max(HWND hwnd);
    static void to_min(HWND hwnd);
    static bool is_min(HWND hwnd);
    static void normal(HWND hwnd);
    static bool is_normal(HWND hwnd);

    // 标题
    static void set_title(HWND hwnd, const wchar_t* title);
    static void set_title(HWND hwnd, const char* title);
    static ddstr get_title(HWND hwnd);
};

inline bool wnd_utils::set_pos(HWND hwnd, s32 x, s32 y)
{
    return TRUE == ::SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

inline bool wnd_utils::set_size(HWND hwnd, s32 w, s32 h)
{
    return TRUE == ::SetWindowPos(hwnd, NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOMOVE);
}

inline void wnd_utils::get_rect(HWND hwnd, RECT& rect)
{
    (void)::GetWindowRect(hwnd, &rect);
}

inline void wnd_utils::show(HWND hwnd, bool show)
{
    (void)::ShowWindow(hwnd, show ? SW_SHOW : SW_HIDE);
}

inline bool wnd_utils::is_show(HWND hwnd)
{
    return TRUE == ::IsWindowVisible(hwnd);
}

inline void wnd_utils::to_max(HWND hwnd)
{
    (void)::ShowWindow(hwnd, SW_MAXIMIZE);
}

inline bool wnd_utils::is_max(HWND hwnd)
{
    return TRUE == ::IsZoomed(hwnd);
}

inline void wnd_utils::to_min(HWND hwnd)
{
    (void)::ShowWindow(hwnd, SW_MINIMIZE);
}

inline bool wnd_utils::is_min(HWND hwnd)
{
    return TRUE == ::IsIconic(hwnd);
}

inline void wnd_utils::normal(HWND hwnd)
{
    (void)::ShowWindow(hwnd, SW_NORMAL);
}

inline bool wnd_utils::is_normal(HWND hwnd)
{
    return !(is_max(hwnd) || is_min(hwnd));
}

inline void wnd_utils::set_title(HWND hwnd, const wchar_t* title)
{
    ::SetWindowText(hwnd, title);
}

inline void wnd_utils::set_title(HWND hwnd, const char* title)
{
    ::SetWindowTextA(hwnd, title);
}

inline ddstr wnd_utils::get_title(HWND hwnd)
{
    u32 len = ::GetWindowTextLength(hwnd);
    ddstr title(len + 1, 0);
    ::GetWindowText(hwnd, ((ddchar*)title.data()), len + 1);
    return title.c_str();
}

END_NSP_DDM
#endif
