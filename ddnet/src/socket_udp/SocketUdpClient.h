
#ifndef __SOCKETUDP_CLIENT_H_
#define __SOCKETUDP_CLIENT_H_

#include "SocketUdpBase.h"

class SocketUdpClient :
    public SocketUdpBase
{
public:
    /** ���캯��
    */
    SocketUdpClient();

    /** ��������
    */
    ~SocketUdpClient();
};

#endif // __SOCKETUDP_CLIENT_H_

