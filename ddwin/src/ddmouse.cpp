#include "ddmouse.h"
#include "wnd_utils.hpp"
BEG_NSP_DDM

#define MAKE_BOOL(val) ((val) != 0)

bool ddmouse::is_key_down(int key)
{
    // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    return (::GetAsyncKeyState(key) & 0x8000);
}

bool ddmouse::is_ldown()
{
    return is_key_down(VK_LBUTTON);
}

bool ddmouse::is_rdown()
{
    return is_key_down(VK_RBUTTON);
}

bool ddmouse::is_mdown()
{
    return is_key_down(VK_MBUTTON);
}

bool ddmouse::on_msg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    bool result = false;
    const POINTS pt = MAKEPOINTS(lParam);
    switch (msg)
    {
    case WM_LBUTTONDOWN:
        {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::down, VK_LBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_LDOWN, ev);
            break;
        }
    case WM_LBUTTONUP:
        {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::up, VK_LBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_LUP, ev);
            try_leave(hWnd, wParam, pt);
            break;
        }
    case WM_LBUTTONDBLCLK:
        {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::dbclick, VK_LBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_LDBCLICK, ev);
            break;
        }

    case WM_RBUTTONUP:
        {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::up, VK_RBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_RUP, ev);
            try_leave(hWnd, wParam, pt);
            break;
        }
    case WM_RBUTTONDOWN:
        {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::down, VK_RBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_RDOWN, ev);
            break;
        }
    case WM_RBUTTONDBLCLK:
        {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev{
                MOUSE_EVENTD::TYPE::dbclick, VK_RBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_RDBCLICK, ev);
            break;
        }

    case WM_MBUTTONDOWN: {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::down, VK_MBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_MDOWN, ev);
            break;
        }
    case WM_MBUTTONUP: {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::up, VK_MBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_MUP, ev);
            try_leave(hWnd, wParam, pt);
            break;
        }
    case WM_MBUTTONDBLCLK:
        {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::dbclick, VK_MBUTTON,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_MDBCLICK, ev);
            break;
        }
    case WM_MOUSEWHEEL:
        {
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::wheel, VK_MBUTTON,
                (u32)pt.x, (u32)pt.y,
                (u32)delta,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            push_mouse_event(ev);
            result = call_cbs(ON_MID_WHELL, ev);
            break;
        }

    case WM_MOUSEMOVE:
        {
            WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            MOUSE_EVENTD ev {
                MOUSE_EVENTD::TYPE::move, 0,
                (u32)pt.x, (u32)pt.y,
                0,
                MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
            };
            // push_mouse_event(ev);
            result = call_cbs(ON_MOVE, ev);

            RECT rect;
            wnd_utils::get_client_rect(hWnd, rect);
            s32 width = rect.right - rect.left;
            s32 height = rect.bottom - rect.top;

            if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
                if (!is_in_wnd) {
                    ::SetCapture(hWnd);
                    is_in_wnd = true;
                    WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
                    MOUSE_EVENTD ev {
                        MOUSE_EVENTD::TYPE::enter, 0,
                        (u32)pt.x, (u32)pt.y,
                        0,
                        MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
                    };
                    push_mouse_event(ev);
                    call_cbs(ON_ENTER, ev);
                }
            } else {
                if (is_in_wnd && !((MAKE_BOOL(MK_LBUTTON & fwKeys)) || (MAKE_BOOL(MK_RBUTTON & fwKeys)) || (MAKE_BOOL(MK_MBUTTON & fwKeys)))) {
                    on_leave(wParam, pt);
                }
            }
            break;
        }
    }
    return result;
}

void ddmouse::trim_mouse_events()
{
    if (mouse_events.size() > mouse_events_max_size) {
        mouse_events.pop_front();
    }
}

void ddmouse::push_mouse_event(const MOUSE_EVENTD& kb_event)
{
    mouse_events.push_back(kb_event);
    trim_mouse_events();
}

bool ddmouse::call_cbs(const MOUSE_EVENT_CB& cb, const MOUSE_EVENTD& ev)
{
    if (cb == nullptr) {
        return false;
    }
    return cb(ev);
}

void ddmouse::on_leave(WPARAM wParam, POINTS pt)
{
    ::ReleaseCapture();
    is_in_wnd = false;
    WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
    MOUSE_EVENTD ev{
        MOUSE_EVENTD::TYPE::leave, 0,
        (u32)pt.x, (u32)pt.y,
        0,
        MAKE_BOOL(MK_LBUTTON & fwKeys), MAKE_BOOL(MK_RBUTTON & fwKeys), MAKE_BOOL(MK_MBUTTON & fwKeys)
    };
    push_mouse_event(ev);
    call_cbs(ON_LEAVE, ev);
}

void ddmouse::try_leave(HWND hwnd, WPARAM wParam, POINTS pt)
{
    RECT rect;
    wnd_utils::get_client_rect(hwnd, rect);
    s32 width = rect.right - rect.left;
    s32 height = rect.bottom - rect.top;
    if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height) {
        on_leave(wParam, pt);
    }
}

END_NSP_DDM
