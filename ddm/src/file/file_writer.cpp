
#include "file_writer.h"
#include "ddassert.h"
#ifdef DD_WINDOW
BEG_NSP_DDM

file_writer::file_writer(const ddstr& path) :
    m_pFile(nullptr), m_path(path) {}

file_writer::~file_writer()
{
    if (m_pFile != nullptr) {
        ::fclose(m_pFile);
        m_pFile = nullptr;
    }
}

bool file_writer::open(u8* checker, const u32 checkSize)
{
    bool res = false;

    do {
        if (m_pFile == nullptr) {
            if (0 != ::_wfopen_s(&m_pFile, m_path.c_str(), _DDT("wb"))) {
                break;
            }
        }

        if (m_pFile == nullptr) {
            break;
        }

        ::_fseeki64(m_pFile, 0, SEEK_SET);

        if (checker != nullptr &&
            checkSize != write_buffa((ddchara*)checker, checkSize)) {
            break;
        }

        res = true;
    } while (0);

    if (m_pFile != nullptr && !res) {
        ::fclose(m_pFile);
        m_pFile = nullptr;
    }

    return res;
}

u32 file_writer::write_buffw(const ddcharw* buff, const u32 size)
{
    DD_ASSERT(m_pFile != nullptr);
    DD_ASSERT(buff != nullptr);
    DD_ASSERT(size != 0);

    return (u32)::fwrite(buff, 2, size, m_pFile);
}

u32 file_writer::write_buffa(const ddchara* buff, const u32 size)
{
    DD_ASSERT(m_pFile != nullptr);
    DD_ASSERT(buff != nullptr);
    DD_ASSERT(size != 0);

    return (u32)::fwrite(buff, 1, size, m_pFile);
}

u64 file_writer::get_file_size() {
    DD_ASSERT(m_pFile != nullptr);
    u64 pos = ::_ftelli64(m_pFile);
    ::_fseeki64(m_pFile, 0, SEEK_END);
    u64 size = ::_ftelli64(m_pFile);
    ::_fseeki64(m_pFile, pos, SEEK_SET);
    return size;
}

END_NSP_DDM
#endif
