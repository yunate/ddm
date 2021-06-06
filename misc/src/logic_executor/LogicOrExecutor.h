
#ifndef __LOGIC_OR_EXECUTOR_H_
#define __LOGIC_OR_EXECUTOR_H_

#include "IExecutor.h"

/** ���߼�ִ���ߣ�����Ƕ��ʱ��ע�ⲻҪѭ���ݹ����
*/
class LogicOrExecutor :
    public IExecutor,
    public ExecutorContainer
{
public:
    /** ��������
    */
    virtual ~LogicOrExecutor()
    {
        m_executors.clear();
    }

    /** ִ��
    @param [in] pData ����
    @return �Ƿ�ɹ�
    */
    virtual inline bool Execute(void* pData)
    {
#ifdef _DEBUG
        assert(!m_executors.empty());
#endif

        for (auto it : m_executors)
        {
            if (it->Execute(pData))
            {
                return true;
            }
        }

        return false;
    }
};

#endif // __LOGIC_OR_EXECUTOR_H_
