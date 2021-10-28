#include "ddprocess.h"
#include "ddassert.h"
#include "windows/ddsecurity.h"
#include <tlhelp32.h>

BEG_NSP_DDM
bool ddprocess::get_process_ids(const std::wstring& processName, std::vector<DWORD>& ids)
{
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) {
        DDLOG_LASTERROR();
        return false;
    }
    ::PROCESSENTRY32 pe = { sizeof(pe) };
    BOOL flag = ::Process32FirstW(hSnapshot, &pe);
    while (flag) {
        if (processName == pe.szExeFile) {
            ids.push_back(pe.th32ProcessID);
        }
        flag = ::Process32NextW(hSnapshot, &pe);
    }

    ::CloseHandle(hSnapshot);
    return true;
}

std::wstring ddprocess::get_process_fullpath(HANDLE hProcess)
{
    // 获取全路径
    WCHAR fullPath[MAX_PATH + 1];
    DWORD buffSize = ARRAYSIZE(fullPath) - 1;
    if (0 == ::QueryFullProcessImageNameW(hProcess, 0, fullPath, &buffSize)) {
        DDLOG_LASTERROR();
        return L"";
    }
    return fullPath;
}

bool ddprocess::is_process_x64(HANDLE hProcess)
{
    HMODULE hModule = ::GetModuleHandleW(L"kernel32");
    if (hModule == NULL) {
        return false;
    }
    typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(hModule, "IsWow64Process");
    if (NULL == fnIsWow64Process) {
        return false;
    }

    BOOL bIsWow64 = FALSE;
    fnIsWow64Process(hProcess, &bIsWow64);
    return (bIsWow64 == FALSE);
}


HMODULE ddprocess::get_moudle(DWORD processId, const std::wstring& moudleName)
{
    if (moudleName.empty()) {
        return NULL;
    }
    DD_ASSERT(processId != 0);
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
    if (INVALID_HANDLE_VALUE == hSnapshot) {
        DDLOG_LASTERROR();
        return NULL;
    }

    bool find = false;
    MODULEENTRY32 me = { 0 };
    me.dwSize = sizeof(MODULEENTRY32);
    BOOL result = ::Module32First(hSnapshot, &me);
    while (result) {
        if (moudleName == me.szModule) {
            find = true;
            break;
        }
        result = ::Module32Next(hSnapshot, &me);
    }

    ::CloseHandle(hSnapshot);
    if (!find) {
        return NULL;
    }
    return me.hModule;
}

ddprocess::~ddprocess()
{
    uninit();
}

HANDLE ddprocess::get_handle()
{
    return m_hProcess;
}

const std::wstring& ddprocess::get_name()
{
    return m_name;
}

const std::wstring& ddprocess::get_fullpath()
{
    return m_fullPath;
}

DWORD ddprocess::get_id()
{
    return m_id;
}

DWORD ddprocess::get_parent_id()
{
    return m_parent_id;
}

bool ddprocess::is_x64()
{
    return m_x64;
}

bool ddprocess::init(DWORD processId)
{
    uninit();
    do {
        m_id = processId;
        m_hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_id);
        if (m_hProcess == NULL) {
            DDLOG_LASTERROR();
            break;
        }

        // 获取 parent id 和 name
        HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (INVALID_HANDLE_VALUE == hSnapshot) {
            DDLOG_LASTERROR();
            break;
        }
        ::PROCESSENTRY32 pe = { sizeof(pe) };
        BOOL flag = ::Process32FirstW(hSnapshot, &pe);
        while (flag) {
            if (m_id == pe.th32ProcessID) {
                m_name = pe.szExeFile;
                m_parent_id = pe.th32ModuleID;
                break;
            }
            flag = ::Process32NextW(hSnapshot, &pe);
        }

        ::CloseHandle(hSnapshot);
        if (m_name.empty()) {
            DDLOG(WARNING, str_utils::str_format("can not find process id:%d", m_id));
            break;
        }
        // 获取全路径
        m_fullPath = get_process_fullpath(m_hProcess);
        if (m_fullPath.empty()) {
            break;
        }

        // 是否是64位
        m_x64 = is_process_x64(m_hProcess);
        return true;
    } while (0);

    uninit();
    return false;
}

