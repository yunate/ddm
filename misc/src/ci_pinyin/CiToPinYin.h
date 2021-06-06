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

/** ����תƴ��
*/
class CiToPinYin
{
protected:
    CiToPinYin();

    CiToPinYin(const CiToPinYin&){ }

    CiToPinYin& operator=(const CiToPinYin&) { }

public:
    /** ���ƴ����
    @param [in] ci ����
    @param [out] out ���
    @return �Ƿ�ɹ�
    */
     bool GetCiPinYin(const TCHAR& ci, std::vector<TString>& out);

     /** ���ƴ����
     @param [in] src ����
     @param [out] out ���
     @return �Ƿ�ɹ�
     */
     bool GetStrPinYin(const TString& src, std::vector<TString>& out);

private:
    /** ���ƴ����ĵݹ�
     @param [in] src ����
     @param [in] index ���ӵ��±�
     @param [in] tmp ����ֵ
     @param [out] out ���
     @return �Ƿ�ɹ�
     */
    bool GetStrPinYin_Core(const TString& src, size_t index, const TString& tmp, std::vector<TString>& out);

private:
    /** ���ձ�
    */
    std::unordered_map<TCHAR, std::vector<TString> > m_ciToPinYin;
};

#endif //__CI_TO_PINYIN_H_
