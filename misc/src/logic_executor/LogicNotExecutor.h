
#ifndef __LOGIC_NOT_EXECUTOR_H_
#define __LOGIC_NOT_EXECUTOR_H_

#include "IExecutor.h"

/** 非逻辑执行者，与或非嵌套时候注意不要循环递归调用
*/
class LogicNotExecutor :
    public IExecutor,
    public ExecutorContainer
{
public:
    /** 构造函数
    */
    LogicNotExecutor()
    {

    }

    /** 析构函数
    */
    virtual ~LogicNotExecutor()
    {
    }

    /** 执行
    @param [in] pData 数据
    @return 是否成功
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
