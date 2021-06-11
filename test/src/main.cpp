
#include "test_case_factory.h"

BEG_NSP_DDM

int test_main()
{
    TCF.insert_white_type("test_pickle");
    TCF.run();
    ::system("pause");
    return 0;
}

END_NSP_DDM



#include <functional>
#include <iostream>

template<class R, class ...V>
struct SS {};

int Fun0()
{
    std::cout << "Fun0" << std::endl;
    return 0;
}

int Fun1(int)
{
    std::cout << "Fun1" << std::endl;
    return 0;
}

int Fun2(int, int)
{
    std::cout << "Fun2" << std::endl;
    return 0;
}


template<class R, class ...A>
auto MBind(R(*fun)(A...))
{
    return [fun](A... a) {
        return fun(a...);
    };
}

class AA
{
public:
    void Fun(int t) 
    {
        std::cout << "AA Fun" << std::endl;
    }
};
typedef AA AAA;

template<class F>
class IPCCluser
{
public:
    template<class F>
    IPCCluser(F callable) : m_callable(callable)
    {

    }
    template<class ...A>
    auto operator()(A... a)
    {
        return m_callable(a...);
    }

    IPCCluser& operator*()
    {
        return *this;
    }

private:
   F m_callable;
};

template<class F>
auto BindCluser(F callable)
{
    return IPCCluser<F>(callable);
}

template<class O, class R, class ...A>
auto MBind(O* obj, R(O::*fun)(A...))
{
    auto it = [fun, obj](A... a) {
        return (obj->*fun)(a...);
    };
    return it;
}

int main()
{
    // std::bind(Fun);
//     auto t0 = MBind(Fun0);
//     auto t1 = MBind(Fun1);
//     auto t2 = MBind(Fun2);
//     t0();
//     t1(1);
//     t2(1, 2);
    auto it = []() {};
    it();
    AA a;
    auto t3 = MBind(&a, &AA::Fun);
    auto cluser = BindCluser(t3);
    (*cluser)(1);
    ///*ittt();
    return ddm::test_main();
}

