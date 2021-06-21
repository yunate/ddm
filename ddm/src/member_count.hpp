
#ifndef member_count_hpp
#define member_count_hpp
#include "g_def.h"

BEG_NSP_DDM
struct member_count_any_type
{
    template <typename T>
    operator T();
};

template <typename T, typename = void, typename ...Ts>
struct member_count {
    constexpr static size_t value = sizeof...(Ts) - 1;
};

template <typename T, typename ...Ts>
struct member_count < T, std::void_t<decltype(T{ Ts{}... }) > , Ts... > {
    constexpr static size_t value = member_count<T, void, Ts..., member_count_any_type>::value;
};
END_NSP_DDM
#endif


