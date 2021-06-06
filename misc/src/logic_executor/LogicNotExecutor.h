
#ifndef __LOGIC_NOT_EXECUTOR_H_
#define __LOGIC_NOT_EXECUTOR_H_

#include "IExecutor.h"

/** ���߼�ִ���ߣ�����Ƕ��ʱ��ע�ⲻҪѭ���ݹ����
*/
class LogicNotExecutor :
    public IExecutor,
    public ExecutorContainer
{
public:
    /** ���캯��
    */
    LogicNotExecutor()
    {

    }

    /** ��������
    */
    virtual ~LogicNotExecutor()
    {
    }

    /** ִ��
    @param [in] pData ����
    @return �Ƿ�ɹ�
    */
    virtual inline bool Execute(void* pData)
    {
#ifdef _DEBUG
        assert(m_executors.size() == 1);
#endif

        if (m_executors.size() > 0)
        {
            return m_executors[0]->Execute(pData);
        }

        return false;
    }

private:
};

#endif // __LOGIC_NOT_EXECUTOR_H_
