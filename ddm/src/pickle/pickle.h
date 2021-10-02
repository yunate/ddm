
#ifndef PIKLE_H_
#define PIKLE_H_

#include "g_def.h"
#include "ddassert.h"
#include "ddmath.h"
#include "traits.hpp"
#include "ddmacro.hpp"

BEG_NSP_DDM
class pickle_reader;
class pickle
{
    friend pickle_reader;
public:
    struct header
    {
        u32 size_after_head = 0; // 不包含头的大小
    };

    pickle();
    pickle(u32 capasize);
    ~pickle();

    pickle(const pickle& r);
    pickle& operator=(const pickle& r);

    pickle(pickle&& r) noexcept;
    pickle& operator=(pickle&& r) noexcept;

    void reserve(u32 size);
    u32 size() const;
    u32 capacity() const;

public:
    inline u8* append(u32 size)
    {
        DD_ASSERT(size != 0);
        if (m_header == nullptr) {
            reserve(size);
        }

        u32 align_size = align(size, (u32)sizeof(u32));
        if (align_size > m_capacity_after_head - m_header->size_after_head) {
            reserve(align_size + m_header->size_after_head);
        }

        u8* beg = ((u8*)(m_header)) + m_header->size_after_head + sizeof(header);
        m_header->size_after_head += align_size;
        return beg;
    }

    template<class T>
    inline pickle& append_pod(T data)
    {
        u32 size = sizeof(T);
        if (m_header == nullptr) {
            reserve(size);
        }

        u32 align_size = align(size, (u32)sizeof(u32));
        if (align_size > m_capacity_after_head - m_header->size_after_head) {
            reserve(align_size + m_header->size_after_head);
        }

        u8* beg = ((u8*)(m_header)) + m_header->size_after_head + sizeof(header);
        *((T*)(beg)) = data;
        m_header->size_after_head += align_size;
        return *this;
    }

    inline pickle& append_buff(const void* buff, u32 buff_Len)
    {
        DD_ASSERT(buff != nullptr);
        DD_ASSERT(buff_Len != 0);
        (*this).append_pod(buff_Len);
        u8* beg = this->append(buff_Len);
        if (beg != nullptr) {
            ::memcpy(beg, (u8*)buff, buff_Len);
        }
        return *this;
    }

protected:
    void free_buff();

protected:
    header* m_header = nullptr;
    u32 m_capacity_after_head = 0;
};

class pickle_reader
{
public:
    pickle_reader(const pickle* p);
    ~pickle_reader();

    pickle_reader(const pickle_reader& r);
    pickle_reader& operator=(const pickle_reader& r);

    pickle_reader(pickle_reader&& r) noexcept;
    pickle_reader& operator=(pickle_reader&& r) noexcept;

    inline u8* read_next(u32 size)
    {
        DD_ASSERT(m_pickle != nullptr);
        DD_ASSERT(m_pickle->m_header != nullptr);
        DD_ASSERT(m_pickle->size() != 0);

        if (m_pos + size > m_pickle->size()) {
            return nullptr;
        }

        u8* beg = ((u8*)(m_pickle->m_header)) + m_pos + sizeof(pickle::header);
        m_pos += align(size, (u32)sizeof(u32));
        return beg;
    }

    template<class T>
    bool read_next_pod(T& data)
    {
        DD_ASSERT(m_pickle != nullptr);
        DD_ASSERT(m_pickle->m_header != nullptr);
        DD_ASSERT(m_pickle->size() != 0);
        u32 size = sizeof(T);
        if (m_pos + size > m_pickle->size()) {
            return false;
        }

        u8* beg = ((u8*)(m_pickle->m_header)) + m_pos + sizeof(pickle::header);
        data = *((T*)(beg));
        m_pos += align(size, (u32)sizeof(u32));
        return true;
    }

    inline pickle_reader& read_buff(void* buff, u32 buff_len)
    {
        (buff_len);
        u32 len = 0;
        (void)read_next_pod(len);
        DD_ASSERT(buff_len >= len);
        u8* data = read_next(len);
        if (data != nullptr) {
            ::memcpy(buff, data, len);
        }

        return *this;
    }

    inline u8* read_buff(u32& buff_len)
    {
        (void)read_next_pod(buff_len);
        return read_next(buff_len);
    }

protected:
    const pickle* m_pickle = nullptr;
    u32 m_pos = 0;
};

template<class T, container_traits traits = _container_traits<T>>
class pickle_reader_writer_helper;

template<class T>
class pickle_reader_writer_helper<T, container_traits::container_traits_standard_layout>
{
public:
    static pickle& write(pickle& pck, const T& r)
    {
        static_assert(std::is_standard_layout<T>::value, "T is not pod");
        static_assert(!std::is_pointer<T>::value, "T should not be a pointer");
        pck.append_pod(r);
        return pck;
    }

    static pickle_reader& read(pickle_reader& reader, T& r)
    {
        static_assert(std::is_standard_layout<T>::value, "T is not pod");
        static_assert(!std::is_pointer<T>::value, "T should not be a pointer");
        (void)reader.read_next_pod(r);
        return reader;
    }
};

