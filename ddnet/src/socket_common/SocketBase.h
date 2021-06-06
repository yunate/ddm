
#ifndef __SOCKETBASE_H_
#define __SOCKETBASE_H_

#include "SocketBean.h"

/** TODO:error_message
*/
class SocketBase
{
public:
    /** 构造函数
    */
    SocketBase();

    /** 析构函数
    */
    virtual ~SocketBase()
    {
        UnInit();
    }

    /** 不允许拷贝
    */
    SocketBase(const SocketBase&) = delete;
    SocketBase& operator=(const SocketBase&) = delete;

protected:
    /** 初始化，不要放到构造函数里面
    @param [in] type socket 数据传输类型
    @param [in] ipProto 协议
    return 是否成功
    */
    virtual bool Init(SocketType type, IpProtocolType ipProto);

    /** 清理资源
    return 是否成功
    */
    bool UnInit();

public:
    /** 获得buff的size
    @return buff 的size
    */
    inline unsigned int GetBuffSize()
    {
        return m_buffSize;
    }

    /** 设置buff size
    @param [in] buff size
    */
    inline void SetBuffSize(unsigned int size)
    {
        m_buffSize = size;
    }

public:
    /** 获得m_socketNode
    @return socket 数据信息
    */
    inline SocketBean& GetSocketBean()
    {
        return m_socketBean;
    }

private:
    /** socket 数据信息
    */
    SocketBean m_socketBean;

    /** 缓存大小
    */
    unsigned int m_buffSize;
};

#endif //__ISOCKET_H_
