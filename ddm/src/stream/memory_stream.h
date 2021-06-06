
#ifndef __MEMORYSTREAM_H_
#define __MEMORYSTREAM_H_

#include "i_ddstream.h"
BEG_NSP_DDM

/** 内存流，最大内存：min(4G, 系统定义)
*/
class memory_stream :
    public i_ddstream
{
public:
    /** 构造函数
    */
    memory_stream();

    /** 构造函数
    @param [in] size 大小
    */
    memory_stream(u32 size);

    /** 构造函数
    @param [in] pBuff 内存
    @param [in] size 大小
    */
    memory_stream(const s8* const pBuff, u32 size);

    /** 构造函数
    @param [in] stream 流
    */
    memory_stream(memory_stream&& stream);

    /** 构造函数
    @param [in] stream 流
    */
    memory_stream(const i_ddstream& stream);

    /** 构造函数
    @param [in] stream 流
    */
    memory_stream(const memory_stream& stream);

    /** 复制运算符
    @param [in] stream 流
    */
    memory_stream& operator=(const i_ddstream& stream);

    /** 复制运算符
    @param [in] stream 流
    */
    memory_stream& operator=(const memory_stream& stream);

    /** 析构函数
    */
    virtual ~memory_stream();

    /** 获得当前位置 seek
    @return 当前位置
    */
    virtual s64 pos() override;

    /** 设置当前位置
    @param [int] offset 偏移
    @param [int] origin 取如下值SEEK_CUR SEEK_END SEEK_SET
    @return 实际设置的位置
    */
    virtual s64 seek(s64 offset, int origin) override;

    /** 获得流的大小
    @return 流的大小
    */
    virtual s64 size() override;

    /** 设置流的大小，内存不够时候会自动分配，但是不会初始化内存
    @param [in] newSize 流的大小
    @return 实际分配的大小
    */
    virtual s64 resize(s64 newSize) override;

    /** 获得实际buff大小，区别于GetSize
    @return 所使用的内存
    */
    u32 capacity();

    /** 清空数据
    */
    void clear();

    /** 从pos开始读取流 (会改变pos)
    @param [out] buff 输出buff，内存需要自己管理
    @param [in] count 读取的大小
    @return 读取的实际大小
    */
    virtual s32 read(u8* const buff, s32 count) override;

    /** 从pos开始写入流 (会改变pos)
    @param [in] buff 写入buff，内存需要自己管理
    @param [in] count 写入的大小
    @return 写入的实际大小
    */
    virtual s32 write(const u8* const buff, s32 count) override;

protected:
    /** 重新分配内存(虽然newSize是64位的，但是这个函数分配内存不会超过32位也就是4G)
    @param [in] newSize 新的大小
    @return 实际分配的大小
    */
    u32 realloc_memory(u32 newSize);

    /** 重新计算即将分配的内存大小
    @param [in] newSize 用户输入
    @return 计算后的大小
    */
    u32 cal_new_size(u32 newSize);

protected:
    /** 当前位置
    */
    u32 m_pos = 0;

    /** 实际使用的大小
    */
    u32 m_size = 0;

    /** 总大小
    */
    u32 m_capacity = 0;

    /** 缓存
    */
    s8* m_pBuff = nullptr;
};

END_NSP_DDM
#endif //__MEMORYSTREAM_H_
