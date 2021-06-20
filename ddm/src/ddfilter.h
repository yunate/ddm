#ifndef ddfilter_h
#define ddfilter_h
#include "g_def.h"

#include <functional>
BEG_NSP_DDM

template<class F, class F1>
class cluser_with_filter
{
public:
     cluser_with_filter(F callable, F1 filter) : m_callable(callable), m_filter(filter) { }

     template<typename ...V>
     inline auto operator()(V&&... a)
     {
         using R = decltype(m_callable(std::forward<V>(a)...));
         if (m_filter(std::forward<V>(a)...)) {
             return m_callable(std::forward<V>(a)...);
         }
         return R();
     }

     inline cluser_with_filter& operator*()
     {
         return *this;
     }

private:
    F m_callable;
    F1 m_filter;
};

template<class F, class F1>
inline auto create_cluser_with_filter(F callable, F1 filter)
{
    return cluser_with_filter<F, F1>(callable, filter);
}

END_NSP_DDM
#endif
