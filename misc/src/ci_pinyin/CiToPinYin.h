#ifndef __CI_TO_PINYIN_H_
#define __CI_TO_PINYIN_H_

#include <unordered_map>
#include <vector>
#include <tchar.h>

#ifdef _UNICODE
typedef std::wstring TString;
#else
typedef std::string TString;
#endif

/** 单字转拼音
*/
class CiToPinYin
{
protected:
    CiToPinYin();

    CiToPinYin(const CiToPinYin&){ }

    CiToPinYin& operator=(const CiToPinYin&) { }

public:
    /** 获得拼音表
    @param [in] ci 单字
    @param [out] out 结果
    @return 是否成功
    */
     bool GetCiPinYin(const TCHAR& ci, std::vector<TString>& out);

     /** 获得拼音表
     @param [in] src 句子
     @param [out] out 结果
     @return 是否成功
     */
     bool GetStrPinYin(const TString& src, std::vector<TString>& out);

private:
    /** 获得拼音表的递归
     @param [in] src 句子
     @param [in] index 句子的下标
     @param [in] tmp 过程值
     @param [out] out 结果
     @return 是否成功
     */
    bool GetStrPinYin_Core(const TString& src, size_t index, const TString& tmp, std::vector<TString>& out);

private:
    /** 对照表
    */
    std::unordered_map<TCHAR, std::vector<TString> > m_ciToPinYin;
};

#endif //__CI_TO_PINYIN_H_
