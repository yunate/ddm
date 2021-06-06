
#ifndef __HTTP_UTILS_H_
#define __HTTP_UTILS_H_

#include "SocketHttpClient.h"

namespace HttpUtils
{
    /** ����httpGet������󣬲�ֱ���ں�����������Ҫ�ǿ��ǵ��첽����
    @param[in] url�����url
    @param[out] out���󷵻صĽ�����ɹ�����������ͷ�����ɹ�ֻ��������ͷ��
    @return http Get�������
    */
    SPHttpClient CreateHttpGet(const DogStringA& url);

    /** ����httpPost�������
    @param[in] url�����url
    @param[in] data�����body
    @return httpPost�������
    */
    SPHttpClient CreateHttpPost(const DogStringA& url, const DogStringA& data);

    /** �����ϴ�����
    @param[in] url�����url
    @param[in] filePath �ļ�·����ע�⺯���ڲ�������filePath��Ч�Լ�飬ֻ��assert��������߱�֤filePath��Ч
    @return httpUpload�������
    */
    SPHttpClient CreateHttpUpLoad(const DogStringA& url, const DogString& filePath);

    /** �������ض���
    @param[in] url�����url
    @param[in] filePath �ļ�·����ע�⺯���ڲ�������filePath��Ч�Լ�飬ֻ��assert��������߱�֤filePath��Ч
    @return httpDownload�������
    */
    SPHttpClient CreateHttpDownload(const DogStringA& url, const DogString& filePath);
}

#endif //__HTTP_UTILS_H_
