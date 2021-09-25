
#include "memory_stream.h"
#include "ddassert.h"
#include "ddmath.h"
#include <malloc.h>
#include <memory.h>

BEG_NSP_DDM
memory_stream::memory_stream(){}

memory_stream::memory_stream(u32 size)
{
    if (resize(size) >= size) {
        m_size = size;
    }
}

memory_stream::memory_stream(const s8* const pBuff, u32 size)
{
    DD_ASSERT(pBuff != nullptr);
    DD_ASSERT(size != 0);
    if (resize(size) >= size) {
        ::memcpy(m_pBuff, pBuff, (size_t)size);
        m_size = size;
    }
}

memory_stream::memory_stream(const i_ddstream& stream)
{
    *this = stream;
}

memory_stream::memory_stream(const memory_stream & stream)
{
    *this = stream;
}

memory_stream::memory_stream(memory_stream&& stream)
{
    this->m_pBuff = stream.m_pBuff;
    stream.m_pBuff = nullptr;
    this->m_pos = stream.m_pos;
    stream.m_pos = 0;
    this->m_size = stream.m_size;
    stream.m_size = 0;
    this->m_capacity = stream.m_capacity;
    stream.m_capacity = 0;
}

memory_stream & memory_stream::operator=(const i_ddstream& stream)
{
    if (this == &stream) {
        return *this;
    }

    if (m_pBuff) {
        delete m_pBuff;
        m_pBuff = nullptr;
    }

    m_pos = 0;
    m_size = 0;
    m_capacity = 0;
    m_pBuff = 0;
    memory_stream& r = (memory_stream&)(stream);
    s64 size = r.size();
    DD_ASSERT(size <= MAX_U32);

    if (resize(size) >= size) {
        s64 pos = r.pos();
        r.readalla((u8*)m_pBuff);
        r.seek(pos, 0);
        m_size = (u32)size;
    }

    return *this;
}

memory_stream & memory_stream::operator=(const memory_stream & stream)
{
    return *this = *((i_ddstream*)(&stream));
}

memory_stream::~memory_stream()
{
    if (m_pBuff) {
        ::free(m_pBuff);
        m_pBuff = nullptr;
    }
}

s64 memory_stream::pos()
{
    return m_pos;
}

s64 memory_stream::seek(s64 offset, int origin)
{
    DD_ASSERT(offset <= MAX_U32);

    switch (origin)
    {
    case SEEK_END: {
            m_pos = m_size - (u32)offset;
            break;
        }
    case SEEK_SET: {
            m_pos = (u32)offset;
            break;
        }
    default: {
            // SEEK_CUR 和其他
            m_pos += (u32)offset;
            break;
        }
    }

    if (m_pos > m_size) {
        m_pos = m_size;
    }

    if (m_pos < 0) {
        m_pos = 0;
    }

    return m_pos;
}

s64 memory_stream::size()
{
    return m_size;
}

s64 memory_stream::resize(s64 newSize)
{
    DD_ASSERT(newSize >= 0 && newSize <= MAX_U32);

    if ((u32)newSize > m_capacity) {
        if (realloc_memory((u32)newSize) >= (u32)newSize) {
            m_size = (u32)newSize;
        }
    } else {
        m_size = (u32)newSize;
    }
 
    return m_size;
}

u32 memory_stream::capacity()
{
    return m_capacity;
}

void memory_stream::clear() {
    m_size = 0;
    m_pos = 0;
    ::memset(m_pBuff, 0, (size_t)m_capacity);
}

s32 memory_stream::read(u8* const buff, s32 count)
{
    if (m_pos >= m_size) {
        return 0;
    }

    u32 endPos = count + m_pos;
    DD_ASSERT(endPos >= m_pos && buff != nullptr);

    if (endPos > m_size) {
        count = (u32)(m_size - m_pos);
        endPos = m_size;
    }

    ::memcpy(buff, m_pBuff + m_pos, count);
    m_pos = endPos;
    return count;
}

s32 memory_stream::write(const u8 * const buff, s32 count)
{
    u32 endPos = count + m_pos;
    DD_ASSERT(endPos >= m_pos && buff != nullptr);

    if (endPos > m_size) {
        if (resize(endPos) < endPos) {
            return 0;
        }
    }

    ::memcpy(m_pBuff + m_pos, buff, count);
    m_pos = endPos;
    return count;
}

u32 memory_stream::realloc_memory(u32 newSize)
{
    DD_ASSERT(newSize >=0);

    // 计算新的大小
    u32 calSize = cal_new_size(newSize);

    if (calSize < newSize) {
        return m_capacity;
    }

    void* pNewBuff = ::malloc((size_t)calSize);

    if (pNewBuff == nullptr) {
        return m_capacity;
    }

    if (m_pBuff != nullptr) {
        ::memcpy(pNewBuff, m_pBuff, (size_t)m_capacity);
        ::free(m_pBuff);
    }

    m_capacity = calSize;
    m_pBuff = (s8*)pNewBuff;
    return m_capacity;
}

u32 memory_stream::cal_new_size(u32 newSize)
{
    if (newSize < m_capacity + m_capacity / 2) {
        newSize = m_capacity + m_capacity / 2;
    }

    return get_min_upper(newSize, MET_DATA_SIZE);
}
END_NSP_DDM
