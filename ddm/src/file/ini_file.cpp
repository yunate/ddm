#include "ini_file.h"
#ifdef DD_WINDOW
#include "dir_utils.h"
#include <Windows.h>

BEG_NSP_DDM

ini_file::~ini_file()
{
}

ini_file::ini_file(const ddstr& sPath)
    : m_sFileFullPath(sPath) {}

sp_ini_file ini_file::create_obj(const ddstr& sPath, bool bAlwaysCreate)
{
    if (dir_utils::is_path_exist(sPath)) {
        return sp_ini_file(new(std::nothrow) ini_file(sPath));
    }

    if (bAlwaysCreate) {
        dir_utils::create_file(sPath);
        return sp_ini_file(new(std::nothrow) ini_file(sPath));
    }

    return sp_ini_file(nullptr);
}

bool ini_file::add_key(const ddchar* sSection, const ddchar* sKey, const ddchar* sValue)
{
    if (!sSection) {
        return false;
    }

    return 0 == ::WritePrivateProfileString(sSection, sKey, sValue, m_sFileFullPath.c_str());
}

bool ini_file::del_key(const ddchar* sSection, const ddchar* sKey)
{
    add_key(sSection, sKey, 0);
    return false;
}

bool ini_file::del_section(const ddchar* sSection)
{
    return add_key(sSection, 0, 0);
}

bool ini_file::chang_key(const ddchar* sSection, const ddchar* sKey, const ddchar* sValue)
{
    return add_key(sSection, sKey, sValue);
}

std::vector<ddstr> ini_file::get(const ddchar* sSection, const ddchar* sKey, const int size/* = 512*/)
{
    std::vector<ddstr> rtn;
    ddchar* buff = new(std::nothrow) ddchar[size];

    if (buff == nullptr) {
        return rtn;
    }

    ::memset(buff, 0, size);
    ::GetPrivateProfileString(sSection, sKey, _DDT(""), buff, size, m_sFileFullPath.c_str());

    if (sSection == nullptr || sKey == nullptr) {
        ddstr tmp;

        for (int i = 0; i < size; ++i) {
            if (buff[i] == 0) {
                if (tmp.empty()) {
                    break;
                } else {
                    rtn.push_back(tmp);
                    tmp.clear();
                }
            } else {
                tmp.append(1, buff[i]);
            }
        }
    } else {
        rtn.push_back(buff);
    }

    delete[]buff;
    buff = nullptr;
    return rtn;
}

END_NSP_DDM
#endif