
#include "test_case_factory.h"
#include "simple_native_window.h"
#include "win_class_register.h"
#include "thread/loop/win_msg_loop.h"

BEG_NSP_DDM
TEST(test_ddwin, simple_native_window)
{
    return;
    HINSTANCE hinst = ::GetModuleHandle(NULL);
    win_class_register::regist_all(hinst);
    simple_native_window nativeWin;
    nativeWin.create(_DDT("simple_native_window"), hinst);
    nativeWin.update_win_pos(500, 200, 500, 300);
    win_msg_loop loop;
    loop.loop();
}

class simple_native_window1 : public simple_native_window
{
public:
    virtual LRESULT CALLBACK win_proc_chain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
            return S_OK;
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
    HINSTANCE hinst = ::GetModuleHandle(NULL);
    win_class_register::regist_all(hinst);
    simple_native_window1 nativeWin;
    (void)nativeWin.create(_DDT("simple_native_window"), hinst);
    nativeWin.update_win_pos(500, 200, 500, 300);
    nativeWin.loop();
}

END_NSP_DDM
