
#ifndef file_writer_h_
#define file_writer_h_ 1

#include "g_def.h"
#include "dir_utils.h"
#include <functional>
#include <memory>
#include <string>

BEG_NSP_DDM

/** �ļ���д�࣬��Ҫ�ļ����ڣ�����ļ���СΪ0 open ������ȥдУ��ͷ������У��У��ͷ
*/
class file_writer {
public:
    /** ���캯��
    */
    file_writer(const ddstr& path);

    /** ��������
    */
    virtual ~file_writer();

    /** ���ļ�
    @param [in] path ·��
    @param [in] checker �ļ�ͷ���
    @param [in] �ļ�ͷ��С
    @return �Ƿ�ɹ�
    */
    bool open(u8* checker, const u32 checkSize);

    /** д wchar
    @param [in] buff ��������
    @param [in] size �����С
    @return д����� ��СΪ 2 * size
    */
    u32 write_buffw(const ddcharw* buff, const u32 size);

    /** дchar
    @param [out] line ���
    @return д�����
    */
    u32 write_buffa(const ddchara* buff, const u32 size);

    /** ����ļ���С
    @return �ļ���С
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

/** ����һ��UCS-2 Little Endian ������ļ� ʹ��getlinew()
@return file_rw��shared_ptr
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

/** ����һ��UTF8 ������ļ� ʹ��getlinea()
@return file_rw��shared_ptr
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

/** ����һ��UTF8 BOM ������ļ� ʹ��getlinea()
@return file_rw��shared_ptr
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
