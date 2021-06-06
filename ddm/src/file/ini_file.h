#ifndef ini_file_h_ 
#define ini_file_h_ 1

#include "g_def.h"
#include <vector>
#include <memory>

BEG_NSP_DDM

class ini_file;
using sp_ini_file = std::shared_ptr<ini_file>;

// ini 文件的读写等
class ini_file
{
public:
    ~ini_file();

    /** 创建一个实例
    @param [in] sPath 文件路径
    @param [in] bAlwaysCreate 文件不存在时是否自动创建，文件不存在时如果为false，返回0，否则自动创建文件
    @return 返回实例对象，需自己释放内存
   */
    static sp_ini_file create_obj(const ddstr& sPath, bool bAlwaysCreate = false);

    /** 添加一个key
    @param [in] sSection 哪一个section，如果不存在则创建
    @param [in] sKey 哪一个key
    @param [in] sValue 要设置的值
    @return 是否成功
   */
    bool add_key(const ddchar* sSection, const ddchar* sKey, const ddchar* sValue);

    /** 删除一个key
    @param [in] sSection 哪一个section
    @param [in] sKey 哪一个key
    @return 是否成功
   */
    bool del_key(const ddchar* sSection, const ddchar* sKey);

    /** 删除一个section
    @param [in] sSection 哪一个section
    @return 是否成功
   */
    bool del_section(const ddchar* sSection);

    /** 修改一个key对应的值
    @param [in] sSection 哪一个section
    @param [in] sKey 哪一个key
    @param [in] sValue 要设置的值
    @return 是否成功
   */
    bool chang_key(const ddchar* sSection, const ddchar* sKey, const ddchar* sValue);

    /** 获得一个key对应的值
    @param [in] sSection 哪一个section sSection == nullptr 时候，枚举所有的section
    @param [in] sKey 哪一个key sKey == nullptr 枚举section下所有的key
    @param [in] size 内部buff大小，默认的不够用的时候指定
    @return key对应的值
   */
    std::vector<ddstr> get(const ddchar* sSection, const ddchar* sKey, const int size = 512);

private:
    /** 构造函数
   */
    ini_file(const ddstr& sPath);

private:
    /** 全路径
   */
    ddstr    m_sFileFullPath;
};
END_NSP_DDM
#endif // ini_file_h_
