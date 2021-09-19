
#include "test_case_factory.h"
#include "simple_native_window.h"
#include "win_class_register.h"
#include "thread/loop/win_msg_loop.h"
#include "ddkeyboard.h"
#include "ddmouse.h"
#include "str/str_utils.h"

BEG_NSP_DDM
TEST(test_ddwin1, simple_native_window)
{
    HINSTANCE hinst = ::GetModuleHandle(NULL);
    win_class_register::regist_all(hinst);
    simple_native_window nativeWin;
    nativeWin.create(_DDT("simple_native_window"), hinst);
    wnd_utils::set_pos(nativeWin.get_wnd(), 500, 200);
    wnd_utils::set_size(nativeWin.get_wnd(), 500, 300);
    win_msg_loop loop;
    loop.loop();
}

class simple_native_window1 : public simple_native_window
{
public:
    virtual bool win_proc_chain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        bool handled = false;
        switch (uMsg)
        {
        case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = ::BeginPaint(m_hWnd, &ps);
                for (int i = 100; i < 500; i++)
                {
                    if (-1 == SetPixel(hdc, i, i, RGB(0, 255, 0)))
                    {
                    }
                }
                ::EndPaint(m_hWnd, &ps);
                break;
            }
        case WM_LBUTTONDOWN: {
                HDC hdc;
                hdc = ::GetDC(m_hWnd);
                MoveToEx(hdc, 100, 300, NULL);
                LineTo(hdc, 200, 400);
                ::ReleaseDC(m_hWnd, hdc);//ÊÍ·Åhdc
                break;
            }
        }

        if (handled) {
            return true;
        }
        return simple_native_window::win_proc_chain(hWnd, uMsg, wParam, lParam);
    }

    void loop()
    {
        m_loop.loop();
    }
private:
    win_msg_loop m_loop;
};

TEST(test_ddwin, simple_native_window1)
{
    bool altDown = ddkeyboard::is_alt_down();
    bool ctrlDown = ddkeyboard::is_ctrl_down();
    bool shiftDown = ddkeyboard::is_shift_down();
    altDown; ctrlDown; shiftDown;
    HINSTANCE hinst = ::GetModuleHandle(NULL);
    win_class_register::regist_all(hinst);
    simple_native_window1 nativeWin;
    (void)nativeWin.create(_DDT("simple_native_window"), hinst);
    wnd_utils::set_pos(nativeWin.get_wnd(), 500, 200);
    wnd_utils::set_size(nativeWin.get_wnd(), 500, 300);

    static std::wstring title;
    nativeWin.KB.ON_KEY_DOWN.push_back([&nativeWin](u8 code) {
        title.push_back(code);
        wnd_utils::set_title(nativeWin.get_wnd(), title.c_str());
        return true;
    });

    nativeWin.KB.ON_KEY_UP.push_back([&nativeWin](u8 code) {
        if (code == 'F') {
            title = L"";
            wnd_utils::set_title(nativeWin.get_wnd(), L"Reset");
        }
        return true;
    });

    nativeWin.MOUSE.ON_LDOWN.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_LBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::down) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_LDOWN:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_LDOWN error");
            DD_ASSERT(false);
        }

        return true;
    });
    nativeWin.MOUSE.ON_LUP.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_LBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::up) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_LUP:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_LUP error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.MOUSE.ON_LDBCLICK.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_LBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::dbclick) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_LDBCLICK:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_LDBCLICK error");
            DD_ASSERT(false);
        }
        return true;
    });

    nativeWin.MOUSE.ON_RDOWN.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_RBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::down) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_RDOWN:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_RDOWN error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.MOUSE.ON_RUP.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_RBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::up) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_RUP:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_RUP error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.MOUSE.ON_RDBCLICK.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_RBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::dbclick) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_RDBCLICK:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_RDBCLICK error");
            DD_ASSERT(false);
        }
        return true;
    });

    nativeWin.MOUSE.ON_MDOWN.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_MBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::down) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_MDOWN:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_MDOWN error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.MOUSE.ON_MUP.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_MBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::up) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_MUP:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_MUP error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.MOUSE.ON_MDBCLICK.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_MBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::dbclick) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_MDBCLICK:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_MDBCLICK error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.MOUSE.ON_MID_WHELL.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == VK_MBUTTON &&
            ev.type == MOUSE_EVENTD::TYPE::wheel) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_MID_WHELL:pt:%d,%d, del:%d", ev.x, ev.y, ev.wheel_del).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_MID_WHELL error");
            DD_ASSERT(false);
        }
        return true;
    });

    nativeWin.MOUSE.ON_MOVE.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == 0 &&
            ev.type == MOUSE_EVENTD::TYPE::move) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_MOVE:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_MOVE error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.MOUSE.ON_ENTER.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == 0 &&
            ev.type == MOUSE_EVENTD::TYPE::enter) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_ENTER:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_ENTER error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.MOUSE.ON_LEAVE.push_back([&nativeWin](const MOUSE_EVENTD& ev) {
        if (ev.code == 0 &&
            ev.type == MOUSE_EVENTD::TYPE::leave) {
            wnd_utils::set_title(nativeWin.get_wnd(), str_utils::str_format(L"ON_LEAVE:pt:%d,%d", ev.x, ev.y).c_str());
        }
        else {
            wnd_utils::set_title(nativeWin.get_wnd(), L"ON_LEAVE error");
            DD_ASSERT(false);
        }
        return true;
    });
    nativeWin.loop();
}

END_NSP_DDM
