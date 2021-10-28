#ifndef ddshared_memory_h_
#define ddshared_memory_h_

#include "g_def.h"
#include "nocopyable.hpp"
#include "windows/ddprocess.h"

#include <atomic>
#include <windows.h>

BEG_NSP_DDM
class ddasync_with_resize_shared_memory;
class ddshared_memory : public nocopyable
{
    friend ddasync_with_resize_shared_memory;
public:
    ddshared_memory();
    ~ddshared_memory();

    u8* get_buff();
    bool init(u32 size, const std::wstring& name);
    void close();

protected:
    u32 m_size = 0;
    std::wstring m_name;

protected:
    void* m_mapview = nullptr;
    HANDLE m_map = nullptr;
};

class ddasync_shared_memory : public nocopyable
{
    friend ddasync_with_resize_shared_memory;
public:
    ddasync_shared_memory();
    ~ddasync_shared_memory();
    bool init(u32 size, const std::wstring& name);
    u8* lockbuff(u32 waitTime = 3000);
    void unlock();
    void close();

protected:
    ddshared_memory* m_shared_memory = nullptr;
    ddprocess_mutex* m_mutex = nullptr;
};

END_NSP_DDM
#endif

