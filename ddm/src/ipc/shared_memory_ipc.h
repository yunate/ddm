#ifndef shared_memory_ipc_h_
#define shared_memory_ipc_h_
#include "windows/ddshared_memory.h"
#include "single_limited.h"
#include "nocopyable.hpp"
BEG_NSP_DDM

class shared_memory_ipc_base
{
    DD_NO_COPY_MOVE(shared_memory_ipc_base);
public:
    virtual ~shared_memory_ipc_base();
    virtual bool create(const ddstr& name, u32 size);
    void close();
protected:
    ddshared_memory* m_shared_memory;
    HANDLE m_recv_event = NULL;
    HANDLE m_send_event = NULL;
    u32 m_max_size = 0;
    ddstr m_name;
};

class shared_memory_ipc_server : public shared_memory_ipc_base
{
public:
    virtual bool create(const ddstr& name, u32 size);
    // time_out 等于 0xFFFFFFFF 时候永不超时
    bool recv(ddbuff& buff, u32 time_out = 0xFFFFFFFF);

    // 获取读event，以便iocp
    HANDLE get_recv_event();

private:
    // 直接读取到buff中，暴露以便iocp
    bool read(ddbuff& buff);
private:
    single_limited m_single_limited;
};

class shared_memory_ipc_client : public shared_memory_ipc_base
{
public:
    // 如果size大于共享内存大小则返回false，由调用者切片
    // 如果size大于buff大小则crash
    bool send(const void* buff, u32 size, u32 time_out = 0xFFFFFFFF);
};
END_NSP_DDM
#endif
