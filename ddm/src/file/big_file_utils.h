
#ifndef big_file_utils_h_
#define big_file_utils_h_ 1

#include "nocopyable.hpp"
#include "g_def.h"
#include <memory>

BEG_NSP_DDM

/** Ĭ���ļ����С10M
*/
static const s32 g_file_block_default_size = 1024 * 1024 * 10;

class big_file_mapper;

/** һ�����ļ����ļ���
*/
class file_block
{
    DD_NO_COPY_MOVE(file_block);
public:
    file_block() = default;

    /** ��������
    */
    ~file_block();

    /** ����ļ����С
    @return �ļ���Ĵ�С
    */
    inline s32 get_size()
    {
        return m_blockSize;
    }

    /** ����������ļ�����ʼ��ַ
    @return �ļ������ʼ��ַ
    */
    inline char* get_block_addr()
    {
        return m_pBlock + m_offSet;
    }

private:
    /** �ļ�����ʼ��ַ
    */
    char* m_pBlock = nullptr;

    /** �����ļ�ӳ��Ҫ��64k���룬���������offset���������Ŀ�ʼ
    */
    s32 m_offSet = 0;

    /** �ļ����С
    */
    s32 m_blockSize = 0;

    friend big_file_mapper;
};

using sp_file_block = std::shared_ptr<file_block>;

/** �ڴ�ӳ���ļ���������ļ����ɶ�д���������ļ���С�ı��˵Ļ���Ҫ����ӳ��
*/
class big_file_mapper
{
    DD_NO_COPY_MOVE(big_file_mapper);
public:
    /** ���캯��
    */
    big_file_mapper();

    /** ��������
    */
    ~big_file_mapper();

public:
    /** ��ʼ�����ظ����û������һ�ε�����
    @param [in] filePath �ļ�·��
    @return �Ƿ�ɹ�
    */
    bool map_file(const ddstr& filePath);

    /** ����ļ���С
    @return �ļ���С
    */
    inline s64 get_file_size()
    {
        return m_fileSize;
    }

    /** ����ļ������һ��
    @param [in] size ָ����С��B�������С�ڵ���0���ߴ���1G�Ļ�ʹ��Ĭ�ϴ�С�������ĩβ���루align != 0�����ص�block��С����ҪС��size
    @param [in] align size ָ���Ĵ�С���ܲ���һ�����ӵ�β�ͣ����Ի���ǰ�ҵ���һ������align�����������ַ���,align == 0 ˵�������룬Ĭ��"\n"
    @return �ļ����share_ptr�����Ϊ��˵��map���ļ�β����
    */
    sp_file_block get_next_block(s32 sizeRaw = g_file_block_default_size, char align = '\n');

    /** ����ļ���
    @param [in] beginPos ��ʼλ��
    @param [in] size ָ����С��B�������С�ڵ���0���ߴ���1G�Ļ�ʹ��Ĭ�ϴ�С�������ĩβ���루align != 0�����ص�block��С����ҪС��size
    @param [in] align size ָ���Ĵ�С���ܲ���һ�����ӵ�β�ͣ����Ի���ǰ�ҵ���һ������align�����������ַ���,align == 0 ˵�������룬Ĭ��"\n"
    @return �ļ����share_ptr�����Ϊ��˵��map���ļ�β����
    */
    sp_file_block get_block(s64 beginPos, s32 sizeRaw = g_file_block_default_size, char align = '\n');

private:
    /** �ļ�ӳ���ڴ�ľ��
    */
    void* m_hFileMap;

    /** ӳ���λ��
    */
    s64 m_bginPos;

    /** �ļ�С
    */
    s64 m_fileSize;
};

END_NSP_DDM
#endif // big_file_utils_h_
