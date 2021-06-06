#pragma once

#include "LogicAndExecutor.h"
#include "LogicOrExecutor.h"
#include "LogicNotExecutor.h"

namespace utilstest
{
    class Exe1 : public IExecutor
    {
    public:
        virtual inline bool Execute(void* pData)
        {
            pData;
            return true;
        }
    };

    static void Test_Executor()
    {
        IExecutor* p1 = new Exe1();
        IExecutor* p2 = new Exe1();
        IExecutor* p3 = new Exe1();
        IExecutor* p4 = new Exe1();
        IExecutor* p5 = new Exe1();

        LogicAndExecutor* pAndExe = new LogicAndExecutor();
        LogicOrExecutor* pOrExe = new LogicOrExecutor();
        LogicNotExecutor* pNotExe = new LogicNotExecutor();

        pAndExe->PushExecutors(p1);
        pAndExe->PushExecutors(p2);

        pOrExe->PushExecutors(p3);
        pOrExe->PushExecutors(p4);

        pNotExe->PushExecutors(p5);

        pOrExe->PushExecutors(pAndExe);
        pAndExe->PushExecutors(pNotExe);
        pNotExe->PushExecutors(pOrExe);
    }
   
}