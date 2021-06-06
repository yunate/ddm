#ifndef single_limited_h_
#define single_limited_h_ 1

#include "g_def.h"
#include <vector>
#include <windows.h>
BEG_NSP_DDM

// 尝试创建并持有某些对象，用来限制实例唯一
class single_limited
{
public:
    ~single_limited()
    {
        ::CloseHandle(m_handles);
    }

    bool test(const ddstr& mutexName)
    {
        bool exit = false;
        HANDLE mutex = ::CreateMutex(nullptr, FALSE, mutexName.c_str());
        if (mutex == nullptr) {
            return exit;
        }

        if (ERROR_ALREADY_EXISTS != ::GetLastError()) {
            exit = true;
            return true;
        }

        (void)::CloseHandle(mutex);
        return exit;
    }

    // 返回false，持有失败
    bool try_hold_mutex(const ddstr& mutexName)
    {
        HANDLE mutex = ::CreateMutex(nullptr, FALSE, mutexName.c_str());
        if (mutex == nullptr) {
            return false;
        }

        if (ERROR_ALREADY_EXISTS != ::GetLastError())
        {
            m_handles = mutex;
            return true;
        }

        (void)::CloseHandle(mutex);
        return false;
    }

private:
    HANDLE m_handles = NULL;
};

END_NSP_DDM
#endif // single_limted_h_
