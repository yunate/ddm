#include "resource_util.h"
#include "file/file_writer.h"

BEG_NSP_DDM

bool resource_util::release_res(HMODULE hModule, DWORD resId, const ddstr& resTy, const ddstr& target)
{
    HRSRC hrc = ::FindResource(hModule, MAKEINTRESOURCE(resId), resTy.c_str());
    if (hrc == nullptr) {
        return false;
    }

    HGLOBAL hGlobal = ::LoadResource(hModule, hrc);
    if (hGlobal == nullptr) {
        return false;
    }

    DWORD dwSize = ::SizeofResource(hModule, hrc);
    LPVOID pData = ::LockResource(hGlobal);

    sp_file_writer spFileWriter = create_UTF8_file_writer(target);

    if (spFileWriter == nullptr) {
        return false;
    }

    return spFileWriter->write_buffa((ddchara*)pData, dwSize);
}
END_NSP_DDM
