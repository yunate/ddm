#include "ddpath.h"
BEG_NSP_DDM
std::wstring ddpath::path_name(const std::wstring& path)
{
    size_t idx = path.find_last_of('\\');
    if (idx == std::wstring::npos) {
        idx = path.find_last_of('/');
        if (idx == std::wstring::npos) {
            return L"";
        }
    }

    return path.substr(idx + 1);
}
END_NSP_DDM