bool ddprocess::init(const std::wstring& name)
{
    uninit();
    do {
        m_name = name;

        // 获取 parent id 和 id
        HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (INVALID_HANDLE_VALUE == hSnapshot) {
            DDLOG_LASTERROR();
            break;
        }
        ::PROCESSENTRY32 pe = { sizeof(pe) };
        BOOL flag = ::Process32FirstW(hSnapshot, &pe);
        while (flag) {
            if (m_name == pe.szExeFile) {
                m_id = pe.th32ProcessID;
                m_parent_id = pe.th32ModuleID;
                break;
            }
            flag = ::Process32NextW(hSnapshot, &pe);
        }
        ::CloseHandle(hSnapshot);
        if (m_id == 0) {
            DDLOG(WARNING, str_utils::str_format("can not find process:%s", m_name.c_str()));
            break;
        }

        m_hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_id);
        if (m_hProcess == NULL) {
            DDLOG_LASTERROR();
            break;
        }

        // 获取全路径
        m_fullPath = get_process_fullpath(m_hProcess);
        if (m_fullPath.empty()) {
            break;
        }

        // 是否是64位
        m_x64 = is_process_x64(m_hProcess);
        return true;
    } while (0);

    uninit();
    return false;
}

bool ddprocess::init(HANDLE hProcess, char dummy)
{
    dummy;
    uninit();
    do {
        m_hProcess = hProcess;
        if (m_hProcess == nullptr) {
            DDLOG(WARNING, "hProcess is nullptr");
            break;
        }

        m_id = ::GetProcessId(hProcess);
        if (m_id == 0) {
            DDLOG_LASTERROR();
            break;
        }

        // 获取 parent id 和 name
        HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (INVALID_HANDLE_VALUE == hSnapshot) {
            DDLOG_LASTERROR();
            break;
        }
        ::PROCESSENTRY32 pe = { sizeof(pe) };
        BOOL flag = ::Process32FirstW(hSnapshot, &pe);
        while (flag) {
            if (m_id == pe.th32ProcessID) {
                m_name = pe.szExeFile;
                m_parent_id = pe.th32ModuleID;
                break;
            }
            flag = ::Process32NextW(hSnapshot, &pe);
        }

        ::CloseHandle(hSnapshot);
        if (m_name.empty()) {
            DDLOG(WARNING, str_utils::str_format("can not find process:%x", m_hProcess));
            break;
        }

        // 获取全路径
        m_fullPath = get_process_fullpath(m_hProcess);
        if (m_fullPath.empty()) {
            break;
        }

        // 是否是64位
        m_x64 = is_process_x64(m_hProcess);
        return true;
    } while (0);
    return false;
}

void ddprocess::uninit()
{
    if (m_hProcess != nullptr) {
        (void)::CloseHandle(m_hProcess);
    }
    m_hProcess = nullptr;
    m_name.clear();
    m_fullPath.clear();
    m_id = 0;
    m_parent_id = 0;
    m_x64 = false;
}

////////////////////////////////ddprocess_mutex//////////////////////////////////////////
bool ddprocess_mutex::init(const std::wstring& name)
{
    // 以低权限创建
    SECURITY_ATTRIBUTES sa;
    sa.bInheritHandle = FALSE;
    SECURITY_DESCRIPTOR sd;
    sa.lpSecurityDescriptor = (void*)&sd;
    if (ERROR_SUCCESS != create_low_sa(sa)) {
        return false;
    }

    m_mutex = ::CreateMutexW(&sa, false, (name + L"_MUTEX").c_str());
    return (m_mutex != NULL);
}

bool ddprocess_mutex::lock(u32 waitTime /*= 1000*/)
{
    if (m_mutex == NULL) {
        return false;
    }
    DWORD hr = ::WaitForSingleObject(m_mutex, waitTime);
    if (hr == WAIT_ABANDONED || hr == WAIT_OBJECT_0) {
        return true;
    }
    return false;
}

void ddprocess_mutex::unlock()
{
    if (m_mutex != NULL) {
        (void)::ReleaseMutex(m_mutex);
    }
}

END_NSP_DDM
