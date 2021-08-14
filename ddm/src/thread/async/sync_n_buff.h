
#ifndef sync_n_buff_h_
#define sync_n_buff_h_ 1

#include "g_def.h"
#include "semaphore.h"
#include <queue>
#include <stack>
#include <list>
BEG_NSP_DDM
enum class sync_buff_ty
{
    sync_buff_ty_queue = 0,
    sync_buff_ty_list = 1,
};

template<class C, sync_buff_ty ty>
class sync_n_buff;

// 同步 带有最大值的队列
template<class T>
using sync_n_queue = sync_n_buff<std::queue<T, std::list<T>>, sync_buff_ty::sync_buff_ty_queue>;

// 同步 带有最大值的栈
template<class T>
using sync_n_stack = sync_n_buff<std::stack<T, std::list<T>>, sync_buff_ty::sync_buff_ty_list>;

// 同步 N 缓存
template<template<class, class> class C, sync_buff_ty ty, class T, class A>
class sync_n_buff<C<T, A>, ty>
{
public:
    // max_cnt = MAX_U32 表示无限
    sync_n_buff(u32 max_cnt = MAX_U32);
    ~sync_n_buff();
    sync_n_buff(const sync_n_buff<C<T, A>, ty>& r);
    sync_n_buff& operator = (const sync_n_buff<C<T, A>, ty>& r);
    sync_n_buff(sync_n_buff<C<T, A>, ty>&& r);
    sync_n_buff& operator = (sync_n_buff<C<T, A>, ty>&& r);

public:
    void lock();
    void unlock();

    void clear();
    void swap(sync_n_buff<C<T, A>, ty>& r);
    u32 size();
    bool empty();

    // wait_time 如果buff满了的等待时间，为MAX_U32表示一直等直到空闲
    // true 表示成功，false 表示失败
    bool push(const T& t, u32 wait_time = MAX_U32);
    u32 push(const C<T, A>& c, u32 wait_time = MAX_U32);

    // 必须返回一个拷贝，这里和stl的pop返回void不一样，因为此类不允许返回应用，因此就没必要 front/top 函数了
    // 如果队列里面没有则未定义行为
    T pop();
    C<T, A> pop_all();
    u32 max_count();
    void set_max_count(u32 cnt);

private:
    // 缓存最大值
    u32 m_max_cnt = MAX_U32;

    // 缓存满等待事件
    semaphore m_buff_busy_event;

    std::recursive_mutex m_mutex;

    // 容器
    C<T, A> m_container;
};

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline sync_n_buff<C<T, A>, ty>::sync_n_buff(u32 max_cnt)
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    m_max_cnt = max_cnt;
    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.reset(m_max_cnt);
    }
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline sync_n_buff<C<T, A>, ty>::~sync_n_buff()
{
    clear();
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline sync_n_buff<C<T, A>, ty>::sync_n_buff(const sync_n_buff<C<T, A>, ty>& r)
{
    *this = r;
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline sync_n_buff<C<T, A>, ty>& sync_n_buff<C<T, A>, ty>::operator=(const sync_n_buff<C<T, A>, ty>& r)
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(r);
    m_max_cnt = r.m_max_cnt;
    m_container = r.m_container;
    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.reset(m_max_cnt);
    }
    return *this
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline sync_n_buff<C<T, A>, ty>::sync_n_buff(sync_n_buff<C<T, A>, ty>&& r)
{
    *this = r;
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline sync_n_buff<C<T, A>, ty>& sync_n_buff<C<T, A>, ty>::operator=(sync_n_buff<C<T, A>, ty>&& r)
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lockr(r);
    m_max_cnt = r.m_max_cnt;
    m_container = std::move(r.m_container);
    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.reset(m_max_cnt);
    }
    return *this
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline void sync_n_buff<C<T, A>, ty>::lock()
{
    m_mutex.lock();
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline void sync_n_buff<C<T, A>, ty>::unlock()
{
    m_mutex.unlock();
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline void sync_n_buff<C<T, A>, ty>::clear()
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    C<T, A> tmp;
    m_container.swap(tmp);
    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.reset(m_max_cnt);
    }
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline void sync_n_buff<C<T, A>, ty>::swap(sync_n_buff<C<T, A>, ty>& r)
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lockr(r);
    std::swap(m_max_cnt, r.m_max_cnt);
    m_max_cnt = r.m_max_cnt;
    m_container.swap(r.m_container);

    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.reset(m_max_cnt);
    }

    if (r.m_max_cnt != MAX_U32) {
        r.m_buff_busy_event.reset(r.m_max_cnt);
    }
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline bool sync_n_buff<C<T, A>, ty>::push(const T& t, u32 wait_time/* = MAX_U32 */)
{
    if (m_max_cnt == MAX_U32 || m_buff_busy_event.wait(wait_time)) {
        std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
        m_container.push(t);
        return true;
    }
   
    return false;
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline u32 sync_n_buff<C<T, A>, ty>::push(const C<T, A>& c, u32 wait_time/* = MAX_U32 */)
{
    u32 cnt = 0;
    for (const auto& it : c) {
        if (m_max_cnt == MAX_U32 || m_buff_busy_event.wait(wait_time)) {
            std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
            m_container.push(it);
            ++cnt;
        } else {
            return cnt;
        }
    }
    return cnt;
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline T sync_n_buff<C<T, A>, ty>::pop()
{
    if constexpr (ty == sync_buff_ty::sync_buff_ty_list)
    {
        std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
        T rtn = m_container.top();
        m_container.pop();
        if (m_max_cnt != MAX_U32) {
            m_buff_busy_event.notify_one();
        }
        return rtn;
    }
    else if constexpr (ty == sync_buff_ty::sync_buff_ty_queue)
    {
        std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
        T rtn = m_container.front();
        m_container.pop();
        if (m_max_cnt != MAX_U32) {
            m_buff_busy_event.notify_one();
        }
        return rtn;
    }
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline C<T, A> sync_n_buff<C<T, A>, ty>::pop_all()
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    C<T> rtn = std::move(m_container);
    if (m_max_cnt != MAX_U32) {
        m_buff_busy_event.reset(m_max_cnt);
    }
    return rtn;
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline u32 sync_n_buff<C<T, A>, ty>::size()
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    return m_container.size();
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline bool sync_n_buff<C<T, A>, ty>::empty()
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    return m_container.empty();
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline u32 sync_n_buff<C<T, A>, ty>::max_count()
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    return m_max_cnt;
}

template<template<class, class> class C, sync_buff_ty ty, class T, class A>
inline void sync_n_buff<C<T, A>, ty>::set_max_count(u32 cnt)
{
    std::lock_guard<sync_n_buff<C<T, A>, ty>> lock(*this);
    m_max_cnt = cnt;
}

END_NSP_DDM
#endif // sync_n_buff_h_