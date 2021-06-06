
#ifndef __DOGGIF_DEF_H_
#define __DOGGIF_DEF_H_

#include <vector>
#include <string>

namespace DogGifNSP
{
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned long;
#pragma pack(push, 1)

/** ����ͷ
47 49 46 38 39 61
*/
struct GifHead
{
    /** �ļ�����
    */
    u8 signature[3];

    /** �ļ��汾
    */
    u8 version[3];

    bool IsGifFile()
    {
        if (signature[0] != 'G' ||
            signature[1] != 'I' ||
            signature[2] != 'F' ||
            version[0] != '8' ||
            (version[1] != '7' && version[1] != '9') ||
            version[2] != 'a')
        {
            return false;
        }

        return true;
    }
};

/** �߼���Ļ��ʶ��(Logical Screen Descriptor)
f0 00 b4 00 f7 8b 00
*/
struct GifLogicalScreenDescriptor
{
    /** ��ȣ����أ�
    00f0
    */
    u16 m_width;

    /** �߶ȣ����أ�
    00b4
    */
    u16 m_height;

    /** ��ɫ����
       f7 1 111 0 111
        m 7 - ȫ����ɫ�б��־(Global Color Table Flag)������λʱ��ʾ��ȫ����ɫ�б�pixelֵ������.
        cr 6 5 4 - ��ɫ���(Color ResoluTion)��cr+1ȷ��ͼ�����ɫ���.
        s 3 - �����־(Sort Flag)��0 �� Global Color Table ����������Ϊ 1 ���ʾ Global Color Table ���ս������У�����Ƶ��������ɫ������ǰ��.
        pixel 2 1 0 - ȫ����ɫ�б��С��pixel+1ȷ����ɫ�б����������2��pixel+1�η�)
    */
    u8 m_colorDes;

    /** ����ɫ
    8b
        Ϊ������ɫָ��ȫ��ɫ��������ɫ��ָ��Щû�б�ͼ�񸲸ǵ��������ֵ���ɫ����ȫ��ɫ���־λ��Ϊ0������ֶ�Ҳ����ֵ0�����ұ����ԡ�
    */
    u8 m_bgColor;

    /** ���ؿ�߱�
    00
        ������ֶε�ֵΪ��0�������صĸ߿��������Ĺ�ʽ���㣺
        �߿�� = (���ظ߿�� + 15) / 64
        ���ֶε�ȡֵ��Χ�����ı�ֵ4��1����ߵı�ֵ1��4�������Ĳ���Ϊ1/64��ȡֵ�� 0 - û�б�ֵ��1��255 - ���ڼ����ֵ��
    */
    u8 m_pixelTo;
};

/** ��չ��
*/
struct ExtendBlock
{
    /** 0x21 �������һ����չ��
    */
    u8 m_flag;

    /** 0xf9 �������һ��ͼ����չ��
    */
    u8 m_controlLabel;

    /** ���С �̶�4
    */
    u8 m_blockSize;

    /** �û�������
        �������һ��������λ��ʾReserved for Future Use��������λ�������ô���
        ��������ģ��壬��λ��ʾ Disposal Method����ʾ�ڽ�����֡��Ⱦʱ��ǰһ֡���µ�ͼ�����δ���0�������κδ���1������ǰһ֡ͼ���ڴ˻����Ͻ�����Ⱦ��2����Ⱦǰ��ͼ����Ϊ����ɫ��3��������һ֡���ǵ�ͼ�����á�
        �������ڶ�λ��ʾ User Input Flag����ʾ�Ƿ���Ҫ�ڵõ��û�������ʱ�Ž�����һ֡�����루�����û�����ָʲô��Ӧ�ö�������0 ��ʾ�����û����롣1 ��ʾ��Ҫ�û����롣
        ���ұ�һλ����ʾ Transparent Flag������ֵΪ 1 ʱ������� Transparent Color Index ָ������ɫ��������͸��ɫ����Ϊ 0 ��������
    */
    u8 m_userFlag;

    /** ����ʱ��
    */
    u16 m_delayTime;

    /** ͸��ɫ����
    */
    u8 m_TranColorIndex;

    /** �ս������ �̶�Ϊ0
    */
    u8 m_endFlag;
};

/** ͼ���ʶ��
    ��������һ������ͼ��ı�ı�Ҫ����������һ��ͼ������ģ�ͼ���ʶ����0x2C(',')�ַ���ʼ��
    һ��ͼ���Ӧһ��ͼ���ʶ����ͼ���ʶ��������Ŷ�Ӧ��ͼ�����ݡ�
*/
struct ImageDescriptor
{
    /** ͼ��Ŀ�ʼ���̶�Ϊ ','
    */
    u8 m_start;

    /** x����ƫ����
    */
    u16 m_left;

    /** y����ƫ����
    */
    u16 m_top;

    /** ͼ����
    */
    u16 m_width;

    /** ͼ��߶�
    */
    u16 m_height;

