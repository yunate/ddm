#include "shared_memory_manager.h"
#ifdef DD_WINDOW

BEG_NSP_DDM
shared_memory_manager::shared_memory_manager() {}

shared_memory_manager::~shared_memory_manager()
{
    for (auto it : m_handleMap) {
        ::CloseHandle(it.second);
    }

    m_handleMap.clear();
}

sp_shared_memory shared_memory_manager::create(const ddstr& name, size_t size)
{
    sp_shared_memory spSharedMemory = open(name);

    if (spSharedMemory != nullptr) {
        return spSharedMemory;
    }

    spSharedMemory.reset(new (std::nothrow) shared_memory);
    if (spSharedMemory == nullptr || name.empty()) {
        return nullptr;
    }

    spSharedMemory->m_hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
                              nullptr,
                              PAGE_READWRITE,
                              0,
                              (DWORD)size,
                              (LPCWSTR)name.c_str());

    // 如果创建失败的话，尝试创建文件映射
    if (spSharedMemory->m_hMap == nullptr) {

        if (m_tmpDir.length() == 0) {
            WCHAR tmpPath[MAX_PATH + 1] = { 0 };
            if (0 == ::GetTempPath(MAX_PATH + 1, tmpPath)) {
                return nullptr;
            }

            m_tmpDir = tmpPath;
        }

        ddstr path = m_tmpDir + name;

        // 打开文件句柄
        HANDLE hFile = ::CreateFile(path.c_str(),
                                    GENERIC_READ | GENERIC_WRITE,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    nullptr,
                                    CREATE_ALWAYS,
                                    FILE_FLAG_OVERLAPPED,
                                    nullptr);

        if (INVALID_HANDLE_VALUE != hFile) {
            spSharedMemory->m_hMap = ::CreateFileMapping(hFile,
                                        nullptr, 
                                        PAGE_READWRITE,
                                        0,
                                        (DWORD)size,
                                        (LPCWSTR)name.c_str());
            ::CloseHandle(hFile);
        }
    }

    if (spSharedMemory->m_hMap == nullptr) {
        return nullptr;
    }

    m_handleMap[name] = spSharedMemory->m_hMap;
    return spSharedMemory;
}

void shared_memory_manager::close(const ddstr& name)
{
    auto it = m_handleMap.find(name);
    if (it != m_handleMap.end()) {
        ::CloseHandle(it->second);
        m_handleMap.erase(it);
    }
}

sp_shared_memory shared_memory_manager::open(const ddstr& name)
{
    sp_shared_memory spSharedMemory(new (std::nothrow) shared_memory);
    if (spSharedMemory == nullptr || name.empty()) {
        return nullptr;
    }

    spSharedMemory->m_hMap = find(name);

    if (spSharedMemory->m_hMap != nullptr) {
        return spSharedMemory;
    }

    spSharedMemory->m_hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, (LPCWSTR)name.c_str());

    if (spSharedMemory->m_hMap == nullptr) {
        return nullptr;
    }

    m_handleMap[name] = spSharedMemory->m_hMap;
    return spSharedMemory;
}

void NSP_DDM::shared_memory_manager::SetTmpDir(const ddstr& tmpDir)
{
    m_tmpDir = tmpDir;
}

HANDLE shared_memory_manager::find(const ddstr& name)
{
    auto it = m_handleMap.find(name);

    if (it == m_handleMap.end()) {
        return nullptr;
    }

    return it->second;
}
END_NSP_DDM
#endif // DD_WINDOW
