#ifndef ini_file_h_ 
#define ini_file_h_ 1

#include "g_def.h"
#include <vector>
#include <memory>

BEG_NSP_DDM

class ini_file;
using sp_ini_file = std::shared_ptr<ini_file>;

// ini �ļ��Ķ�д��
class ini_file
{
public:
    ~ini_file();

    /** ����һ��ʵ��
    @param [in] sPath �ļ�·��
    @param [in] bAlwaysCreate �ļ�������ʱ�Ƿ��Զ��������ļ�������ʱ���Ϊfalse������0�������Զ������ļ�
    @return ����ʵ���������Լ��ͷ��ڴ�
   */
    static sp_ini_file create_obj(const ddstr& sPath, bool bAlwaysCreate = false);

    /** ���һ��key
    @param [in] sSection ��һ��section������������򴴽�
    @param [in] sKey ��һ��key
    @param [in] sValue Ҫ���õ�ֵ
    @return �Ƿ�ɹ�
   */
    bool add_key(const ddchar* sSection, const ddchar* sKey, const ddchar* sValue);

    /** ɾ��һ��key
    @param [in] sSection ��һ��section
    @param [in] sKey ��һ��key
    @return �Ƿ�ɹ�
   */
    bool del_key(const ddchar* sSection, const ddchar* sKey);

    /** ɾ��һ��section
    @param [in] sSection ��һ��section
    @return �Ƿ�ɹ�
   */
    bool del_section(const ddchar* sSection);

    /** �޸�һ��key��Ӧ��ֵ
    @param [in] sSection ��һ��section
    @param [in] sKey ��һ��key
    @param [in] sValue Ҫ���õ�ֵ
    @return �Ƿ�ɹ�
   */
    bool chang_key(const ddchar* sSection, const ddchar* sKey, const ddchar* sValue);

    /** ���һ��key��Ӧ��ֵ
    @param [in] sSection ��һ��section sSection == nullptr ʱ��ö�����е�section
    @param [in] sKey ��һ��key sKey == nullptr ö��section�����е�key
    @param [in] size �ڲ�buff��С��Ĭ�ϵĲ����õ�ʱ��ָ��
    @return key��Ӧ��ֵ
   */
    std::vector<ddstr> get(const ddchar* sSection, const ddchar* sKey, const int size = 512);

private:
    /** ���캯��
   */
    ini_file(const ddstr& sPath);

private:
    /** ȫ·��
   */
    ddstr    m_sFileFullPath;
};
END_NSP_DDM
#endif // ini_file_h_
