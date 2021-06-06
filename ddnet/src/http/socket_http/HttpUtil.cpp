#include "HttpUtil.h"

#include "stream/MemoryStream.h"
#include "stream/FileStream.h"

namespace HttpUtils
{
    SPHttpClient CreateHttpGet(const DogStringA& url)
    {
        SPRequest request(new(std::nothrow) HttpRequest(HttpRequest::GET, url));
        SPResponse response(new(std::nothrow) HttpResponse());
        SPDogStream responseStream(new(std::nothrow)MemoryStream());
        SPHttpClient httpClient(new(std::nothrow) SocketHttpClient());

        if (request == NULL || response == NULL || responseStream == NULL || httpClient == NULL)
        {
            return NULL;
        }

        // �رճ�����
        request->Set("Connection", "close");

        // ��������
        httpClient->SetRequest(request);

        // ���������
        response->SetStream(responseStream);

        // ���ûظ�
        httpClient->SetResponse(response);

        // ���ó�ʱʱ��
        httpClient->SetGTimeOut(15000);
        return httpClient;
    }

    SPHttpClient CreateHttpPost(const DogStringA& url, const DogStringA& data)
    {
        SPRequest request(new(std::nothrow) HttpRequest(HttpRequest::POST, url));
        SPResponse response(new(std::nothrow) HttpResponse());
        SPDogStream requestStream(new(std::nothrow)MemoryStream());
        SPDogStream responseStream(new(std::nothrow)MemoryStream());
        SPHttpClient httpClient(new(std::nothrow) SocketHttpClient());

        if (request == NULL ||
            response == NULL ||
            requestStream == NULL ||
            responseStream == NULL ||
            httpClient == NULL)
        {
            return NULL;
        }

        // ����������
        requestStream->Write((u8*)data.c_str(), data.length());
        request->SetStream(requestStream);

        // �رճ�����
        request->Set("Connection", "close");

        // ��������
        httpClient->SetRequest(request);

        // ���������
        response->SetStream(responseStream);

        // ���ûظ�
        httpClient->SetResponse(response);

        // ���ó�ʱʱ��
        httpClient->SetGTimeOut(15000);
        return httpClient;
    }

    SPHttpClient CreateHttpUpLoad(const DogStringA& url, const DogString& filePath)
    {
        SPRequest request(new(std::nothrow) HttpRequest(HttpRequest::POST, url));
        SPResponse response(new(std::nothrow) HttpResponse());
        SPDogStream requestStream(new(std::nothrow)FileStream(filePath.c_str()));
        SPDogStream responseStream(new(std::nothrow)MemoryStream());
        SPHttpClient httpClient(new(std::nothrow) SocketHttpClient());

        if (request == NULL ||
            response == NULL ||
            requestStream == NULL ||
            responseStream == NULL ||
            httpClient == NULL)
        {
            return NULL;
        }

        // �����ļ�ͷ��
        requestStream->Seek(0, SEEK_SET);

        // ����������
        request->SetStream(requestStream);

        // �رճ�����
        request->Set("Connection", "close");

        // ��������
        httpClient->SetRequest(request);

        // ���������
        response->SetStream(responseStream);

        // ���ûظ�
        httpClient->SetResponse(response);

        // �������ݳ�ʱʱ�䣬��Ҫ����ȫ�ֳ�ʱ����Ϊ�ϴ��ļ��ܿ��ܻ���Ҫ�ܳ�ʱ��
        httpClient->SetDataTimeOut(15000);
        return httpClient;
    }

    SPHttpClient CreateHttpDownload(const DogStringA& url, const DogString& filePath)
    {
        SPRequest request(new(std::nothrow) HttpRequest(HttpRequest::POST, url));
        SPResponse response(new(std::nothrow) HttpResponse());
        SPDogStream responseStream(new(std::nothrow)FileStream(filePath.c_str()));
        SPHttpClient httpClient(new(std::nothrow) SocketHttpClient());

        if (request == NULL ||
            response == NULL ||
            responseStream == NULL ||
            httpClient == NULL)
        {
            return NULL;
        }

        // �رճ�����
        request->Set("Connection", "close");

        // �ϵ��������
        s64 size = responseStream->Size();

        if (size > 0)
        {
            char len[64] = {0};
            ::sprintf_s(len, "bytes=%lld-", size);
            request->Set("Range", len);
            responseStream->Seek(0, SEEK_END);
        }

        // ��������
        httpClient->SetRequest(request);

        // ���������
        response->SetStream(responseStream);

        // ����ͷ��������ɻص������������������Ƿ�֧�ֶϵ�����
        if (size > 0)
        {
            response->SetAllHeadCallBack([&size](HttpResponse& response)
            {
                if (size > 0)
                {
                    // ���������206˵����֧�ֶϵ�����
                    if (response.GetStatusCode() != 206)
                    {
                        // ��ʱ�ȷ���false���Ժ�������´����ļ�������ǧ��Ҫ��steam seek����ʼȥд
                        return false;
                    }
                }

                return true;
            });
        }

        // ���ûظ�
        httpClient->SetResponse(response);

        // �������ݳ�ʱʱ�䣬��Ҫ����ȫ�ֳ�ʱ����Ϊ�ϴ��ļ��ܿ��ܻ���Ҫ�ܳ�ʱ��
        httpClient->SetDataTimeOut(15000);
        return httpClient;
    }
}

