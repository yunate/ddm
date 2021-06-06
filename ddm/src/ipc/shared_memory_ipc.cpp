
#include "shared_memory_ipc.h"
BEG_NSP_DDM

shared_memory_ipc_base::~shared_memory_ipc_base()
{
    close();
}

bool shared_memory_ipc_base::create(const ddstr& name, u32 size)
{
    m_name = name;
    do {
        // 本程序已经打开了该名称的共享内存
        if (SHARED_MEMORY_MANAGER.find(name) != NULL) {
            return false;
        }

        m_shared_memory = SHARED_MEMORY_MANAGER.create(name + _DDT("_shared_memory_ipc_memory"), size);
        if (m_shared_memory == NULL) {
            return false;
        }

        if (m_shared_memory->get_buff(0) == NULL) {
            break;
        }

        ddstr recv_event_name = name + _DDT("_shared_memory_ipc_recv_event");
        m_recv_event = ::CreateEvent(NULL, false, false, recv_event_name.c_str());
        if (m_recv_event == NULL) {
            break;
        }

        ddstr send_event_name = name + _DDT("_shared_memory_ipc_send_event");
        m_send_event = ::CreateEvent(NULL, false, true, send_event_name.c_str());
        if (m_send_event == NULL) {
            break;
        }

        m_max_size = size;
        return true;
    } while (0);
    
    close();
    return false;
}

void shared_memory_ipc_base::close()
{
    if (m_recv_event != NULL) {
        ::CloseHandle(m_recv_event);
        m_recv_event = NULL;
    }

    if (m_send_event != NULL) {
        ::CloseHandle(m_send_event);
        m_send_event = NULL;
    }

    if (m_shared_memory != nullptr) {
        m_shared_memory.reset();
    }
    SHARED_MEMORY_MANAGER.close(m_name + _DDT("_shared_memory_ipc_memory"));
}

bool shared_memory_ipc_server::create(const ddstr& name, u32 size)
{
    if (m_single_limited.try_hold_mutex(name + _DDT("_shared_memory_ipc_sever_limited"))) {
        return shared_memory_ipc_base::create(name, size);
    }
    
    return false;
}

bool shared_memory_ipc_server::recv(ddbuff& buff, u32 time_out /*= 0xFFFFFFFF*/)
{
    if (m_shared_memory == nullptr) {
        return false;
    }

    DWORD hr = ::WaitForSingleObject(m_recv_event, time_out);
    if (hr != WAIT_OBJECT_0) {
        return false;
    }

    return read(buff);
}

bool shared_memory_ipc_server::read(ddbuff& buff)
{
    char* mem_buff = (char*)m_shared_memory->get_buff();
    if (mem_buff == nullptr) {
        return false;
    }

    u32 size = *((u32*)mem_buff);
    if (size + sizeof(u32) > m_max_size) {
        return false;
    }

    buff.resize(size);
    (void)::memcpy_s(buff.data(), size, (mem_buff + sizeof(u32)), size);
    (void)::SetEvent(m_send_event);
    return true;
}

HANDLE shared_memory_ipc_server::get_recv_event()
{
    return m_recv_event;
}

bool shared_memory_ipc_client::send(const void* buff, u32 size, u32 time_out /*= 0xFFFFFFFF*/)
{
    DWORD hr = ::WaitForSingleObject(m_send_event, time_out);
    if (hr != WAIT_OBJECT_0) {
        return false;
    }

    char* mem_buff = (char*)m_shared_memory->get_buff();
    if (mem_buff == nullptr) {
        return false;
    }

    if (size + sizeof(u32) > m_max_size) {
        return false;
    }

    *((u32*)mem_buff) = size;
    (void)::memcpy_s((mem_buff + sizeof(u32)), size, buff, size);
    (void)::SetEvent(m_recv_event);
    return true;
}

END_NSP_DDM
