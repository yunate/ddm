
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

    /** ����Ƿ������ʵ��
    @reutrn �Ƿ����
    */
    virtual inline bool HasObj(ExecutorChecker* pR)
    {
        if (pR == nullptr)
        {
            return false;
        }

        return (&m_ == &pR->m_);
    }

    /** ֻ��Ϊ�˿�����ַ�Ƿ���ͬ��û��ʵ������
    */
    char m_ = 0;
};
#endif

/** ִ����
*/
class IExecutor
#ifdef _DEBUG
    : virtual public ExecutorChecker
#endif
{
public:
    /** ��������
    */
    virtual ~IExecutor()
    {

    }

    /** ִ��
    @param [in] pData ����
    @return �Ƿ�ɹ�
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

    /** ���һ��ִ����
    @param [in] pExecutor ���������ִ���������಻��ȥ����pExecutor��ע��Ұָ������
    @param [in] checkRecursive �Ƿ��鼴�������IExecutor���Ƿ��Ѿ���������
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
    /** ����Ƿ��ǰ���ʵ��
    @reutrn �Ƿ����
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
    /** ����ִ����
    */
    std::vector<IExecutor*> m_executors;
};

#endif // __LOGIC_EXECUTOR_H_
