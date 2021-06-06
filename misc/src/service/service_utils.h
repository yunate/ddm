#ifndef __SERVICE_UTILS_H_
#define __SERVICE_UTILS_H_

#include <windows.h>
/**
 *   omaha/base/service_util.h/cc
 *
 * 
 */
class ScmDatabase 
{
public:
    typedef HRESULT(*EnumerateServicesCallback)(void* callback_context,
        const wchar_t* service_name);

    static HRESULT EnumerateServices(EnumerateServicesCallback callback,
        void* callback_context);

    static bool IsServiceStateEqual(SC_HANDLE service, DWORD state);

    static bool IsServiceMarkedDeleted(SC_HANDLE service);
};

class ServiceInstall 
{
public:
    static HRESULT InstallServices(
        const wchar_t* service_name,
        const wchar_t* service_display_name,
        const wchar_t* service_cmd_line,
        DWORD service_type,
        DWORD service_start_type);

    static HRESULT UpgradeService(const wchar_t* service_name,
                                  const wchar_t* service_cmd_line,
                                  DWORD service_start_type);

    static bool IsServiceCorrectlyConfigured(const wchar_t* service_name, 
        const wchar_t* service_cmd_line, DWORD service_start_type);

    static HRESULT UninstallServices(const wchar_t* service_name);

    static bool IsServiceInstalled(const wchar_t* service_name);
    static bool CanInstallWithoutReboot(LPWSTR lpszServiceName);

    static HRESULT StartService(const wchar_t* service_name);
    static HRESULT stopService(const wchar_t* service_name);

    static HRESULT SetDescription(const wchar_t* service_name, const wchar_t* description);
    static bool IsDelayedAutoStart(const wchar_t* service_name);

    static HRESULT SetDelayedAutoStart(const wchar_t* service_name);

private:
    static HRESULT DoInstallNewService(
        const wchar_t* service_name, 
        const wchar_t* service_display_name,
        const wchar_t* service_cmd_line, 
        DWORD service_type,
        DWORD service_start_type);
};

class ServiceUtils 
{
public:
    static bool IsServiceRunning(const wchar_t* service_name);
    static bool IsServiceDisabled(const wchar_t* service_name);
};

#endif // __SERVICE_UTILS_H_