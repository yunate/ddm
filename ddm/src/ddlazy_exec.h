#ifndef ddlazy_exec_h_
#define ddlazy_exec_h_

#include "g_def.h"
#include "nocopyable.hpp"
#include <mutex>
#include <list>
#include <functional>

BEG_NSP_DDM

// ��֤���е�ִ����һ���ڵ����� ready() ֮��ִ��
class ddlazy_exec
{
    DD_NO_COPY_MOVE(ddlazy_exec);
public:
    ddlazy_exec() = default;
    ~ddlazy_exec() = default;

    using lazy_excutable = std::function<void()>;
    inline void exec(const lazy_excutable& excutable)
    {
        m_mutex.lock();
        if (m_ready) {
            m_mutex.unlock();
            excutable();
            return;
        }

        m_execs.push_back(excutable);
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
