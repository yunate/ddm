#ifndef singleton_hpp_
#define singleton_hpp_ 1

#include "g_def.h"
BEG_NSP_DDM

// 为了能够让T类的构造函数写成protect
template<class T>
class singleton_wrapper : public T {};

// 饿汉模式
template <class T>
class singleton
{
public:
    static T& get_instance()
    {
        // 为了能够在main函数前初始化s_pInstance
        s_pInstance;
        static singleton_wrapper<T> instance;
        return *static_cast<T *>(&instance);
    }

private:
    singleton() {}
    singleton(const singleton&) {}
    singleton& operator = (const singleton&) {}
    ~singleton() {}

    // 强制饿汉模式，这种写法主要是为了防止初始化依赖问题
    static T* s_pInstance;
};

template<class T>
T* singleton<T>::s_pInstance = &singleton<T>::get_instance();

END_NSP_DDM
#endif // singleton_hpp_
