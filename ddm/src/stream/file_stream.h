
#ifndef file_stream_h_
#define file_stream_h_ 1

#include "i_ddstream.h"

BEG_NSP_DDM
class file_stream :
    public i_ddstream
{
public:
    /** 构造函数
    @param [in] path 路径
    @param [in] mode 打开方式,mode == nullptr 时候，默认为"a+b"
    */
    file_stream(const ddchar* const path, const ddchar* const mode = nullptr);

    /** 移动构造构造函数
    @param [in] stream 流
    */
    file_stream(file_stream&& stream) noexcept;

    /** 析构函数
    */
    virtual ~file_stream();

    /** 不允许复制
    */
    file_stream(const file_stream& stream) = delete;

    /** 不允许复制
    */
    file_stream& operator=(const file_stream& stream) = delete;

public:
    /** 获得当前位置 seek
    @return 当前位置,-1表示失败，参见ftell
    */
    virtual s64 pos() override;

    /** 设置当前位置
    @param [int] offset 偏移
    @param [int] origin 取如下值SEEK_CUR SEEK_END SEEK_SET
    @return 实际设置的位置
    */
    virtual s64 seek(s64 offset, int origin) override;

    /** 设置流的大小
    @param [in] newSize 流的大小
    @return 实际分配的大小
    */
    virtual s64 resize(s64 newSize) override;

    /** 从pos开始读取流,由于取到内存中因此不能超过2^31位 (会改变pos)
    @param [out] buff 输出buff，内存需要自己管理
    @param [in] count 读取的大小
    @return 读取的实际大小
    */
    virtual s32 read(u8* const buff, s32 count) override;

    /** 从pos开始写入流，一次性写入不能超过32位(会改变pos)
    @param [in] buff 写入buff，内存需要自己管理
    @param [in] count 写入的大小
    @return 写入的实际大小
    */
    virtual s32 write(const u8* const buff, s32 count) override;

protected:
    /** 文件句柄
    */
    FILE* m_hFile;

    /** 路径
    */
    ddstr m_path;
};

END_NSP_DDM
#endif // file_stream_h_
