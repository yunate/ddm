
#ifndef file_reader_h_
#define file_reader_h_ 1

#include "g_def.h"
#include <functional>
#include <memory>
#include <string>

BEG_NSP_DDM

/** 文件读写类，需要文件存在，如果文件大小为0 open 函数会去写校验头，否者校验校验头
*/
class file_reader {
public:
    /** 构造函数
    */
    file_reader(const ddstr& path);

    /** 析构函数
    */
    virtual ~file_reader();

    /** 打开文件
    @param [in] path 路径
    @param [in] checker 文件头检查
    @param [in] 文件头大小
    @return 是否成功
    */
    bool open(u8* checker, const u32 checkSize);

    /** 获得行(包含行末的\r\n)
    @param [out] line 输出
    @return 是否成功
    */
    bool getlinew(ddstrw& line);

    /** 获得行(包含行末的\n)
    @param [out] line 输出
    @return 是否成功
    */
    bool getlinea(ddstra& line);

    /** 读取文件知道
    @return 是否成功
    */
    void getuntila(std::function<bool(ddchara ch)> until);
    void getuntilw(std::function<bool(ddcharw ch)> until);

    /** 获得size个数据，buff需要外部分配内存
    @param [out] buff 输出数组
    @param [in] size 大小
    @return 实际获得的大小
    */
    u32 get_buff(u8* buff, const u32& size);

    /** 获得文件大小
    @return 文件大小
    */
    u64 get_file_size();

protected:
    FILE* m_pFile;
    ddstr m_path;
};

using sp_file_reader = std::shared_ptr<file_reader>;

/** 创建一个UCS-2 Little Endian 编码的文件 使用getlinew()
@return file_rw的shared_ptr
*/
inline sp_file_reader create_UCS2_file_reader(const ddstr& path)
{
    u8 header[] = {0xff, 0xfe};
    sp_file_reader spFileRW(new (std::nothrow) file_reader(path));

    if (spFileRW == nullptr || !spFileRW->open(header, 2)){
        return nullptr;
    }

    return spFileRW;
}

/** 创建一个UTF8 编码的文件 使用getlinea()
@return file_rw的shared_ptr
*/
inline sp_file_reader create_UTF8_file_reader(const ddstr& path)
{
    sp_file_reader spFileRW(new (std::nothrow) file_reader(path));

    if (spFileRW == nullptr || !spFileRW->open(nullptr, 0)) {
        return nullptr;
    }

    return spFileRW;
}

/** 创建一个UTF8 BOM 编码的文件 使用getlinea()
@return file_rw的shared_ptr
*/
inline sp_file_reader create_UTF8Bom_file_reader(const ddstr& path)
{
    u8 header[] = {0xef, 0xbb, 0xbf};
    sp_file_reader spFileRW(new (std::nothrow) file_reader(path));

    if (spFileRW == nullptr || !spFileRW->open(header, 3)) {
        return nullptr;
    }

    return spFileRW;
}

END_NSP_DDM
#endif // file_reader_h_
