
#ifndef __HTTP_HEADER_H_
#define __HTTP_HEADER_H_

#include "stream/IDogStream.h"
#include "typedef/DogString.h"

#include <functional>
#include <map>
#include <memory>

/** 进度回调函数
    @functional:
    @param [in] currentSize 当前大小
    @param [in] allSize 总大小
*/
using  DogPercentCallBack = std::function<void(const u64 currentSize, const u64 allSize)>;

/** 流的智能指针
*/
using SPDogStream = std::shared_ptr<IDogStream>;

/** request/reponse 辅助信息
*/
class HttpAssister
{
public:
    /** 插入一个键值对，可以重复
    @param [in] name 名称
    @param [in] value 值
    */
    inline void Set(const DogStringA& name, const DogStringA& value)
    {
        m_keyVal.insert(std::make_pair(name, value));
    }

    /** 插入一个键值对，删除以前的
    @param [in] name 名称
    @param [in] value 值
    */
    inline void ReSetOne(const DogStringA& name, const DogStringA& value)
    {
        m_keyVal.erase(name);
        m_keyVal.insert(std::make_pair(name, value));
    }

    /** 删除一个
    @param [in] name 名称
    */
    inline void Delete(const DogStringA& name)
    {
        m_keyVal.erase(name);
    }

    /** 设置进度回调
    @param [in] callback: 进度回调
    */
    void SetPercentCallBack(const DogPercentCallBack& callback)
    {
        m_percentCallBack = callback;
    }

    /** 获得回调
    @return 进度回调
    */
    DogPercentCallBack& GetPercentCallBack()
    {
        return m_percentCallBack;
    }

    /** 设置输出/输入流
    @param [in] stream: 输出/输入
    */
    void SetStream(const SPDogStream& stream)
    {
        m_stream = stream;
    }

    /** 获得流
    return 流
    */
    SPDogStream& GetStream()
    {
        return m_stream;
    }

protected:
    /** 键值对
    */
    std::multimap<DogStringA, DogStringA> m_keyVal;

    /** 进度回调
    */
    DogPercentCallBack m_percentCallBack;

    /** 输出/输入流
    */
    SPDogStream m_stream;
};

#endif //__HTTP_HEADER_H_
