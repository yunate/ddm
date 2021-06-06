
#ifndef _SOCKET_HTTP_CLIENT_H__
#define _SOCKET_HTTP_CLIENT_H__

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "noncopyable/noncopyable.h"
#include "timer_recorder/timer_recorder.h"

/** socket ��װ������ָ��
*/
class SocketBase;
class SocketHttpClient;
using SPSocketClient = std::shared_ptr<SocketBase>;
using SPRequest = std::shared_ptr<HttpRequest>;
using SPResponse = std::shared_ptr<HttpResponse>;
using SPHttpClient = std::shared_ptr<SocketHttpClient>;

/** http��
*/
class SocketHttpClient:
    public NonCopyable
{
public:
    /** ���캯��
    */
    SocketHttpClient();

    /** ��������
    */
    ~SocketHttpClient();

public:
    /** set/get �������
    */
    void SetRequest(const SPRequest& request);
    SPRequest& GetRequest();

    /** set/get �������ظ�����
    */
    void SetResponse(const SPResponse& response);
    SPResponse& GetResponse();

    /** ��ʼ����
    @return �Ƿ�ɹ�,�ú�������ȥ��鷵�������Ƿ���Ч���Ƿ�������ֻҪ�����ݷ��أ���Ϊtrue
    */
    bool MakeRequest();

private:
    /** ��������
    @param [in] spClient tcp����������Ч��������жϣ������ڲ�����assert
    @return �Ƿ�ɹ�
    */
    bool RecvResponse(SPSocketClient spClient);

    /** ����body��ĿǰֻPOST����
    @param [in] spClient tcp����������Ч��������жϣ������ڲ�����assert
    @return �Ƿ�ɹ�
    */
    bool SendBody(SPSocketClient spClient);

public:
    /** ���Գ�ʱʱ�䣬�ӽ��뺯����ʼ�����˳���������ȴ�ʱ�䣬����һ�����Գ�ʱʱ�䣬����㲻ϣ�����ܵ�����ֻ����һ��ͽ����ˣ�������Ϊ0
    @note: Ĭ��0
    @note: Ϊ0ʱ��Ϊ������ʱ
    @param [in] timeOut: ��ʱʱ�� (ms)
    */
    void SetGTimeOut(u32 timeOut);

    /** ���ݳ�ʱʱ�䣬ָ������һ�����ݵ���һ������֮�����ȴ�ʱ��
    @note: Ĭ��15000ms
    @note: Ϊ0ʱ��Ϊ������ʱ
    @param [in] timeOut: ��ʱʱ�� (ms)
    */
    void SetDataTimeOut(u32 timeOut);

    /** ֹͣ����
    */
    void Stop();

protected:
    /** �������ظ�
    */
    SPResponse m_spReponse;

    /** ����
    */
    SPRequest m_spRequest;

    /** (ms) ���Գ�ʱʱ�䣬�ӽ��뺯����ʼ�����˳���������ȴ�ʱ�䣬����һ�����Գ�ʱʱ�䣬����㲻ϣ�����ܵ�����ֻ����һ��ͽ����ˣ�������Ϊ0
    @note: Ϊ0ʱ��Ϊ������ʱ
    */
    u32 m_gTimeOut;

    /** ȫ�ּ�ʱ��
    */
    xtimer m_gTimer;

    /** (ms) ���ݳ�ʱʱ�䣬ָ������һ�����ݵ���һ������֮�����ȴ�ʱ��
    @note: ��Ȼ�������ȼ�����m_gTimeOut
    */
    u32 m_dataTimeOut;

    /** �Ƿ�ֹͣ
    */
    bool m_stop;
};

#endif // _SOCKET_HTTP_CLIENT_H__
