
#ifndef PIKLE_H_
#define PIKLE_H_

#include "g_def.h"
#include "ddmath.h"
#include "traits.hpp"
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

protected:
    const pickle* m_pickle = nullptr;
    u32 m_pos = 0;
};

template<class T, container_traits traits = _container_traits<T>>
class pickle_reader_writer_helper;

template<class T>
class pickle_reader_writer_helper<T, container_traits::container_traits_pod>
{
public:
    static pickle& write(pickle& pck, const T& r)
    {
        static_assert(std::is_pod<T>::value, "T is not pod");
        static_assert(!std::is_pointer<T>::value, "T should not be a pointer");
        pck.append_pod(r);
        return pck;
    }

    static pickle_reader& read(pickle_reader& reader, T& r)
    {
        static_assert(std::is_pod<T>::value, "T is not pod");
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
    pck << (u32)r.size();
    u8* beg = pck.append((u32)r.size());
    if (beg != nullptr) {
        ::memcpy(beg, (u8*)r.data(), r.size());
    }
    return pck;
}

inline pickle& operator<<(pickle& pck, const char* r)
{
    pck << std::string(r);
    return pck;
}

inline pickle& operator<<(pickle& pck, const std::wstring& r)
{
    pck << (u32)r.size();
    u8* beg = pck.append((u32)r.size() * sizeof(wchar_t));
    if (beg != nullptr) {
        ::memcpy(beg, (u8*)r.data(), r.size());
    }
    return pck;
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
    reader >> size;
    if (size != 0) {
        char* data = (char*)reader.read_next(size);
        if (data != nullptr) {
            r.assign(data, size);
        }
    }
    return reader;
}

inline pickle_reader& operator>>(pickle_reader& reader, std::wstring& r)
{
    u32 size = 0;
    reader >> size;
    if (size != 0) {
        wchar_t* data = (wchar_t*)reader.read_next(size * sizeof(wchar_t));
        if (data != nullptr) {
            r.assign(data, size);
        }
    }
    return reader;
}

END_NSP_DDM
#endif // PIKLE_H_
