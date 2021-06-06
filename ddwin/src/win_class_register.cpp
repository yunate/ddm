#include "win_class_register.h"
#include "simple_native_window.h"
BEG_NSP_DDM

bool win_class_register::regist_window_class1(HINSTANCE hInst, const ddstr& className)
{
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX),0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = base_native_window::win_proc;
    wcex.hInstance = hInst;
    wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = className.c_str();
    return ::RegisterClassEx(&wcex);
}

bool win_class_register::regist_window_class2(HINSTANCE hInst, const ddstr& className)
{
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX),0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_IME;
    wcex.lpfnWndProc = base_native_window::win_proc;
    wcex.hInstance = hInst;
    wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = className.c_str();
    return ::RegisterClassEx(&wcex);
}

void win_class_register::regist_all(HINSTANCE hInst)
{
    (void)win_class_register::regist_window_class1(hInst, simple_native_window::get_class_name());
}
END_NSP_DDM

