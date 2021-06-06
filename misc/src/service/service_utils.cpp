
#include "service_utils.h"

#include <VersionHelpers.h>

typedef long uint32_t;
typedef unsigned char uint8_t;
const uint32_t kMaxQueryConfigBufferBytes = 8 * 1024;

struct scoped_service {
    scoped_service(SC_HANDLE handle) : handle_(handle) {
    }

    ~scoped_service() {
        if (handle_) {
            CloseServiceHandle(handle_);
        }
    }

    operator bool() {
        return handle_ != nullptr;
    }

    SC_HANDLE handle_;
};

struct scoped_timer {
    scoped_timer() : start_(GetTickCount64()) {

    }

    ULONGLONG GetMilliseconds() {
        return GetTickCount64() - start_;
    }
    ULONGLONG start_;
};

HRESULT ScmDatabase::EnumerateServices(
    EnumerateServicesCallback callback, 
    void* callback_context)
{
    if (!callback)
        return E_POINTER;

    const wchar_t* kServicesRegKeyFromRoot =
        L"SYSTEM\\CurrentControlSet\\Services";

//     base::win::RegistryKeyIterator services_key_iterator(HKEY_LOCAL_MACHINE,
//         kServicesRegKeyFromRoot);
//     while (services_key_iterator.Valid()) {
//         callback(callback_context, services_key_iterator.Name());
//     }
    return S_OK;
}

bool ScmDatabase::IsServiceStateEqual(SC_HANDLE service, DWORD state) {
    DWORD bytes_needed_ignored = 0;
    BYTE buffer[8 * 1024] = { 0 };
    QUERY_SERVICE_CONFIG* service_config =
        reinterpret_cast<QUERY_SERVICE_CONFIG*>(buffer);
    if (!::QueryServiceConfig(service, service_config, sizeof(buffer),
        &bytes_needed_ignored)) {
        return false;
    }
    return (service_config[0].dwStartType == state);
}

bool ScmDatabase::IsServiceMarkedDeleted(SC_HANDLE service) {
    // Services that have been marked deleted are always in the
    // SERVICE_DISABLED state.  The converse is not true, and unfortunately
    // there is no way to check if a service has been marked deleted except by
    // attempting to change one of its configuration parameters, at which
    // point you get a specific error indicating it has been marked deleted.
    //
    // The following call to ChangeServiceConfig does not actually change any
    // of the service's configuration, but should hopefully return the
    // specific error if the service has been marked deleted.
    if (!::ChangeServiceConfig(service, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE,
        SERVICE_NO_CHANGE, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr) &&
        ::GetLastError() == ERROR_SERVICE_MARKED_FOR_DELETE) {
        return true;
    }
    else {
        return false;
    }
}

HRESULT ServiceInstall::InstallServices(
    const wchar_t* service_name,
    const wchar_t* service_display_name,
    const wchar_t* service_cmd_line,
    DWORD service_type,
    DWORD service_start_type) {
    if (IsServiceInstalled(service_name)) {
        HRESULT hr = UpgradeService(service_name, service_cmd_line, service_start_type);
        if (SUCCEEDED(hr)) {
            return hr;
        }

        // Delete the previous version of the service. Then create a new service
        // name, and fall through to install that.
        UninstallServices(service_name);
    }
    return DoInstallNewService(service_name, service_display_name, service_cmd_line, service_type, service_start_type);
}

HRESULT ServiceInstall::SetDescription(const wchar_t* service_name, const wchar_t* description) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
    if (!scm) {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    // Opening the service with less rights fails the ChangeServiceConfig2 call
    // with E_ACCESSDENIED.
    scoped_service service(::OpenService(scm.handle_, service_name, SERVICE_ALL_ACCESS));
    if (!service) {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    SERVICE_DESCRIPTION info = { const_cast<wchar_t*>(description) };
    if (!::ChangeServiceConfig2(service.handle_,
        SERVICE_CONFIG_DESCRIPTION,
        &info)) {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    return S_OK;
}

bool ServiceInstall::IsDelayedAutoStart(const wchar_t* service_name)
{
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT));
    if (!scm) {
        return false;
    }
    scoped_service service(::OpenService(scm.handle_, service_name, SERVICE_QUERY_CONFIG));
    if (!service) {
        return false;
    }

    uint8_t buffer[kMaxQueryConfigBufferBytes] = { 0 };
    DWORD bytes_needed_ignored = 0;
    SERVICE_DELAYED_AUTO_START_INFO* service_config =
        reinterpret_cast<SERVICE_DELAYED_AUTO_START_INFO*>(buffer);
    if (!::QueryServiceConfig2(service.handle_,
        SERVICE_CONFIG_DELAYED_AUTO_START_INFO,
        buffer,
        sizeof(buffer),
        &bytes_needed_ignored)) {
        return false;
    }
    return !!service_config->fDelayedAutostart;
}

