
#ifndef __LOGIC_AND_EXECUTOR_H_
#define __LOGIC_AND_EXECUTOR_H_

#include "IExecutor.h"

/** ���߼�ִ���ߣ�����Ƕ��ʱ��ע�ⲻҪѭ���ݹ����
*/
class LogicAndExecutor :
    public IExecutor,
    public ExecutorContainer
{
public:
    /** ��������
    */
    virtual ~LogicAndExecutor()
    {
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
            if (!it->Execute(pData))
            {
                return false;
            }
        }

        return true;
    }
};


#endif // __LOGIC_AND_EXECUTOR_H_
