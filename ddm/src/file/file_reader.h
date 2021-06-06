
#ifndef file_reader_h_
#define file_reader_h_ 1

#include "g_def.h"
#include <functional>
#include <memory>
#include <string>

BEG_NSP_DDM

/** �ļ���д�࣬��Ҫ�ļ����ڣ�����ļ���СΪ0 open ������ȥдУ��ͷ������У��У��ͷ
*/
class file_reader {
public:
    /** ���캯��
    */
    file_reader(const ddstr& path);

    /** ��������
    */
    virtual ~file_reader();

    /** ���ļ�
    @param [in] path ·��
    @param [in] checker �ļ�ͷ���
    @param [in] �ļ�ͷ��С
    @return �Ƿ�ɹ�
    */
    bool open(u8* checker, const u32 checkSize);

    /** �����(������ĩ��\r\n)
    @param [out] line ���
    @return �Ƿ�ɹ�
    */
    bool getlinew(ddstrw& line);

    /** �����(������ĩ��\n)
    @param [out] line ���
    @return �Ƿ�ɹ�
    */
    bool getlinea(ddstra& line);

    /** ��ȡ�ļ�֪��
    @return �Ƿ�ɹ�
    */
    void getuntila(std::function<bool(ddchara ch)> until);
    void getuntilw(std::function<bool(ddcharw ch)> until);

    /** ���size�����ݣ�buff��Ҫ�ⲿ�����ڴ�
    @param [out] buff �������
    @param [in] size ��С
    @return ʵ�ʻ�õĴ�С
    */
    u32 get_buff(u8* buff, const u32& size);

    /** ����ļ���С
    @return �ļ���С
    */
    u64 get_file_size();

protected:
    FILE* m_pFile;
    ddstr m_path;
};

using sp_file_reader = std::shared_ptr<file_reader>;

/** ����һ��UCS-2 Little Endian ������ļ� ʹ��getlinew()
@return file_rw��shared_ptr
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

/** ����һ��UTF8 ������ļ� ʹ��getlinea()
@return file_rw��shared_ptr
*/
inline sp_file_reader create_UTF8_file_reader(const ddstr& path)
{
    sp_file_reader spFileRW(new (std::nothrow) file_reader(path));

    if (spFileRW == nullptr || !spFileRW->open(nullptr, 0)) {
        return nullptr;
    }

    return spFileRW;
}

/** ����һ��UTF8 BOM ������ļ� ʹ��getlinea()
@return file_rw��shared_ptr
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
