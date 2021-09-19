
#ifndef ddmouse_h_
#define ddmouse_h_

#include "g_def.h"
#include "nocopyable.hpp"

#include <functional>
#include <list>
#include <queue>
#include <windows.h>
BEG_NSP_DDM
struct MOUSE_EVENTD
{
    enum class TYPE
    {
        down,
        up,
        dbclick,
        wheel,
        move,
        enter,
        leave,
    };
    TYPE type;
    u8 code = 0;
    u32 x = 0;
    u32 y = 0;
    u32 wheel_del = 0;
    bool is_ldown = false;
    bool is_rdown = false;
    bool is_mdown = false;
};

using MOUSE_EVENT_CB = std::function<bool(const MOUSE_EVENTD&)>;

class ddmouse : private nocopyable
{
public:
    static bool is_key_down(int key);
    static bool is_ldown();
    static bool is_rdown();
    static bool is_mdown();

    bool on_msg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    std::list<MOUSE_EVENTD> mouse_events; // 不会记录移动事件
    u32 mouse_events_max_size = 16;

public:
    MOUSE_EVENT_CB ON_LDOWN;
    MOUSE_EVENT_CB ON_LUP;
    MOUSE_EVENT_CB ON_LDBCLICK;

    MOUSE_EVENT_CB ON_RDOWN;
    MOUSE_EVENT_CB ON_RUP;
    MOUSE_EVENT_CB ON_RDBCLICK;

    MOUSE_EVENT_CB ON_MDOWN;
    MOUSE_EVENT_CB ON_MUP;
    MOUSE_EVENT_CB ON_MDBCLICK;
    MOUSE_EVENT_CB ON_MID_WHELL;

    MOUSE_EVENT_CB ON_MOVE;
    MOUSE_EVENT_CB ON_ENTER;
    MOUSE_EVENT_CB ON_LEAVE;

private:
    bool is_in_wnd = false;
    void trim_mouse_events();
    void push_mouse_event(const MOUSE_EVENTD& kb_event);
    bool call_cbs(const MOUSE_EVENT_CB& cb, const MOUSE_EVENTD& ev);

private:
    void on_leave(WPARAM wParam, POINTS pt);
    void try_leave(HWND hwnd, WPARAM wParam, POINTS pt);
};

END_NSP_DDM
#endif