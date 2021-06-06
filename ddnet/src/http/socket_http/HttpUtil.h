
#ifndef __HTTP_UTILS_H_
#define __HTTP_UTILS_H_

#include "SocketHttpClient.h"

namespace HttpUtils
{
    /** 创建httpGet请求对象，不直接在函数内请求主要是考虑到异步问题
    @param[in] url请求的url
    @param[out] out请求返回的结果（成功不包含返回头，不成功只返回请求头）
    @return http Get请求对象
    */
    SPHttpClient CreateHttpGet(const DogStringA& url);

    /** 创建httpPost请求对象
    @param[in] url请求的url
    @param[in] data请求的body
    @return httpPost请求对象
    */
    SPHttpClient CreateHttpPost(const DogStringA& url, const DogStringA& data);

    /** 创建上传对象
    @param[in] url请求的url
    @param[in] filePath 文件路径，注意函数内部不会做filePath有效性检查，只会assert，请调用者保证filePath有效
    @return httpUpload请求对象
    */
    SPHttpClient CreateHttpUpLoad(const DogStringA& url, const DogString& filePath);

    /** 创建下载对象
    @param[in] url请求的url
    @param[in] filePath 文件路径，注意函数内部不会做filePath有效性检查，只会assert，请调用者保证filePath有效
    @return httpDownload请求对象
    */
    SPHttpClient CreateHttpDownload(const DogStringA& url, const DogString& filePath);
}

#endif //__HTTP_UTILS_H_
