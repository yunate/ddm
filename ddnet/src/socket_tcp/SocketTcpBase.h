
#ifndef __SOCKET_TCP_BASE_H_
#define __SOCKET_TCP_BASE_H_

#include "socket_common/SocketBase.h"
#include <functional>


class SocketTcpBase :
    public SocketBase
{
public:
    /** 构造函数
    */
    SocketTcpBase();

    /** 析构函数
    */
    virtual ~SocketTcpBase();

public:
    /** 发送消息 当msg大于大约 1K时候返回的值可能不等于msg的值（一次性发送不了）
    @param [in] msg 希望发送的消息
    @return 成功返回发送的个数，失败返回SOCKET_ERROR
    */
    virtual int SendMsg(const DogStringA& msg);

    /** 接收消息，接受GetBuffSize()个字符，这个缓冲大小可以设置
    @param [out] msg 收到的消息
    @return 成功返回接受的个数，失败返回0
    */
    virtual int RcvMsg(DogStringA& msg);

    /** 接收消息，一次性不能够全部接受完全
    @param [out] callBack 回调函数
                 @param pBuff 
                 @param buffSize
                 @return 如果为false时候，终止接受
    */
    virtual void RcvMsg(std::function<bool(char* pBuff, unsigned int buffSize)> callBack);
};

#endif // __SOCKET_TCP_BASE_H_

