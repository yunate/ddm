
#ifndef sync_n_buff_h_
#define sync_n_buff_h_ 1

#include "g_def.h"
#include "semaphore.h"
BEG_NSP_DDM
template<class C>
class sync_n_buff;

// 同步 N 缓存
template<template<class> class C, class T>
class sync_n_buff<C<T>>
{
public:
    sync_n_buff(u32 max_cnt);
    ~sync_n_buff();
    sync_n_buff(const sync_n_buff<C<T>>& r);
    sync_n_buff& operator = (const sync_n_buff<C<T>>& r);
    sync_n_buff(sync_n_buff<C<T>>&& r);
    sync_n_buff& operator = (sync_n_buff<C<T>>&& r);

public:
    void lock();
    void unlock();

    void clear();

    // wait_time 如果buff满了的等待时间，为MAX_U32表示一直等直到空闲
    // true 表示成功，false 表示失败
    bool push(const T& t, u32 wait_time = MAX_U32);
    u32 push(const C<T>& c, u32 wait_time = MAX_U32);

    // 必须返回一个拷贝，这里和stl的pop返回void不一样，因为此类不允许返回应用，因此就没必要 front/top 函数了
    // 如果队列里面没有则未定义行为
    T pop();
    C<T> pop_all();
    u32 count();
    u32 max_count();
    void set_max_count(u32 cnt);

private:
    // 缓存最大值
    u32 m_max_cnt = MAX_U32;

    // 缓存满等待事件
    semaphore m_buff_busy_event;

    std::recursive_mutex m_mutex;

    // 容器
    C m_container;
};

template<template<class> class C, class T>
inline sync_n_buff<C<T>>::sync_n_buff(u32 max_cnt)
{
    std::unique_lock<std::mutex> lck(*this);
    m_max_cnt = max_cnt;
}

template<template<class> class C, class T>
inline sync_n_buff<C<T>>::~sync_n_buff()
{
    clear();
}

template<template<class> class C, class T>
inline sync_n_buff<C<T>>::sync_n_buff(const sync_n_buff<C<T>>& r)
{
    *this = r;
}

template<template<class> class C, class T>
inline sync_n_buff<C<T>>& sync_n_buff<C<T>>::operator=(const sync_n_buff<C<T>>& r)
{
    std::unique_lock<std::mutex> lck(*this);
    std::unique_lock<std::mutex> lck(r);
    m_max_cnt = r.m_max_cnt;
    m_container = r.m_container;
    return *this
}

template<template<class> class C, class T>
inline sync_n_buff<C<T>>::sync_n_buff(sync_n_buff<C<T>>&& r)
{
    *this = r;
}

template<template<class> class C, class T>
inline sync_n_buff<C<T>>& sync_n_buff<C<T>>::operator=(sync_n_buff<C<T>>&& r)
{
    std::unique_lock<std::mutex> lck(*this);
    std::unique_lock<std::mutex> lck(r);
    m_max_cnt = r.m_max_cnt;
    m_container = std::move(r.m_container);
    return *this
}

template<template<class> class C, class T>
inline void sync_n_buff<C<T>>::lock()
{
    m_mutex.lock();
}

template<template<class> class C, class T>
inline void sync_n_buff<C<T>>::unlock()
{
    m_mutex.unlock();
}

template<template<class> class C, class T>
inline void sync_n_buff<C<T>>::clear()
{
    std::unique_lock<std::mutex> lck(*this);
    m_container.clear();
    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.reset(m_max_cnt);
    }
}

template<template<class> class C, class T>
inline bool sync_n_buff<C<T>>::push(const T& t, u32 wait_time/* = MAX_U32 */)
{
    if (m_max_cnt == MAX_U32 || m_buff_busy_event.wait(wait_time)) {
        std::unique_lock<std::mutex> lck(*this);
        m_container.push(t);
        return true;
    }
   
    return false;
}

template<template<class> class C, class T>
inline u32 sync_n_buff<C<T>>::push(const C<T>& c, u32 wait_time/* = MAX_U32 */)
{
    u32 cnt = 0;
    for (const auto& it : c) {
        if (m_max_cnt == MAX_U32 || m_buff_busy_event.wait(wait_time)) {
            std::unique_lock<std::mutex> lck(*this);
            m_container.push(it);
            ++cnt;
        } else {
            return cnt;
        }
    }
    return cnt;
}

template<template<class> class C, class T>
inline T sync_n_buff<C<T>>::pop()
{
    std::unique_lock<std::mutex> lck(*this);
    T rtn = m_container.front();
    m_container.pop();
    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.notify_one();
    }
    return rtn;
}

template<template<class> class C, class T>
inline C<T> sync_n_buff<C<T>>::pop_all()
{
    std::unique_lock<std::mutex> lck(*this);
    C<T> rtn = std::move(m_container);
    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.reset(m_max_cnt);
    }
    return rtn;
}

template<template<class> class C, class T>
inline u32 sync_n_buff<C<T>>::count()
{
    std::unique_lock<std::mutex> lck(*this);
    return m_container.count();
}

template<template<class> class C, class T>
inline u32 sync_n_buff<C<T>>::max_count()
{
    std::unique_lock<std::mutex> lck(*this);
    return m_max_cnt;
}

template<template<class> class C, class T>
inline void sync_n_buff<C<T>>::set_max_count(u32 cnt)
{
    std::unique_lock<std::mutex> lck(*this);
    m_max_cnt = cnt;
}

END_NSP_DDM
#endif // sync_n_buff_h_