
#include "file_reader.h"
#include "ddassert.h"
#ifdef DD_WINDOW
BEG_NSP_DDM
file_reader::file_reader(const ddstr& path) :
    m_pFile(nullptr), m_path(path) {}

file_reader::~file_reader()
{
    if (m_pFile != nullptr) {
        ::fclose(m_pFile);
        m_pFile = nullptr;
    }
}

bool file_reader::open(u8* checker, const u32 checkSize)
{
    bool res = false;

    do {
        if (m_pFile == nullptr) {
            if (0 != ::_wfopen_s(&m_pFile, m_path.c_str(), _DDT("rb"))) {
                break;
            }
        }

        if (m_pFile == nullptr) {
            break;
        }

        ::_fseeki64(m_pFile, 0, SEEK_SET);
        
        if (checker != nullptr) {
            bool check = true;
            for (u32 i = 0; i < checkSize; ++i) {
                int c = ::fgetc(m_pFile);
                if (c == EOF || c != checker[i]) {
                    check = false;
                    break;
                }
            }
            if (!check) {
                break;
            }
        }

        res = true;
    } while (0);

    if (m_pFile != nullptr && !res) {
        ::fclose(m_pFile);
        m_pFile = nullptr;
    }

    return res;
}

bool file_reader::getlinew(ddstrw& line)
{
    getuntilw([&line](ddcharw c) {
        line.append(1, (ddcharw)c);
        return c == L'\n';
    });

    return line.length() > 0;
}

bool file_reader::getlinea(ddstra& line)
{
    getuntila([&line](ddchara c) {
        line.append(1, (u8)c);
        return c == '\n';
    });

    return line.length() > 0;
}

void file_reader::getuntila(std::function<bool(ddchara ch)> until)
{
    DD_ASSERT(m_pFile != nullptr);
    ddchara c = 0;
    while ((c = (ddchara)::fgetc(m_pFile)) != EOF && !until(c));
}

void file_reader::getuntilw(std::function<bool(ddcharw ch)> until)
{
    DD_ASSERT(m_pFile != nullptr);
    ddcharw c = 0;
    while ((c = (ddcharw)::fgetwc(m_pFile)) != WEOF && !until(c));
}

u32 file_reader::get_buff(u8* buff, const u32& size)
{
    DD_ASSERT(m_pFile != nullptr);
    DD_ASSERT(buff != nullptr);
    DD_ASSERT(size != 0);

    u32 pos = 0;
    for (u32 i = 0; i < size; ++i) {
        int c = ::fgetc(m_pFile);
        if (c == EOF) {
            break;
        }

        buff[pos++] = static_cast<u8>(c);
    }

    return pos;
}

u64 file_reader::get_file_size() {
    DD_ASSERT(m_pFile != nullptr);
    u64 pos = ::_ftelli64(m_pFile);
    ::_fseeki64(m_pFile, 0, SEEK_END);
    u64 size = ::_ftelli64(m_pFile);
    ::_fseeki64(m_pFile, pos, SEEK_SET);
    return size;
}

END_NSP_DDM
#endif
