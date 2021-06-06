#include "CiToPinYin.h"

extern const TCHAR* g_ci_pinyin_table[][2];
extern const unsigned int g_ci_pinyin_table_size;

CiToPinYin::CiToPinYin()
{
    for (unsigned int i = 0; i < g_ci_pinyin_table_size; ++i)
    {
        m_ciToPinYin[g_ci_pinyin_table[i][0][0]].push_back(g_ci_pinyin_table[i][1]);
    }
}

bool CiToPinYin::GetCiPinYin(const TCHAR & ci, std::vector<TString>& out)
{
    auto it = m_ciToPinYin.find(ci);

    if (it != m_ciToPinYin.end())
    {
        out = it->second;
        return true;
    }

    return false;
}

bool CiToPinYin::GetStrPinYin(const TString& src, std::vector<TString>& out)
{
    return GetStrPinYin_Core(src, 0, L"", out);
}

bool CiToPinYin::GetStrPinYin_Core(const TString& src, size_t index, const TString& tmp, std::vector<TString>& out)
{
    if (index == src.length())
    {
        out.push_back(tmp);
        return true;
    }

    auto it = m_ciToPinYin.find(src[index]);

    if (it != m_ciToPinYin.end())
    {
        bool res = true;

        for (size_t i = 0; i < it->second.size(); ++i)
        {
            res &= GetStrPinYin_Core(src, index + 1, tmp + it->second[i], out);
        }

        return res;
    }

    return false;
}


