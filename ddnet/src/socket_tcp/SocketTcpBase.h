
#ifndef __SOCKET_TCP_BASE_H_
#define __SOCKET_TCP_BASE_H_

#include "socket_common/SocketBase.h"
#include <functional>


class SocketTcpBase :
    public SocketBase
{
public:
    /** ���캯��
    */
    SocketTcpBase();

    /** ��������
    */
    virtual ~SocketTcpBase();

public:
    /** ������Ϣ ��msg���ڴ�Լ 1Kʱ�򷵻ص�ֵ���ܲ�����msg��ֵ��һ���Է��Ͳ��ˣ�
    @param [in] msg ϣ�����͵���Ϣ
    @return �ɹ����ط��͵ĸ�����ʧ�ܷ���SOCKET_ERROR
    */
    virtual int SendMsg(const DogStringA& msg);

    /** ������Ϣ������GetBuffSize()���ַ�����������С��������
    @param [out] msg �յ�����Ϣ
    @return �ɹ����ؽ��ܵĸ�����ʧ�ܷ���0
    */
    virtual int RcvMsg(DogStringA& msg);

    /** ������Ϣ��һ���Բ��ܹ�ȫ��������ȫ
    @param [out] callBack �ص�����
                 @param pBuff 
                 @param buffSize
                 @return ���Ϊfalseʱ����ֹ����
    */
    virtual void RcvMsg(std::function<bool(char* pBuff, unsigned int buffSize)> callBack);
};

#endif // __SOCKET_TCP_BASE_H_

