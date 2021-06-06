#ifndef single_limited_h_
#define single_limited_h_ 1

#include "g_def.h"
#include <vector>
#include <windows.h>
BEG_NSP_DDM

// ���Դ���������ĳЩ������������ʵ��Ψһ
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

    // ����false������ʧ��
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
