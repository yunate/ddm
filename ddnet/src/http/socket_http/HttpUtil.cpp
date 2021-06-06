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

        // 关闭长连接
        request->Set("Connection", "close");

        // 设置请求
        httpClient->SetRequest(request);

        // 设置输出流
        response->SetStream(responseStream);

        // 设置回复
        httpClient->SetResponse(response);

        // 设置超时时间
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

        // 设置输入流
        requestStream->Write((u8*)data.c_str(), data.length());
        request->SetStream(requestStream);

        // 关闭长连接
        request->Set("Connection", "close");

        // 设置请求
        httpClient->SetRequest(request);

        // 设置输出流
        response->SetStream(responseStream);

        // 设置回复
        httpClient->SetResponse(response);

        // 设置超时时间
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

        // 设置文件头部
        requestStream->Seek(0, SEEK_SET);

        // 设置输入流
        request->SetStream(requestStream);

        // 关闭长连接
        request->Set("Connection", "close");

        // 设置请求
        httpClient->SetRequest(request);

        // 设置输出流
        response->SetStream(responseStream);

        // 设置回复
        httpClient->SetResponse(response);

        // 设置数据超时时间，不要设置全局超时，因为上传文件很可能会需要很长时间
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

        // 关闭长连接
        request->Set("Connection", "close");

        // 断点续传相关
        s64 size = responseStream->Size();

        if (size > 0)
        {
            char len[64] = {0};
            ::sprintf_s(len, "bytes=%lld-", size);
            request->Set("Range", len);
            responseStream->Seek(0, SEEK_END);
        }

        // 设置请求
        httpClient->SetRequest(request);

        // 设置输出流
        response->SetStream(responseStream);

        // 设置头部接收完成回调，用来看看服务器是否支持断点续传
        if (size > 0)
        {
            response->SetAllHeadCallBack([&size](HttpResponse& response)
            {
                if (size > 0)
                {
                    // 如果不等于206说明不支持断点续传
                    if (response.GetStatusCode() != 206)
                    {
                        // 暂时先返回false，以后可以重新创建文件，但是千万不要将steam seek到开始去写
                        return false;
                    }
                }

                return true;
            });
        }

        // 设置回复
        httpClient->SetResponse(response);

        // 设置数据超时时间，不要设置全局超时，因为上传文件很可能会需要很长时间
        httpClient->SetDataTimeOut(15000);
        return httpClient;
    }
}

