
#ifndef ddkeyboard_h_
#define ddkeyboard_h_

#include "g_def.h"
#include "nocopyable.hpp"

#include <functional>
#include <list>
#include <queue>
#include <windows.h>
BEG_NSP_DDM
struct KB_EVENT
{
    bool is_down = true;
    u8 code = 0;
    bool is_shift_down = false;
    bool is_ctrl_down = false;
    bool is_alt_down = false;
};

using KB_EVENT_CB = std::function<bool(u8)>;

class ddkeyboard
{
    DD_NO_COPY_MOVE(ddkeyboard);
public:
    static bool is_key_down(int key);
    static bool is_ctrl_down();
    static bool is_shift_down();
    static bool is_alt_down();
    static bool is_caplock();

    bool on_msg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    bool key_down_repeat = false;
    std::list<KB_EVENT> kb_events;
    u32 kb_events_max_size = 16;

public:
    KB_EVENT_CB ON_KEY_DOWN;
    KB_EVENT_CB ON_KEY_UP;
    KB_EVENT_CB ON_CHAR;

private:
    void trim_kb_events();
    void push_kb_event(const KB_EVENT& kb_event);
    bool call_cbs(const KB_EVENT_CB& cb, u8 code);
};

END_NSP_DDM
#endif