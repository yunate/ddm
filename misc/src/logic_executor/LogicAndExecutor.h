
#ifndef __LOGIC_AND_EXECUTOR_H_
#define __LOGIC_AND_EXECUTOR_H_

#include "IExecutor.h"

/** 与逻辑执行者，与或非嵌套时候注意不要循环递归调用
*/
class LogicAndExecutor :
    public IExecutor,
    public ExecutorContainer
{
public:
    /** 析构函数
    */
    virtual ~LogicAndExecutor()
    {
    }

    /** 执行
    @param [in] pData 数据
    @return 是否成功
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
