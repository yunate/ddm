
#ifndef color_print_h_
#define color_print_h_

#include "g_def.h"
#include <Windows.h>
#include <stdio.h>
BEG_NSP_DDM

// 控制台前景颜色
enum console_color
{
    Red = FOREGROUND_INTENSITY | FOREGROUND_RED,
    Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    Purple = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    Gray = FOREGROUND_INTENSITY,
    White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    HighWhite = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    Black = 0,
};

#define color_printf(foreColor, fmt, ...)\
do {\
    HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);\
    CONSOLE_SCREEN_BUFFER_INFO info;\
    if (handle != nullptr && ::GetConsoleScreenBufferInfo(handle, &info))\
    {\
        ::SetConsoleTextAttribute(handle, foreColor | Black);\
        ::printf(fmt, __VA_ARGS__);\
        ::SetConsoleTextAttribute(handle, info.wAttributes);\
    }\
}while (0)

#define color_printfw(foreColor, fmt, ...)\
do {\
    HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);\
    CONSOLE_SCREEN_BUFFER_INFO info;\
    if (handle != nullptr && ::GetConsoleScreenBufferInfo(handle, &info))\
    {\
        ::SetConsoleTextAttribute(handle, foreColor | Black);\
        ::wprintf(fmt, __VA_ARGS__);\
        ::SetConsoleTextAttribute(handle, info.wAttributes);\
    }\
}while (0)
END_NSP_DDM
#endif // color_print_h_