HRESULT ServiceInstall::SetDelayedAutoStart(const wchar_t* service_name) {
    if (IsWindowsVistaOrGreater()) {
        return S_OK;
    }

    if (IsDelayedAutoStart(service_name)) {
        return S_OK;
    }

    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
    if (!scm) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_CHANGE_CONFIG));
    if (!service) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    SERVICE_DELAYED_AUTO_START_INFO auto_start_info = { TRUE };
    if (!::ChangeServiceConfig2(service.handle_,
        SERVICE_CONFIG_DELAYED_AUTO_START_INFO,
        &auto_start_info)) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return S_OK;
}

HRESULT ServiceInstall::UpgradeService(const wchar_t* service_name, const wchar_t* service_cmd_line, DWORD service_start_type) {
    if (IsServiceCorrectlyConfigured(service_cmd_line, service_cmd_line, service_start_type)) {
        return S_OK;
    }

    HRESULT hr(stopService(service_name));
    if (FAILED(hr)) {
        return hr;
    }

    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
    if (!scm) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_CHANGE_CONFIG));
    if (!service) {
        return HRESULT_FROM_WIN32(::GetLastError());
    }

    if (!::ChangeServiceConfig(service.handle_,
        SERVICE_WIN32_OWN_PROCESS,
        service_start_type,
        SERVICE_ERROR_NORMAL,
        service_cmd_line,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr)) {
        return HRESULT_FROM_WIN32(::GetLastError());
    }

    return S_OK;
}

bool ServiceInstall::IsServiceCorrectlyConfigured(const wchar_t* service_name, 
    const wchar_t* service_cmd_line,
    DWORD service_start_type) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT));
    if (!scm) {
        return false;
    }

    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_QUERY_CONFIG));

    if (!service) {
        return false;
    }

    uint8_t buffer[kMaxQueryConfigBufferBytes] = { 0 };
    DWORD bytes_needed_ignored = 0;
    QUERY_SERVICE_CONFIG* service_config =
        reinterpret_cast<QUERY_SERVICE_CONFIG*>(buffer);
    if (!::QueryServiceConfig(service.handle_,
        service_config,
        sizeof(buffer),
        &bytes_needed_ignored)) {
        return false;
    }

    bool does_service_cmd_line_match = false;
    if (service_config->lpBinaryPathName == nullptr || service_cmd_line == nullptr) {
        does_service_cmd_line_match =
            (service_config->lpBinaryPathName == service_cmd_line);
    }
    else {
        does_service_cmd_line_match =
            !lstrcmpi(service_config->lpBinaryPathName, service_cmd_line);
    }

    return service_config->dwServiceType == SERVICE_WIN32_OWN_PROCESS &&
        service_config->dwStartType == service_start_type &&
        service_config->dwErrorControl == SERVICE_ERROR_NORMAL &&
        does_service_cmd_line_match;
}

HRESULT ServiceInstall::UninstallServices(const wchar_t* service_name) {
    stopService(service_name);

    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
    if (!scm) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }
    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_CHANGE_CONFIG | DELETE));
    if (service) {
        ::DeleteService(service.handle_);
    }
    return S_OK;
}

bool ServiceInstall::IsServiceInstalled(const wchar_t* service_name) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT));
    if (!scm) {
        return false;
    }
    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_QUERY_CONFIG));
    return service.handle_ != nullptr;
}

