
#ifndef base_native_window_h_
#define base_native_window_h_

#include "g_def.h"
#include "nocopyable.hpp"
#include "wnd_utils.hpp"
#include <windows.h>
BEG_NSP_DDM

class base_native_window
{
    DD_NO_COPY_MOVE(base_native_window);
public:
    base_native_window() = default;
    ~base_native_window() = default;
    static LRESULT CALLBACK win_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual bool win_proc_chain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
END_NSP_DDM
#endif // base_native_window_h_
