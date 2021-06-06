
#ifndef __SOCKET_TCP_CLIENT_H_
#define __SOCKET_TCP_CLIENT_H_

#include "SocketTcpBase.h"

class SocketTcpClient :
    public SocketTcpBase
{
public:
    /** ���캯��
    */
    SocketTcpClient();

    /** ���캯��
    @param [in] ip ������ip��ַ
    @param [in] port �����������˿�
    */
    SocketTcpClient(const DogStringA& ip, unsigned short port);

    /** ��������
    */
    virtual ~SocketTcpClient();

public:
    /** ���ӵ� [��������]
        ��ʼ��socket -> ����
    @return �Ƿ�ɹ�
    */
    bool Connect();

    /**
    @param [in] ip ������ip��ַ
    @param [in] port �����������˿�
    */
    bool Connect(const DogStringA& ip, unsigned short port);
};

#endif // __SOCKET_TCP_CLIENT_H_