template<class T>
class pickle_reader_writer_helper<T, container_traits::container_traits_vector>
{
public:
    inline static pickle& write (pickle& pck, const T& r)
    {
        pck << (u32)r.size();
        for (const auto& it : r) {
            pck << it;
        }
        return pck;
    }

    inline static pickle_reader& read(pickle_reader& reader, T& r)
    {
        u32 size = 0;
        reader >> size;
        if (size != 0) {
            r.resize(size);
            for (u32 i = 0; i < size; ++i) {
                reader >> r[i];
            }
        }
        return reader;
    }
};

template<class T>
class pickle_reader_writer_helper<T, container_traits::container_traits_map>
{
public:
    inline static pickle& write (pickle& pck, const T& r)
    {
        pck << (u32)r.size();
        for (const auto& it : r) {
            pck << it.first;
            pck << it.second;
        }
        return pck;
    }
    inline static pickle_reader& read(pickle_reader& reader, T& r)
    {
        u32 size = 0;
        reader >> size;
        for (u32 i = 0; i < size; ++i) {
            T::key_type key;
            reader >> key;
            T::mapped_type val;
            reader >> val;
            r[key] = val;
        }
        return reader;
    }
};

template<class T>
inline pickle& operator << (pickle& pck, const T& r)
{
    return pickle_reader_writer_helper<T>::write(pck, r);
}

inline pickle& operator<<(pickle& pck, const std::string& r)
{
    return pck.append_buff(r.data(), (u32)r.size());
}

inline pickle& operator<<(pickle& pck, const char* r)
{
    pck << std::string(r);
    return pck;
}

inline pickle& operator<<(pickle& pck, const std::wstring& r)
{
    return pck.append_buff(r.data(), (u32)r.size() * sizeof(wchar_t));
}

inline pickle& operator<<(pickle& pck, const wchar_t* r)
{
    pck << std::wstring(r);
    return pck;
}

///////////////////////////// >> /////////////////////////////////////////////
template<class T>
inline pickle_reader& operator >> (pickle_reader& reader, T& r)
{
    return pickle_reader_writer_helper<T>::read(reader, r);
}

inline pickle_reader& operator>>(pickle_reader& reader, std::string& r)
{
    u32 size = 0;
    char* data = (char*)reader.read_buff(size);
    if (size != 0 && data != nullptr) {
        r.assign(data, size);
    }
    return reader;
}

inline pickle_reader& operator>>(pickle_reader& reader, std::wstring& r)
{
    u32 size = 0;
    wchar_t* data = (wchar_t*)reader.read_buff(size);
    if (size != 0 && data != nullptr) {
        r.assign(data, size / 2);
    }
    return reader;
}

#define DD_PICKLE_OPT_LL(a, idx) << r.a
#define DD_PICKLE_OPT_RR(a, idx) >> r.a
#define DD_PICKLE_OPT_BASE_STRUCT_TRAITS_W(a, idx) pickle_reader_writer_helper<a, _container_traits<a>>::write(pck, r);
#define DD_PICKLE_OPT_BASE_STRUCT_TRAITS_R(a, idx) pickle_reader_writer_helper<a, _container_traits<a>>::read(reader, r);

// 最多20个参数
// eg: DD_PICKLE_TRAITS_GEN_EX(StructName, DD_EXPEND(BaseStruct1, BaseStruct2), arg1, arg2)
#define DD_PICKLE_TRAITS_GEN_EX(StructName, BaseStructs, ...)                                                            \
template<>                                                                                                               \
class pickle_reader_writer_helper<StructName, _container_traits<StructName>>                                             \
{                                                                                                                        \
public:                                                                                                                  \
    static pickle& write(pickle& pck, const StructName& r)                                                               \
    {                                                                                                                    \
        r;                                                                                                               \
        DD_EACH(DD_PICKLE_OPT_BASE_STRUCT_TRAITS_W, BaseStructs)                                                         \
        pck DD_EACH(DD_PICKLE_OPT_LL, __VA_ARGS__);                                                                      \
        return pck;                                                                                                      \
    }                                                                                                                    \
                                                                                                                         \
    static pickle_reader& read(pickle_reader& reader, StructName& r)                                                     \
    {                                                                                                                    \
        r;                                                                                                               \
        DD_EACH(DD_PICKLE_OPT_BASE_STRUCT_TRAITS_R, BaseStructs)                                                         \
        reader DD_EACH(DD_PICKLE_OPT_RR, __VA_ARGS__);                                                                   \
        return reader;                                                                                                   \
    }                                                                                                                    \
};

// 最多20个参数
// eg: DD_PICKLE_TRAITS_GEN_EX(StructName, arg1, arg2)
#define DD_PICKLE_TRAITS_GEN(StructName, ...) DD_PICKLE_TRAITS_GEN_EX(StructName, DD_EXPEND(), __VA_ARGS__)
END_NSP_DDM
#endif // PIKLE_H_
