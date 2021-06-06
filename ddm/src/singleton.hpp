#ifndef singleton_hpp_
#define singleton_hpp_ 1

#include "g_def.h"
BEG_NSP_DDM

// Ϊ���ܹ���T��Ĺ��캯��д��protect
template<class T>
class singleton_wrapper : public T {};

// ����ģʽ
template <class T>
class singleton
{
public:
    static T& get_instance()
    {
        // Ϊ���ܹ���main����ǰ��ʼ��s_pInstance
        s_pInstance;
        static singleton_wrapper<T> instance;
        return *static_cast<T *>(&instance);
    }

private:
    singleton() {}
    singleton(const singleton&) {}
    singleton& operator = (const singleton&) {}
    ~singleton() {}

    // ǿ�ƶ���ģʽ������д����Ҫ��Ϊ�˷�ֹ��ʼ����������
    static T* s_pInstance;
};

template<class T>
T* singleton<T>::s_pInstance = &singleton<T>::get_instance();

END_NSP_DDM
#endif // singleton_hpp_
