#include "file_stream.h"
#ifdef DD_WINDOW
#include <tchar.h>

BEG_NSP_DDM
file_stream::file_stream(const ddchar* const path, const ddchar* const mode/* = nullptr*/) : 
    m_hFile(nullptr)
{
    DD_ASSERT(path != nullptr);
    m_path = path;
    DD_ASSERT(m_path.length() > 0);

    if(mode == nullptr) {
        ::_tfopen_s(&m_hFile, m_path.c_str(), _DDT("a+b"));
    } else {
        ::_tfopen_s(&m_hFile, m_path.c_str(), mode);
    }
}

file_stream::file_stream(file_stream&& stream) noexcept
{
    m_path = stream.m_path;
    m_hFile = stream.m_hFile;
}

file_stream::~file_stream()
{
    if (m_hFile != nullptr) {
        ::fclose(m_hFile);
        m_hFile = nullptr;
    }
}

s64 file_stream::pos()
{
    if (m_hFile != nullptr) {
        return ::_ftelli64(m_hFile);
    } else {
        return -1;
    }
}

s64 file_stream::seek(s64 offset, int origin)
{
    if (m_hFile != nullptr) {
        ::_fseeki64(m_hFile, offset, origin);
        return pos();
    } else {
        return -1;
    }
}

s64 file_stream::resize(s64 newSize)
{
    newSize;
    return -1;
}

s32 file_stream::read(u8 * const buff, s32 count)
{
    DD_ASSERT(buff != nullptr);
    DD_ASSERT(count != 0);

    if (m_hFile != nullptr) {
        return (s32)::fread(buff, 1, count, m_hFile);
    } else {
        return -1;
    }
}

s32 file_stream::write(const u8 * const buff, s32 count)
{
    DD_ASSERT(buff != nullptr);
    DD_ASSERT(count != 0);

    if (m_hFile != nullptr) {
        return (s32)::fwrite(buff, 1, count, m_hFile);
    } else {
        return -1;
    }
}

END_NSP_DDM
#endif // DD_WINDOW
