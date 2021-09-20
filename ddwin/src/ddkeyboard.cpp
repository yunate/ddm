#include "ddkeyboard.h"
BEG_NSP_DDM
bool ddkeyboard::is_key_down(int key)
{
    // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    return (::GetAsyncKeyState(key) & 0x8000);
}

bool ddkeyboard::is_ctrl_down()
{
    return is_key_down(VK_CONTROL);
}
bool ddkeyboard::is_shift_down()
{
    return is_key_down(VK_SHIFT);
}
bool ddkeyboard::is_alt_down()
{
    return is_key_down(VK_MENU);
}

bool ddkeyboard::is_caplock()
{
    return (::GetKeyState(VK_CAPITAL) & 1);
}

//////////////////////////////////////////////////////////////////////////
bool ddkeyboard::on_msg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    bool result = false;
    switch (msg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: // sys key commands need to be handled to track ALT key (VK_MENU) and F10
        if (!(lParam & 0x40000000) || key_down_repeat) {
            result = call_cbs(ON_KEY_DOWN, static_cast<u8>(wParam));
            push_kb_event(KB_EVENT{ true, static_cast<u8>(wParam) });
            trim_kb_events();
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        result = call_cbs(ON_KEY_UP, static_cast<u8>(wParam));
        push_kb_event(KB_EVENT{ false, static_cast<u8>(wParam) });
        trim_kb_events();
        break;
    case WM_CHAR:
        result = call_cbs(ON_CHAR, static_cast<u8>(wParam));
        break;
    }
    return result;
}

void ddkeyboard::trim_kb_events()
{
    if (kb_events.size() > kb_events_max_size) {
        kb_events.pop_front();
    }
}
void ddkeyboard::push_kb_event(const KB_EVENT& kb_event)
{
    kb_events.push_back({ kb_event.is_down, kb_event.code, is_shift_down(), is_alt_down(), is_ctrl_down() });
    trim_kb_events();
}

bool ddkeyboard::call_cbs(const KB_EVENT_CB& cb, u8 code)
{
    if (cb == nullptr) {
        return false;
    }
    return cb(code);
}
END_NSP_DDM

