#ifndef TRAITS_HPP_
#define TRAITS_HPP_
#include "g_def.h"
#include <vector>
#include <list>
#include <deque>
#include <map>
//#include <hash_map>
#include <unordered_map>
#include <set>
#include <unordered_set>
//#include <hash_set>
#include <stack>
#include <queue>
BEG_NSP_DDM

enum class container_traits
{
    container_traits_none,
    container_traits_pod,
    container_traits_vector,
    container_traits_map,
    container_traits_set,
    container_traits_stack,
    container_traits_queue
};
template<class, class pod_dummy = void>
constexpr container_traits _container_traits = container_traits::container_traits_none;

template<class T>
constexpr container_traits _container_traits<T, std::enable_if_t<std::is_pod_v<T>, void>> = container_traits::container_traits_pod;

template<class T>
constexpr container_traits _container_traits<std::vector<T>> = container_traits::container_traits_vector;
template<class T>
constexpr container_traits _container_traits<std::list<T>> = container_traits::container_traits_vector;
template<class T>
constexpr container_traits _container_traits<std::deque<T>> = container_traits::container_traits_vector;

template<class K, class V>
constexpr container_traits _container_traits<std::map<K, V>> = container_traits::container_traits_map;
template<class K, class V>
constexpr container_traits _container_traits<std::unordered_map<K, V>> = container_traits::container_traits_map;
//template<class K, class V>
//constexpr container_traits _container_traits<std::hash_map<K, V>> = container_traits::container_traits_map;

template<class K, class V>
constexpr container_traits _container_traits<std::set<K, V>> = container_traits::container_traits_set;
template<class K, class V>
constexpr container_traits _container_traits<std::unordered_set<K, V>> = container_traits::container_traits_set;
//template<class K, class V>
//constexpr container_traits _container_traits<std::hash_set<K, V>> = container_traits::container_traits_set;

template<class T>
constexpr container_traits _container_traits<std::stack<T>> = container_traits::container_traits_stack;

template<class T>
constexpr container_traits _container_traits<std::queue<T>> = container_traits::container_traits_queue;
////////////////////////////////////////////////////container_traits end ////////////////////////////////////////////////////////////////////
END_NSP_DDM
#endif // !TRAITS_HPP_
