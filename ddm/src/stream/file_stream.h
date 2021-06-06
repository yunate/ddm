
#ifndef file_stream_h_
#define file_stream_h_ 1

#include "i_ddstream.h"

BEG_NSP_DDM
class file_stream :
    public i_ddstream
{
public:
    /** ���캯��
    @param [in] path ·��
    @param [in] mode �򿪷�ʽ,mode == nullptr ʱ��Ĭ��Ϊ"a+b"
    */
    file_stream(const ddchar* const path, const ddchar* const mode = nullptr);

    /** �ƶ����칹�캯��
    @param [in] stream ��
    */
    file_stream(file_stream&& stream) noexcept;

    /** ��������
    */
    virtual ~file_stream();

    /** ��������
    */
    file_stream(const file_stream& stream) = delete;

    /** ��������
    */
    file_stream& operator=(const file_stream& stream) = delete;

public:
    /** ��õ�ǰλ�� seek
    @return ��ǰλ��,-1��ʾʧ�ܣ��μ�ftell
    */
    virtual s64 pos() override;

    /** ���õ�ǰλ��
    @param [int] offset ƫ��
    @param [int] origin ȡ����ֵSEEK_CUR SEEK_END SEEK_SET
    @return ʵ�����õ�λ��
    */
    virtual s64 seek(s64 offset, int origin) override;

    /** �������Ĵ�С
    @param [in] newSize ���Ĵ�С
    @return ʵ�ʷ���Ĵ�С
    */
    virtual s64 resize(s64 newSize) override;

    /** ��pos��ʼ��ȡ��,����ȡ���ڴ�����˲��ܳ���2^31λ (��ı�pos)
    @param [out] buff ���buff���ڴ���Ҫ�Լ�����
    @param [in] count ��ȡ�Ĵ�С
    @return ��ȡ��ʵ�ʴ�С
    */
    virtual s32 read(u8* const buff, s32 count) override;

    /** ��pos��ʼд������һ����д�벻�ܳ���32λ(��ı�pos)
    @param [in] buff д��buff���ڴ���Ҫ�Լ�����
    @param [in] count д��Ĵ�С
    @return д���ʵ�ʴ�С
    */
    virtual s32 write(const u8* const buff, s32 count) override;

protected:
    /** �ļ����
    */
    FILE* m_hFile;

    /** ·��
    */
    ddstr m_path;
};

END_NSP_DDM
#endif // file_stream_h_
