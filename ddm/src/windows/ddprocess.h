#ifndef ddprocess_h_
#define ddprocess_h_

#include "g_def.h"
#include "nocopyable.hpp"
#include "ddwin_utils.h"
#include <windows.h>
BEG_NSP_DDM
#ifdef DD_WINDOW
class ddprocess : private nocopyable
{
public:
    // 进程名称枚举所有的进程id
    // processName 进程名称, 比如 notepad.exe
    static bool get_process_ids(const std::wstring& processName, std::vector<DWORD>& ids);
    static std::wstring get_process_fullpath(HANDLE hProcess);
    static bool is_process_x64(HANDLE hProcess);
    // 获得其加载了的模块的handle, 返回值不需要销毁
    static HMODULE get_moudle(DWORD processId, const std::wstring& moudleName);
public:
    ddprocess() = default;
    ~ddprocess();

    HANDLE get_handle();
    const std::wstring& get_name();
    const std::wstring& get_fullpath();
    DWORD get_id();
    DWORD get_parent_id();
    bool is_x64();
    bool init(DWORD processId);
    // 有多个重名的取第一个
    bool init(const std::wstring& processName);
    bool init(HANDLE hProcess, char dummy);

    void uninit();

private:
    HANDLE m_hProcess = nullptr;
    std::wstring m_name;
    std::wstring m_fullPath;
    DWORD m_id = 0;
    DWORD m_parent_id = 0;
    bool m_x64 = false;
};

class ddprocess_mutex : public nocopyable
{
public:
    ddprocess_mutex() = default;
    ~ddprocess_mutex() = default;

    bool init(const std::wstring& name);
    bool lock(u32 waitTime = 1000);
    void unlock();

protected:
    HANDLE m_mutex = NULL;
};

#endif
END_NSP_DDM
#endif //
