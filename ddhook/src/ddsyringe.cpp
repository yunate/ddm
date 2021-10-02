#include "ddsyringe.h"
#include "ddsyringe.h"
#include "windows/ddwin_utils.h"
#include "file/dir_utils.h"
#include "file/ddpath.h"
#include <tlhelp32.h>

ddclassic_syringe::ddclassic_syringe(const std::shared_ptr<ddprocess>& process, const std::wstring& dllFullPath)
    : m_process(process), m_dllFullPath(dllFullPath)
{ }

bool ddclassic_syringe::check_param()
{
    if (m_process == nullptr || m_process->get_id() == 0) {
        DDLOG(WARNING, "m_process is invalid");
        return false;
    }

    if (!dir_utils::is_path_exist(m_dllFullPath) || dir_utils::is_dir(m_dllFullPath)) {
        DDLOG(WARNING, str_utils::str_format("m_dllFullPath is not a valid dll path:%s", m_dllFullPath.c_str()));
        return false;
    }

    HANDLE hCurrentProcess = ::GetCurrentProcess();
    if (hCurrentProcess == NULL) {
        DDLOG_LASTERROR();
        return false;
    }

    if (ddprocess::is_process_x64(hCurrentProcess) != m_process->is_x64()) {
        DDLOG_EX(WARNING, str_utils::str_format("target is %s, when current is not", m_process->is_x64() ? "x64" : "win32").c_str());
        return false;
    }

    return true;
}

bool ddclassic_syringe::inject_dll(u32 waitTime /*= 5000*/)
{
    if (!check_param()) {
        return false;
    }

    HMODULE hMoudle = NULL;
    bool result = false;
    do {
        hMoudle = ::LoadLibraryW(L"kernel32.dll");
        if (hMoudle == NULL) {
            DDLOG_LASTERROR();
            break;
        }
        void* enterAddr = ::GetProcAddress(hMoudle, "LoadLibraryW");
        if (enterAddr == NULL) {
            DDLOG_LASTERROR();
            break;
        }
        result = exec_remote(m_process->get_handle(), enterAddr, (void*)m_dllFullPath.data(), m_dllFullPath.size() * 2, waitTime);

    } while (0);
    if (hMoudle != NULL) {
        (void)::FreeLibrary(hMoudle);
    }
    return result;
}

bool ddclassic_syringe::uninject_dll(u32 waitTime /*= 5000*/)
{
    if (!check_param()) {
        return false;
    }

    HMODULE hMoudle = NULL;
    HANDLE hRemoteThread = NULL;
    bool result = false;
    do {
        hMoudle = ::LoadLibraryW(L"kernel32.dll");
        if (hMoudle == NULL) {
            DDLOG_LASTERROR();
            break;
        }
        void* enterAddr = ::GetProcAddress(hMoudle, "FreeLibrary");
        if (enterAddr == NULL) {
            DDLOG_LASTERROR();
            break;
        }

        HMODULE hModule = ddprocess::get_moudle(m_process->get_id(), ddpath::path_name(m_dllFullPath));
        if (hModule == NULL) {
            break;
        }

        // 在目标进程执行
        hRemoteThread = ::CreateRemoteThread(m_process->get_handle(), NULL, 0, (LPTHREAD_START_ROUTINE)enterAddr, hModule, 0, NULL);
        if (hRemoteThread == NULL) {
            DDLOG_LASTERROR();
            return false;
        }
        ::WaitForSingleObject(hRemoteThread, (DWORD)waitTime);
        result = true;
    } while (0);
    if (hMoudle != NULL) {
        (void)::FreeLibrary(hMoudle);
    }
    if (hRemoteThread != NULL) {
        ::CloseHandle(hRemoteThread);
    }
    return result;
}

bool ddclassic_syringe::exec_remote(HANDLE hProcess, void* enterPoint, void* buff, u32 buffSize, u32 waitTime /*= 0*/)
{
    DD_ASSERT(hProcess != NULL);
    DD_ASSERT(enterPoint != NULL);
    DD_ASSERT(buff != NULL);
    bool result = false;
    PVOID remoteBuff = NULL;
    HANDLE hRemoteThread = NULL;
    do {
        // 将数据写入目标进程
        remoteBuff = ::VirtualAllocEx(hProcess, NULL, buffSize, MEM_COMMIT, PAGE_READWRITE);
        if (remoteBuff == nullptr) {
            DDLOG(WARNING, "VirtualAllocEx failure");
            break;
        }

        SIZE_T written = 0;
        if (!::WriteProcessMemory(hProcess, remoteBuff, buff, buffSize, &written) ||
            written == 0) {
            DDLOG(WARNING, "WriteProcessMemory failure");
            break;
        }

        // 在目标进程执行
        hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)enterPoint, remoteBuff, 0, NULL);
        if (hRemoteThread == NULL) {
            DDLOG_LASTERROR();
            return false;
        }
        ::WaitForSingleObject(hRemoteThread, (DWORD)waitTime);
        result = true;
    } while (0);


    if (remoteBuff != NULL) {
        ::VirtualFreeEx(hProcess, remoteBuff, 0, MEM_RELEASE);
    }

    if (hRemoteThread != NULL) {
        ::CloseHandle(hRemoteThread);
    }
    return result;
}

