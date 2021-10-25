#ifndef ddlazy_exec_h_
#define ddlazy_exec_h_

#include "g_def.h"
#include "nocopyable.hpp"
#include <mutex>
#include <list>
#include <functional>

BEG_NSP_DDM

// 保证所有的执行体一定在调用了 ready() 之后执行
class ddlazy_exec : public nocopyable
{
public:
    ddlazy_exec() = default;
    ~ddlazy_exec() = default;

    using lazy_excutable = std::function<void()>;
    inline void exec(const lazy_excutable& exec)
    {
        m_mutex.lock();
        if (m_ready) {
            m_mutex.unlock();
            exec();
            return;
        }

        m_execs.push_back(exec);
        m_mutex.unlock();
    }

    inline void ready(bool execAll)
    {
        {
            std::lock_guard<std::recursive_mutex> lock(m_mutex);
            m_ready = true;
        }

        if (execAll) {
            for (const auto& it : m_execs) {
                it();
            }
        }
        m_execs.clear();
    }

    inline void reset()
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        m_ready = false;
        m_execs.clear();
    }

private:
    std::recursive_mutex m_mutex;
    std::list<lazy_excutable> m_execs;
    bool m_ready = false;
};

END_NSP_DDM
#endif