bool ServiceInstall::CanInstallWithoutReboot(LPWSTR lpszServiceName) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
    if (!scm) {
        return false;
    }

    scoped_service service(::OpenService(scm.handle_,
        lpszServiceName,
        SERVICE_QUERY_CONFIG |
        SERVICE_CHANGE_CONFIG));
    if (!service) {
        DWORD last_error = ::GetLastError();
        if (last_error == ERROR_ACCESS_DENIED ||
            last_error == ERROR_INVALID_HANDLE) {
            return false;
        }
        else {
            // service does not exist
            return true;
        }
    }
    return !ScmDatabase::IsServiceMarkedDeleted(service.handle_);
}

HRESULT ServiceInstall::StartService(const wchar_t* service_name) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
    if (!scm) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_QUERY_STATUS | SERVICE_START));
    if (!service) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    SERVICE_STATUS status = { 0 };
    if (::QueryServiceStatus(service.handle_, &status)) {
        if (status.dwCurrentState == SERVICE_RUNNING ||
            status.dwCurrentState == SERVICE_START_PENDING) {
            return S_OK;
        }
    }

    // Start the service.
    if (::StartService(service.handle_, 0, nullptr)) {
        return S_OK;
    }

    HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
    if (hr == HRESULT_FROM_WIN32(ERROR_SERVICE_ALREADY_RUNNING)) {
        return S_OK;
    }
    return hr;
}

HRESULT ServiceInstall::stopService(const wchar_t* service_name) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
    if (!scm) {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_QUERY_STATUS | SERVICE_STOP));
    if (!service) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    SERVICE_STATUS status = { 0 };
    if (::QueryServiceStatus(service.handle_, &status)) {
        if (status.dwCurrentState != SERVICE_STOPPED &&
            status.dwCurrentState != SERVICE_STOP_PENDING) {
            // stop the service.
            ZeroMemory(&status, sizeof(SERVICE_STATUS));
            if (!::ControlService(service.handle_, SERVICE_CONTROL_STOP, &status)) {
                return HRESULT_FROM_WIN32(GetLastError());
            }
        }
    }

    if (status.dwCurrentState != SERVICE_STOPPED) {
        const int kWaitForServiceTostopMs = 8000;
        scoped_timer t;
        while (status.dwCurrentState != SERVICE_STOPPED &&
            t.GetMilliseconds() < kWaitForServiceTostopMs) {
            const int kSleepTimeMs = 50;
            ::Sleep(kSleepTimeMs);
            ::QueryServiceStatus(service.handle_, &status);
        }

        if (status.dwCurrentState != SERVICE_STOPPED) {
            return HRESULT_FROM_WIN32(ERROR_TIMEOUT);
        }
    }

    return S_OK;
}

HRESULT ServiceInstall::DoInstallNewService (
    const wchar_t* service_name,
    const wchar_t* service_display_name,
    const wchar_t* service_cmd_line,
    DWORD service_type,
    DWORD service_start_type) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
    if (!scm) {
        const DWORD error = ::GetLastError();
        return HRESULT_FROM_WIN32(error);
    }
    scoped_service service(::CreateService(scm.handle_,
        service_name,
        service_display_name,
        SERVICE_ALL_ACCESS,
        service_type,
        service_start_type,
        SERVICE_ERROR_NORMAL,
        service_cmd_line,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr));
    if (!service) {
        const DWORD error = ::GetLastError();
        return HRESULT_FROM_WIN32(error);
    }
    return S_OK;
}

bool ServiceUtils::IsServiceRunning(const wchar_t* service_name) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT));
    if (!scm) {
        return false;
    }

    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_QUERY_STATUS));
    if (!service) {    
        return false;
    }

    SERVICE_STATUS status = { 0 };
    if (!::QueryServiceStatus(service.handle_, &status)) {
        return false;
    }
    return status.dwCurrentState == SERVICE_RUNNING ||
        status.dwCurrentState == SERVICE_START_PENDING;
}

bool ServiceUtils::IsServiceDisabled(const wchar_t* service_name) {
    scoped_service scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT));
    if (!scm) {
        return false;
    }

    scoped_service service(::OpenService(scm.handle_,
        service_name,
        SERVICE_QUERY_CONFIG));
    if (!service) {
        return false;
    }

    return ScmDatabase::IsServiceStateEqual(service.handle_, SERVICE_DISABLED);
}
