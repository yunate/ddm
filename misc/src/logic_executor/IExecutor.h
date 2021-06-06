
#ifndef __LOGIC_EXECUTOR_H_
#define __LOGIC_EXECUTOR_H_
#include <vector>

#ifdef _DEBUG
#include <assert.h>
#pragma warning(disable:4250)

class ExecutorChecker
{
public:
    virtual ~ExecutorChecker()
    {
    }

    /** 检查是否包含个实例
    @reutrn 是否包含
    */
    virtual inline bool HasObj(ExecutorChecker* pR)
    {
        if (pR == nullptr)
        {
            return false;
        }

        return (&m_ == &pR->m_);
    }

    /** 只是为了看看地址是否相同，没有实际意义
    */
    char m_ = 0;
};
#endif

/** 执行者
*/
class IExecutor
#ifdef _DEBUG
    : virtual public ExecutorChecker
#endif
{
public:
    /** 析构函数
    */
    virtual ~IExecutor()
    {

    }

    /** 执行
    @param [in] pData 数据
    @return 是否成功
    */
    virtual bool Execute(void* pData) = 0;
};

class ExecutorContainer
#ifdef _DEBUG
    : virtual public ExecutorChecker
#endif
{
public:
    virtual ~ExecutorContainer()
    {
        m_executors.clear();
    }

    /** 添加一个执行器
    @param [in] pExecutor 即将加入的执行器，本类不会去销毁pExecutor，注意野指针问题
    @param [in] checkRecursive 是否检查即将插入的IExecutor中是否已经被包含了
    */
    inline void PushExecutors(IExecutor* pExecutor)
    {
#ifdef _DEBUG
        assert(!pExecutor->HasObj(this));
#endif
        if (pExecutor != nullptr)
        {
            m_executors.push_back(pExecutor);
        }
    }

#ifdef _DEBUG
    /** 检查是否是包含实例
    @reutrn 是否包含
    */
    virtual inline bool IsSameObj(ExecutorChecker* pR)
    {
        if (pR == nullptr)
        {
            return false;
        }

        if (ExecutorChecker::HasObj(pR))
        {
            return true;
        }

        for (auto it : m_executors)
        {
            if (it->HasObj(pR))
            {
                return true;
            }
        }

        return false;
    }
#endif

protected:
    /** 所有执行器
    */
    std::vector<IExecutor*> m_executors;
};

#endif // __LOGIC_EXECUTOR_H_
