
#ifndef i_ddstream_h_
#define i_ddstream_h_ 1

#include "g_def.h"
#include <stdio.h>

BEG_NSP_DDM

/** ��� (n * b) �� a <= (n * b) <= a + b
@param [in] a ���ܳ���0xffffffff - (b - 1)���򷵻�С��a
@param [in] b
@return b�������ڵ���a����С�Ͻ磬���С��a��˵��ʧ��
*/
inline u32 get_min_upper(u64 a, u64 b)
{
    --b;
    return (u32)((a + b) & (~b));
}

/** ���ݿ��С��λ 1 K
*/
#define MET_DATA_SIZE 0x400

/** ���Ļ���
*/
class i_ddstream
{
public:
    /** ��������
    */
    virtual ~i_ddstream() { };

    /** ��õ�ǰλ�� seek
    @return ��ǰλ�� 
    */
    virtual s64 pos() = 0;

    /** ���õ�ǰλ��
    @param [int] offset ƫ��
    @param [int] origin ȡ����ֵSEEK_CUR SEEK_END SEEK_SET
    @return ʵ�����õ�λ��
    */
    virtual s64 seek(s64 offset, int origin) = 0;

    /** ������Ĵ�С
    @return ���Ĵ�С
    */
    virtual s64 size()
    {
        s64 curPos = pos();
        s64 end = seek(0, SEEK_END);
        seek(curPos, SEEK_SET);
        return end;
    }

    /** �������Ĵ�С
    @param [in] newSize ���Ĵ�С
    @return ʵ�ʷ���Ĵ�С
    */
    virtual s64 resize(s64 newSize) = 0;

    /** ��pos��ʼ��ȡ��,����ȡ���ڴ�����˲��ܳ���2^31λ (��ı�pos)
    @param [out] buff ���buff���ڴ���Ҫ�Լ�����
    @param [in] count ��ȡ�Ĵ�С
    @return ��ȡ��ʵ�ʴ�С
    */
    virtual s32 read(u8* const buff, s32 count) = 0;

    /** ��pos��ʼд������һ����д�벻�ܳ���32λ(��ı�pos)
    @param [in] buff д��buff���ڴ���Ҫ�Լ�����
    @param [in] count д��Ĵ�С
    @return д���ʵ�ʴ�С
    */
    virtual s32 write(const u8* const buff, s32 count) = 0;

    /** ��ȡ�� (��ı�pos)
    @param [out] buff ���buff���ڴ���Ҫ�Լ�����
    @param [in] count ��ȡ�Ĵ�С
    @return ��ȡ��ʵ�ʴ�С
    */
    virtual s32 readw(u16* const buff, s32 count)
    {
        return read((u8*)buff, count * 2) / 2;
    }

    /** д���� (��ı�pos)
    @param [in] buff д��buff���ڴ���Ҫ�Լ�����
    @param [in] count д��Ĵ�С
    @return д���ʵ�ʴ�С
    */
    virtual s32 writew(const u16* const buff, s32 count)
    {
        return write((const u8*)buff, count * 2) / 2;
    }

    /** ��ȡȫ��������ȡ���ڴ�����˲��ܳ���2^31λ�����ʵ�ʴ�С����2^31λ����ô�˺���ֻ��ȡǰ2^31λ (��ı�pos)
    @param [out] buff ���buff���ڴ���Ҫ�Լ�����
    @return ��ȡ��ʵ�ʴ�С
    */
    virtual s32 readalla(u8* const buff)
    {
        seek(0, SEEK_SET);
        return read(buff, (s32)size());
    }

    /** ��ȡȫ�� (��ı�pos)
    @param [out] buff ���buff���ڴ���Ҫ�Լ�����
    @return ��ȡ��ʵ�ʴ�С
    */
    virtual s32 readallw(u16* const buff)
    {
        return readalla((u8*)buff) / 2;
    }
};

END_NSP_DDM
#endif // i_ddstream_h_