    /** �ֲ���ɫ�б���
        m 7 - �ֲ���ɫ�б��־(Local Color Table Flag) ��λʱ��ʶ������ͼ���ʶ��֮����һ���ֲ���ɫ�б�����������֮���һ��ͼ��ʹ�ã�ֵ��ʱʹ��ȫ����ɫ�б�����pixelֵ��
        i 6 - ��֯��־(Interlace Flag)��ʾ�Ƿ���Ҫ����ɨ�衣1 Ϊ��Ҫ��0 Ϊ����Ҫ��
        s 5 - �����־(Sort Flag)�������λ��ʾ�����ŵľֲ���ɫ�б��������.
        r 4 3 - �����������ʼ��Ϊ0.
        pixel 2 1 0 - �ֲ���ɫ�б��С(Size of Local Color Table)��pixel+1��Ϊ��ɫ�б��λ��
    */
    u8 m_localColorFlag;
};

/** ��ɫ��
*/
struct DogGifColor
{
    DogGifColor(u8 r, u8 g, u8 b, u8 a) :
        m_r(r),
        m_g(g),
        m_b(b),
        m_a(a)
    {

    }

    DogGifColor()
    {

    }

    /** blue
    */
    u8 m_b = 0;

    /** green
    */
    u8 m_g = 0;

    /** red
    */
    u8 m_r = 0;

    /** a
    */
    u8 m_a = 0xff;
};

/** ÿһ֡��Ϣ
*/
struct DogGifFrame
{
    /** 0�������κδ���1������ǰһ֡ͼ���ڴ˻����Ͻ�����Ⱦ��2����Ⱦǰ��ͼ����Ϊ����ɫ��3��������һ֡���ǵ�ͼ�����á�
    */
    u8 m_disposalMethod;

    /** �û�������
    */
    u8 m_userInputFlag = 0;

    /** ����ֵΪ 1 ʱ������� m_tranColorIndex ָ������ɫ��������͸��ɫ����Ϊ 0 ��������
    */
    u8 m_tranFlag;

    /** ����ʱ��
    */
    u16 m_delayTime = 0;

    /** ͸��ɫ����
        ��λ��ʾʹ��͸����ɫ
    */
    u8 m_tranColorIndex = 0;

    /** x����ƫ����
    */
    u16 m_left = 0;

    /** y����ƫ����
    */
    u16 m_top = 0;

    /** ͼ����
    */
    u16 m_width = 0;

    /** ͼ��߶�
    */
    u16 m_height = 0;

    /** �Ƿ��оֲ���ɫ��
    */
    u8 m_hasLocalColorTable = 0;

    /** ��֯��־(Interlace Flag)����λʱͼ������ʹ�ý�֯��ʽ���У�����ʹ��˳�����С�
    */
    u8 m_interlaceFlag = 0;

    /** �����־(Sort Flag)�������λ��ʾ�����ŵľֲ���ɫ�б��������.
    */
    u8 m_sortFlag = 0;

    /** �ֲ���ɫ�б���ɫ����
    */
    u16 m_LocalColorTableBit = 0;

    /** �ֲ�ɫ��
    */
    std::vector<DogGifColor> m_localColorTable;

    /** ���볤��
    */
    u8 m_codeLen = 0;

    /** frame����
    */
    std::vector<u8> m_frameData;
};

/** gifȫ����Ϣ
*/
struct DogGifGolInfo
{
    /** gif��ʶ
        GIF89a ���� GIF87a
    */
    std::string m_gifHeadSignaturl = "";

    /** ���
    */
    u16 m_width = 0;

    /** �߶ȶ�
    */
    u16 m_height = 0;

    /** �Ƿ���ȫ����ɫ��
    */
    u8 m_hasGolColorTable = 0;

    /** ��ɫ���
    */
    u8 m_colorDepth = 0;

    /** ȫ����ɫ�б��Ƿ��������
    */
    u8 m_isGolColorTableSorted = 0;

    /** ȫ����ɫ�б���ɫ����
    */
    u16 m_golColorTableBit = 0;

    /** ����ɫλ��
    */
    u8 m_bgColorIndex;

    /** ���صĸ߿��
        ������ֶε�ֵΪ��0�������صĸ߿��������Ĺ�ʽ���㣺
        �߿�� = (���ظ߿�� + 15) / 64
        ���ֶε�ȡֵ��Χ�����ı�ֵ4��1����ߵı�ֵ1��4�������Ĳ���Ϊ1/64��ȡֵ�� 0 - û�б�ֵ��1��255 - ���ڼ����ֵ��
    */
    u8 m_pixelToWidthHeight = 0;

    /** ȫ��ɫ��
    */
    std::vector<DogGifColor> m_golColorTable;

    /** ÿһ֡����
    */
    std::vector<DogGifFrame*> m_frameData;

    ~DogGifGolInfo()
    {
        for (auto it : m_frameData)
        {
            delete it;
        }

        m_golColorTable.clear();
    }
};

#pragma pack(pop)
}

#endif // __DOGGIF_DEF_H_

