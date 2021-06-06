#include "pickle.h"
#include <algorithm>
BEG_NSP_DDM

pickle_reader::pickle_reader(const pickle* p) : m_pickle(p) { }

pickle_reader::~pickle_reader() 
{ 
    m_pickle = nullptr;
    m_pos = 0;
}

pickle_reader::pickle_reader(const pickle_reader& r) { *this = r; }

pickle_reader& pickle_reader::operator=(const pickle_reader& r)
{
    m_pickle = r.m_pickle;
    m_pos = r.m_pos;
    return *this;
}

pickle_reader::pickle_reader(pickle_reader&& r) noexcept : m_pickle(r.m_pickle), m_pos(r.m_pos)
{
    *this = r;
}

pickle_reader& pickle_reader::operator=(pickle_reader&& r) noexcept
{
    m_pickle = r.m_pickle;
    m_pos = r.m_pos;
    r.m_pickle = nullptr;
    r.m_pos = 0;
    return *this;
}
////////////////////////////pickle_reader end//////////////////////////////////////////////

pickle::pickle()
{
    try {
        reserve(64 + sizeof(header));
        m_header->size_after_head = 0;
    } catch(const std::exception&) { }
}

pickle::pickle(u32 capasize)
{
    try {
        reserve(capasize + sizeof(header));
        m_header->size_after_head = 0;
    }
    catch (const std::exception&) { }
}

pickle::~pickle()
{
    free_buff();
}

pickle::pickle(const pickle& r)
{
    *this = r;
}

pickle& pickle::operator=(const pickle& r)
{
    if (r.size() == 0) {
        free_buff();
        return *this;
    }

    try
    {
        reserve(r.size());
        m_header->size_after_head = r.size();
        ::memcpy(((char*)m_header) + sizeof(header), ((char*)r.m_header) + sizeof(header), r.size());
    }
    catch (const std::exception&) {}
    return *this;
}

pickle::pickle(pickle&& r) noexcept
{
    *this = r;
}

pickle& pickle::operator=(pickle&& r) noexcept
{
    free_buff();
    m_header = r.m_header;
    r.m_header = nullptr;
    return *this;
}

void pickle::reserve(u32 size)
{
    if (size == 0) {
        free_buff();
        return;
    }

    size = align(size, 64);
    const u32 MAX_DEL = 4096;
    if (size > m_capacity_after_head) {
        size += std::min(m_capacity_after_head / 2, (u32)MAX_DEL);
    }

    if (size > MAX_DEL) {
        size = align(size, MAX_DEL);
    }

    void* p = ::realloc(m_header, size);
    if (p != nullptr) {
        m_header = reinterpret_cast<header*>(p);
        m_capacity_after_head = size - sizeof(header);
    } else {
        throw std::bad_alloc();
    }
}

void pickle::free_buff()
{
    if (m_header != nullptr) {
        ::free(m_header);
        m_header = nullptr;
        m_capacity_after_head = 0;
    }
}

u32 pickle::capacity() const
{
    if (m_header == nullptr) {
        return 0;
    }

    return m_capacity_after_head;
}

u32 pickle::size() const
{
    if (m_header == nullptr) {
        return 0;
    }

    return m_header->size_after_head;
}
END_NSP_DDM
