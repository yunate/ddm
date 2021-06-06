
#include "shared_memory.h"
#ifdef DD_WINDOW
BEG_NSP_DDM
shared_memory::~shared_memory()
{
    close();
}

void shared_memory::close()
{
    if (m_pMapViewOfFile != nullptr) {
        ::UnmapViewOfFile(m_pMapViewOfFile);
        m_pMapViewOfFile = nullptr;
    }
}

void* shared_memory::get_buff(size_t offset /* = 0 */)
{
    if (m_hMap == nullptr) {
        return nullptr;
    }

    if (m_pMapViewOfFile != nullptr && offset == m_offset)
    {
        return m_pMapViewOfFile;
    } 

    if (m_pMapViewOfFile) {
        ::UnmapViewOfFile(m_pMapViewOfFile);
        m_pMapViewOfFile = nullptr;
    }

    m_offset = offset;
    m_pMapViewOfFile = ::MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, (DWORD)offset, 0);
    return m_pMapViewOfFile;
}
END_NSP_DDM

#endif // DD_WINDOW
