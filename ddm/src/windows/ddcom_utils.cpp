#include "ddcom_utils.h"
#include "ddwin_utils.h"

#ifdef DD_WINDOW
BEG_NSP_DDM

std::wstring map_thread_model(com_thread_model model)
{
    switch (model)
    {
    case ddm::com_thread_model::Apartment:
        return L"Apartment";
    case ddm::com_thread_model::Both:
        return L"Both";
    case ddm::com_thread_model::Free:
        return L"Free";
    case ddm::com_thread_model::Neutral:
        return L"Neutral";
    }
    return L"";
}

HRESULT write_com_init_register(const std::wstring& clsid, const std::wstring& desc, com_thread_model threadModel, const std::wstring dllFullPath)
{
    LSTATUS status = ERROR_SUCCESS;
    HKEY clsidKey = NULL;
    HKEY inprocServer32Key = NULL;
    do {
        std::wstring clsidKeyStr = L"CLSID\\" + clsid;
        status = ::RegCreateKeyExW(HKEY_CLASSES_ROOT, clsidKeyStr.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &clsidKey, NULL);
        if (ERROR_SUCCESS != status) {
            break;
        }

        status = ::RegSetValueExW(clsidKey, NULL, 0, REG_SZ, (const BYTE*)desc.c_str(), (DWORD)desc.size() * 2);
        if (ERROR_SUCCESS != status) {
            break;
        }

        status = ::RegCreateKeyExW(clsidKey, L"InProcServer32", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &inprocServer32Key, NULL);
        if (ERROR_SUCCESS != status) {
            break;
        }

        status = ::RegSetValueExW(inprocServer32Key, NULL, 0, REG_SZ, (const BYTE*)dllFullPath.c_str(), (DWORD)dllFullPath.size() * 2);
        if (ERROR_SUCCESS != status) {
            break;
        }

        std::wstring threadModelStr = map_thread_model(threadModel);
        status = RegSetValueExW(inprocServer32Key, L"ThreadingModel", 0, REG_SZ, (const BYTE*)threadModelStr.c_str(), (DWORD)threadModelStr.size() * 2);
        if (ERROR_SUCCESS != status) {
            break;
        }
    } while (0);


    if (NULL != clsidKey) {
        ::RegCloseKey(clsidKey);
        clsidKey = NULL;
    }

    if (NULL != inprocServer32Key) {
        ::RegCloseKey(inprocServer32Key);
        inprocServer32Key = NULL;
    }

    return HRESULT_FROM_WIN32(status);
}

HRESULT write_com_uninit_register(const std::wstring& clsid)
{
    std::wstring clsidKeyStr = L"CLSID\\" + clsid;
    return HRESULT_FROM_WIN32(::RegDeleteTree(HKEY_CLASSES_ROOT, clsidKeyStr.c_str()));
}
END_NSP_DDM
#endif

