
#ifndef __MEMORYSTREAM_H_
#define __MEMORYSTREAM_H_

#include "i_ddstream.h"
BEG_NSP_DDM

/** �ڴ���������ڴ棺min(4G, ϵͳ����)
*/
class memory_stream :
    public i_ddstream
{
public:
    /** ���캯��
    */
    memory_stream();

    /** ���캯��
    @param [in] size ��С
    */
    memory_stream(u32 size);

    /** ���캯��
    @param [in] pBuff �ڴ�
    @param [in] size ��С
    */
    memory_stream(const s8* const pBuff, u32 size);

    /** ���캯��
    @param [in] stream ��
    */
    memory_stream(memory_stream&& stream);

    /** ���캯��
    @param [in] stream ��
    */
    memory_stream(const i_ddstream& stream);

    /** ���캯��
    @param [in] stream ��
    */
    memory_stream(const memory_stream& stream);

    /** ���������
    @param [in] stream ��
    */
    memory_stream& operator=(const i_ddstream& stream);

    /** ���������
    @param [in] stream ��
    */
    memory_stream& operator=(const memory_stream& stream);

    /** ��������
    */
    virtual ~memory_stream();

    /** ��õ�ǰλ�� seek
    @return ��ǰλ��
    */
    virtual s64 pos() override;

    /** ���õ�ǰλ��
    @param [int] offset ƫ��
    @param [int] origin ȡ����ֵSEEK_CUR SEEK_END SEEK_SET
    @return ʵ�����õ�λ��
    */
    virtual s64 seek(s64 offset, int origin) override;

    /** ������Ĵ�С
    @return ���Ĵ�С
    */
    virtual s64 size() override;

    /** �������Ĵ�С���ڴ治��ʱ����Զ����䣬���ǲ����ʼ���ڴ�
    @param [in] newSize ���Ĵ�С
    @return ʵ�ʷ���Ĵ�С
    */
    virtual s64 resize(s64 newSize) override;

    /** ���ʵ��buff��С��������GetSize
    @return ��ʹ�õ��ڴ�
    */
    u32 capacity();

    /** �������
    */
    void clear();

    /** ��pos��ʼ��ȡ�� (��ı�pos)
    @param [out] buff ���buff���ڴ���Ҫ�Լ�����
    @param [in] count ��ȡ�Ĵ�С
    @return ��ȡ��ʵ�ʴ�С
    */
    virtual s32 read(u8* const buff, s32 count) override;

    /** ��pos��ʼд���� (��ı�pos)
    @param [in] buff д��buff���ڴ���Ҫ�Լ�����
    @param [in] count д��Ĵ�С
    @return д���ʵ�ʴ�С
    */
    virtual s32 write(const u8* const buff, s32 count) override;

protected:
    /** ���·����ڴ�(��ȻnewSize��64λ�ģ�����������������ڴ治�ᳬ��32λҲ����4G)
    @param [in] newSize �µĴ�С
    @return ʵ�ʷ���Ĵ�С
    */
    u32 realloc_memory(u32 newSize);

    /** ���¼��㼴��������ڴ��С
    @param [in] newSize �û�����
    @return �����Ĵ�С
    */
    u32 cal_new_size(u32 newSize);

protected:
    /** ��ǰλ��
    */
    u32 m_pos = 0;

    /** ʵ��ʹ�õĴ�С
    */
    u32 m_size = 0;

    /** �ܴ�С
    */
    u32 m_capacity = 0;

    /** ����
    */
    s8* m_pBuff = nullptr;
};

END_NSP_DDM
#endif //__MEMORYSTREAM_H_
