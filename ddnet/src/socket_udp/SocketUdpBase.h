
#ifndef __SOCKET_UDP_BASE_H_
#define __SOCKET_UDP_BASE_H_

#include "socket_common/SocketBase.h"
#include <functional>

class SocketUdpBase :
    public SocketBase
{
public:
    /** ���캯��
    */
    SocketUdpBase();

    /** ��������
    */
    virtual ~SocketUdpBase();

public:
    /** ������Ϣ ��msg���ڴ�Լ 1Kʱ�򷵻ص�ֵ���ܲ�����msg��ֵ��һ���Է��Ͳ��ˣ�
    @param [in] ip Ŀ��ip
    @param [in] port Ŀ��port
    @param [in] msg ϣ�����͵���Ϣ
    @return �ɹ����ط��͵ĸ�����ʧ�ܷ���SOCKET_ERROR
    */
    virtual int SendMsg(const DogStringA& ip, unsigned short port, const DogStringA& msg);

    /** ������Ϣ ��msg���ڴ�Լ 1Kʱ�򷵻ص�ֵ���ܲ�����msg��ֵ��һ���Է��Ͳ��ˣ�
    @param [in] sockAddr Ŀ��sockAddr
    @param [in] msg ϣ�����͵���Ϣ
    @return �ɹ����ط��͵ĸ�����ʧ�ܷ���SOCKET_ERROR
    */
    virtual int SendMsg(const SOCKADDR& sockAddr, const DogStringA& msg);

    /** ������Ϣ-�����Ϣ���������С��ѭ������
    @param [out] ip Ŀ��ip
    @param [out] port Ŀ��port
    @param [out] msg �յ�����Ϣ
    */
    virtual int RcvMsg(DogStringA& msg, DogStringA& ip, unsigned short& port);

    /** ������Ϣ
    @param [out] sockAddr Ŀ��sockAddr
    @param [out] msg �յ�����Ϣ
    @return �ɹ����ؽ��ܵĸ�����ʧ�ܷ���SOCKET_ERROR
    */
    virtual int RcvMsg(DogStringA& msg, SOCKADDR& sockAddr);

//     /** ������Ϣ
//     @param [out] callBack �ص�����
//                  @param pBuff
//                  @param buffSize
//                  @return ���Ϊfalseʱ����ֹ����
//     @param [out] ip Ŀ��ip
//     @param [out] port Ŀ��port
//     @return �ɹ����ؽ��ܵĸ�����ʧ�ܷ���SOCKET_ERROR
//     */
//     virtual void RcvMsg(std::function<bool(char* pBuff, unsigned int buffSize)> callBack,
//                         std::string& ip, unsigned short& port);
// 
//      /** ������Ϣ
//      @param [out] callBack �ص�����
//              @param pBuff
//              @param buffSize
//              @return ���Ϊfalseʱ����ֹ����
//      @param [out] msg �յ�����Ϣ
//      */
//      virtual void RcvMsg(std::function<bool(char* pBuff, unsigned int buffSize)> callBack, SOCKADDR& sockAddr);
};

#endif // __SOCKET_UDP_BASE_H_

