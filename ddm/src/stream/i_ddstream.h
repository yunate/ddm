
#ifndef i_ddstream_h_
#define i_ddstream_h_ 1

#include "g_def.h"
#include <stdio.h>

BEG_NSP_DDM

/** 获得 (n * b) 有 a <= (n * b) <= a + b
@param [in] a 不能超过0xffffffff - (b - 1)否则返回小于a
@param [in] b
@return b倍数大于等于a的最小上界，如果小于a，说明失败
*/
inline u32 get_min_upper(u64 a, u64 b)
{
    --b;
    return (u32)((a + b) & (~b));
}

/** 数据块大小单位 1 K
*/
#define MET_DATA_SIZE 0x400

/** 流的基类
*/
class i_ddstream
{
public:
    /** 析构函数
    */
    virtual ~i_ddstream() { };

    /** 获得当前位置 seek
    @return 当前位置 
    */
    virtual s64 pos() = 0;

    /** 设置当前位置
    @param [int] offset 偏移
    @param [int] origin 取如下值SEEK_CUR SEEK_END SEEK_SET
    @return 实际设置的位置
    */
    virtual s64 seek(s64 offset, int origin) = 0;

    /** 获得流的大小
    @return 流的大小
    */
    virtual s64 size()
    {
        s64 curPos = pos();
        s64 end = seek(0, SEEK_END);
        seek(curPos, SEEK_SET);
        return end;
    }

    /** 设置流的大小
    @param [in] newSize 流的大小
    @return 实际分配的大小
    */
    virtual s64 resize(s64 newSize) = 0;

    /** 从pos开始读取流,由于取到内存中因此不能超过2^31位 (会改变pos)
    @param [out] buff 输出buff，内存需要自己管理
    @param [in] count 读取的大小
    @return 读取的实际大小
    */
    virtual s32 read(u8* const buff, s32 count) = 0;

    /** 从pos开始写入流，一次性写入不能超过32位(会改变pos)
    @param [in] buff 写入buff，内存需要自己管理
    @param [in] count 写入的大小
    @return 写入的实际大小
    */
    virtual s32 write(const u8* const buff, s32 count) = 0;

    /** 读取流 (会改变pos)
    @param [out] buff 输出buff，内存需要自己管理
    @param [in] count 读取的大小
    @return 读取的实际大小
    */
    virtual s32 readw(u16* const buff, s32 count)
    {
        return read((u8*)buff, count * 2) / 2;
    }

    /** 写入流 (会改变pos)
    @param [in] buff 写入buff，内存需要自己管理
    @param [in] count 写入的大小
    @return 写入的实际大小
    */
    virtual s32 writew(const u16* const buff, s32 count)
    {
        return write((const u8*)buff, count * 2) / 2;
    }

    /** 读取全部，由于取到内存中因此不能超过2^31位，如果实际大小超过2^31位，那么此函数只读取前2^31位 (会改变pos)
    @param [out] buff 输出buff，内存需要自己管理
    @return 读取的实际大小
    */
    virtual s32 readalla(u8* const buff)
    {
        seek(0, SEEK_SET);
        return read(buff, (s32)size());
    }

    /** 读取全部 (会改变pos)
    @param [out] buff 输出buff，内存需要自己管理
    @return 读取的实际大小
    */
    virtual s32 readallw(u16* const buff)
    {
        return readalla((u8*)buff) / 2;
    }
};

END_NSP_DDM
#endif // i_ddstream_h_
