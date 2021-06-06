/*********************************************************************
// ��л�ο����ͣ�
// https://www.jianshu.com/p/4fabac6b15b3
// https://www.jianshu.com/p/62ad4f953660
**********************************************************************/

#ifndef __DOGGIF_H_
#define __DOGGIF_H_

#include "DogGifDef.h"
#include "StringTable.h"
#include <string>

namespace DogGifNSP
{

class DogGif
{
public:
    /** ���캯��
    */
    DogGif();

    /** ��������
    */
    ~DogGif();

    // ��������
    DogGif& operator=(const DogGif&) = delete;
    DogGif(const DogGif&) = delete;
    DogGif(DogGif&&) = delete;

public:
    /** ��ʼ��
    @note pBuff�������ڱ�֤��Init����ִ���ڼ���Ч����
    @param [in] pBuff ͼƬ�Ķ��������ݣ��ļ��ж�������ԭʼ���ݣ�
    @paran [in] buffLen pBuff����
    @return �Ƿ�ɹ�
    */
    bool Init(u8* pBuff, u32 buffLen);

    /** ������һ֡���̲߳���ȫ������ɹ��ڲ�֡index��һ
    @param [out] ppBuff �����ɫ���飬�ڴ治Ҫ�Լ��ͷ�
    @param [out] buffLen ���鳤��
    @reutrn �Ƿ�ɹ�
    */
    bool GetNextFrame(DogGifColor ** ppBuff, u32 & buffLen);

    /** ���ص�ǰ֡���̲߳���ȫ
    @param [out] ppBuff �����ɫ���飬�ڴ治Ҫ�Լ��ͷ�
    @param [out] buffLen ���鳤��
    @reutrn �Ƿ�ɹ�
    */
    bool GetCurrentFrame(DogGifColor ** ppBuff, u32 & buffLen);

    /** ���ȫ�ֿ��
    @return ȫ�ֿ��
    */
    u32 GetGolWidth();

    /** ���ȫ�ָ߶�
    @return ȫ�ָ߶�
    */
    u32 GetGolHeight();

    /** ���ȫ��ʱ������ȡ��0֡��
    @return ȫ��ʱ����
    */
    u32 GetGolTimeDelay();
    u32 GetCurTimeDelay();
    u32 GetTimeDelay(u32 frameIdx);

    /** �Ƿ��ʼ����
    @return �Ƿ��ʼ����
    */
    bool HasInit();

    /** һ���ж���֡
    @return ֡��Ŀ
    */
    u32 GetFrameCount();

    /** ��õ�ǰ�ڼ�֡
    @return �ڼ�֡
    */
    u32 GetCurFrameIndex();

private:
    /** ��gifͷ
    @pram [in, out] ppBuff head�Ŀ�ʼָ�룬��ȡ�����Զ�����ƶ���Ӧλ��
    @pram [in, out] buffLen ppBuff���ȣ���ȡ�����Զ����ٶ�ȡ�ĸ���
    @return �Ƿ�ɹ�
    */
    bool ReadHead(u8 ** ppBuff, u32& buffLen);

    /** ��LSD
    @pram [in, out] ppBuff head�Ŀ�ʼָ�룬��ȡ�����Զ�����ƶ���Ӧλ��
    @pram [in, out] buffLen ppBuff���ȣ���ȡ�����Զ����ٶ�ȡ�ĸ���
    @return �Ƿ�ɹ�
    */
    bool ReadLsd(u8 ** ppBuff, u32& buffLen);

    /** ��ȫ��ɫ��
    @pram [in, out] ppBuff head�Ŀ�ʼָ�룬��ȡ�����Զ�����ƶ���Ӧλ��
    @pram [in, out] buffLen ppBuff���ȣ���ȡ�����Զ����ٶ�ȡ�ĸ���
    @return �Ƿ�ɹ�
    */
    bool ReadGolColorTable(u8** ppBuff, u32& buffLen);

    /** ��һ֡
    @pram [in, out] ppBuff head�Ŀ�ʼָ�룬��ȡ�����Զ�����ƶ���Ӧλ��
    @pram [in, out] buffLen ppBuff���ȣ���ȡ�����Զ����ٶ�ȡ�ĸ���
    @return �ɹ�����DogGifFrame���󣬷���NULL
    */
    DogGifFrame* ReadFrameData(u8** ppBuff, u32& buffLen);

private:
    /** ����m_curFrame������֡��
    @note Ҫ��m_frameBuff�洢������һ֡���������ö�û�иı�m_curFrame����ô���п��ܳ���
    @return �Ƿ�ɹ�
    */
    bool DecodeFrame();

private:
    /** ��ǰ֡��ͼƬbuff����DecodeFrame�����ڱ�ʾ��һ֡
    */
    std::vector<DogGifColor> m_frameBuff;

    /** ��ǰ֡������
    */
    u32 m_curFrame;

    /** ȫ��gif��Ϣ
    */
    DogGifGolInfo m_gifGolInfo;

    /** �ӽ���֡��
    */
    StringTable* m_pStringTable;

    /** �Ƿ��Ѿ���ʼ����
    */
    bool m_hasInit;
};

}
#endif // __DOGGIF_H_

