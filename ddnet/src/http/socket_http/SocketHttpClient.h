
#ifndef _SOCKET_HTTP_CLIENT_H__
#define _SOCKET_HTTP_CLIENT_H__

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "noncopyable/noncopyable.h"
#include "timer_recorder/timer_recorder.h"

/** socket 封装类智能指针
*/
class SocketBase;
class SocketHttpClient;
using SPSocketClient = std::shared_ptr<SocketBase>;
using SPRequest = std::shared_ptr<HttpRequest>;
using SPResponse = std::shared_ptr<HttpResponse>;
using SPHttpClient = std::shared_ptr<SocketHttpClient>;

/** http类
*/
class SocketHttpClient:
    public NonCopyable
{
public:
    /** 构造函数
    */
    SocketHttpClient();

    /** 析构函数
    */
    ~SocketHttpClient();

public:
    /** set/get 请求对象
    */
    void SetRequest(const SPRequest& request);
    SPRequest& GetRequest();

    /** set/get 服务器回复对象
    */
    void SetResponse(const SPResponse& response);
    SPResponse& GetResponse();

    /** 开始请求
    @return 是否成功,该函数不会去检查返回数据是否有效，是否完整，只要有数据返回，就为true
    */
    bool MakeRequest();

private:
    /** 接受数据
    @param [in] spClient tcp对象，它的有效性在外边判断，函数内部将会assert
    @return 是否成功
    */
    bool RecvResponse(SPSocketClient spClient);

    /** 发送body，目前只POST发送
    @param [in] spClient tcp对象，它的有效性在外边判断，函数内部将会assert
    @return 是否成功
    */
    bool SendBody(SPSocketClient spClient);

public:
    /** 绝对超时时间，从进入函数开始，到退出函数的最长等待时间，这是一个绝对超时时间，如果你不希望接受的数据只接受一般就结束了，请设置为0
    @note: 默认0
    @note: 为0时候为永不超时
    @param [in] timeOut: 超时时间 (ms)
    */
    void SetGTimeOut(u32 timeOut);

    /** 数据超时时间，指的是上一次数据到下一次数据之间的最长等待时间
    @note: 默认15000ms
    @note: 为0时候为永不超时
    @param [in] timeOut: 超时时间 (ms)
    */
    void SetDataTimeOut(u32 timeOut);

    /** 停止请求
    */
    void Stop();

protected:
    /** 服务器回复
    */
    SPResponse m_spReponse;

    /** 请求
    */
    SPRequest m_spRequest;

    /** (ms) 绝对超时时间，从进入函数开始，到退出函数的最长等待时间，这是一个绝对超时时间，如果你不希望接受的数据只接受一般就结束了，请设置为0
    @note: 为0时候为永不超时
    */
    u32 m_gTimeOut;

    /** 全局计时器
    */
    xtimer m_gTimer;

    /** (ms) 数据超时时间，指的是上一次数据到下一次数据之间的最长等待时间
    @note: 显然它的优先级低于m_gTimeOut
    */
    u32 m_dataTimeOut;

    /** 是否停止
    */
    bool m_stop;
};

#endif // _SOCKET_HTTP_CLIENT_H__
