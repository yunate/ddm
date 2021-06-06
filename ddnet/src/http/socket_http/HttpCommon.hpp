
#ifndef __HTTP_HEADER_H_
#define __HTTP_HEADER_H_

#include "stream/IDogStream.h"
#include "typedef/DogString.h"

#include <functional>
#include <map>
#include <memory>

/** ���Ȼص�����
    @functional:
    @param [in] currentSize ��ǰ��С
    @param [in] allSize �ܴ�С
*/
using  DogPercentCallBack = std::function<void(const u64 currentSize, const u64 allSize)>;

/** ��������ָ��
*/
using SPDogStream = std::shared_ptr<IDogStream>;

/** request/reponse ������Ϣ
*/
class HttpAssister
{
public:
    /** ����һ����ֵ�ԣ������ظ�
    @param [in] name ����
    @param [in] value ֵ
    */
    inline void Set(const DogStringA& name, const DogStringA& value)
    {
        m_keyVal.insert(std::make_pair(name, value));
    }

    /** ����һ����ֵ�ԣ�ɾ����ǰ��
    @param [in] name ����
    @param [in] value ֵ
    */
    inline void ReSetOne(const DogStringA& name, const DogStringA& value)
    {
        m_keyVal.erase(name);
        m_keyVal.insert(std::make_pair(name, value));
    }

    /** ɾ��һ��
    @param [in] name ����
    */
    inline void Delete(const DogStringA& name)
    {
        m_keyVal.erase(name);
    }

    /** ���ý��Ȼص�
    @param [in] callback: ���Ȼص�
    */
    void SetPercentCallBack(const DogPercentCallBack& callback)
    {
        m_percentCallBack = callback;
    }

    /** ��ûص�
    @return ���Ȼص�
    */
    DogPercentCallBack& GetPercentCallBack()
    {
        return m_percentCallBack;
    }

    /** �������/������
    @param [in] stream: ���/����
    */
    void SetStream(const SPDogStream& stream)
    {
        m_stream = stream;
    }

    /** �����
    return ��
    */
    SPDogStream& GetStream()
    {
        return m_stream;
    }

protected:
    /** ��ֵ��
    */
    std::multimap<DogStringA, DogStringA> m_keyVal;

    /** ���Ȼص�
    */
    DogPercentCallBack m_percentCallBack;

    /** ���/������
    */
    SPDogStream m_stream;
};

#endif //__HTTP_HEADER_H_
