
#ifndef file_writer_h_
#define file_writer_h_ 1

#include "g_def.h"
#include "dir_utils.h"
#include <functional>
#include <memory>
#include <string>

BEG_NSP_DDM

/** 文件读写类，需要文件存在，如果文件大小为0 open 函数会去写校验头，否者校验校验头
*/
class file_writer {
public:
    /** 构造函数
    */
    file_writer(const ddstr& path);

    /** 析构函数
    */
    virtual ~file_writer();

    /** 打开文件
    @param [in] path 路径
    @param [in] checker 文件头检查
    @param [in] 文件头大小
    @return 是否成功
    */
    bool open(u8* checker, const u32 checkSize);

    /** 写 wchar
    @param [in] buff 输入数组
    @param [in] size 数组大小
    @return 写入个数 大小为 2 * size
    */
    u32 write_buffw(const ddcharw* buff, const u32 size);

    /** 写char
    @param [out] line 输出
    @return 写入个数
    */
    u32 write_buffa(const ddchara* buff, const u32 size);

    /** 获得文件大小
    @return 文件大小
    */
    u64 get_file_size();

protected:
    FILE* m_pFile;
    ddstr m_path;
};

using sp_file_writer = std::shared_ptr<file_writer>;

static bool create_file_if_not_exit(const ddstr& path)
{
    if (!dir_utils::is_path_exist(path)) {
        return dir_utils::create_file(path);
    }

    if (dir_utils::is_dir(path)) {
        return false;
    }

    return true;
}

/** 创建一个UCS-2 Little Endian 编码的文件 使用getlinew()
@return file_rw的shared_ptr
*/
inline sp_file_writer create_UCS2_file_writer(const ddstr& path)
{
    if (create_file_if_not_exit(path)) {
        return nullptr;
    }

    u8 header[] = { 0xff, 0xfe };
    sp_file_writer spFileRW(new (std::nothrow) file_writer(path));

    if (spFileRW == nullptr || !spFileRW->open(header, 2)) {
        return nullptr;
    }

    return spFileRW;
}

/** 创建一个UTF8 编码的文件 使用getlinea()
@return file_rw的shared_ptr
*/
inline sp_file_writer create_UTF8_file_writer(const ddstr& path)
{
    if (!create_file_if_not_exit(path)) {
        return nullptr;
    }

    sp_file_writer spFileRW(new (std::nothrow) file_writer(path));

    if (spFileRW == nullptr || !spFileRW->open(nullptr, 0)) {
        return nullptr;
    }

    return spFileRW;
}

/** 创建一个UTF8 BOM 编码的文件 使用getlinea()
@return file_rw的shared_ptr
*/
inline sp_file_writer create_UTF8Bom_file_writer(const ddstr& path)
{
    if (!create_file_if_not_exit(path)) {
        return nullptr;
    }

    u8 header[] = { 0xef, 0xbb, 0xbf };
    sp_file_writer spFileRW(new (std::nothrow) file_writer(path));

    if (spFileRW == nullptr || !spFileRW->open(header, 3)) {
        return nullptr;
    }

    return spFileRW;
}

END_NSP_DDM
#endif // file_writer_h_
