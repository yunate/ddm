#ifndef shared_memory_manager_h_
#define shared_memory_manager_h_ 1

#include "shared_memory.h"

#include "singleton.hpp"
#include "nocopyable.hpp"

#include <string>
#include <unordered_map>
#include <Windows.h>

BEG_NSP_DDM
class shared_memory_manager : public nocopyable
{
protected:
    shared_memory_manager();
    ~shared_memory_manager();

public:
    /** 创建共享内存，如果已经存在则打开
    @param [in] name 共享内存名称，为空时失败
    @param [in] size 大小
    @return 映射句柄，失败返回nullptr
    */
    sp_shared_memory create(const ddstr& name, size_t size);
    void close(const ddstr& name);

    /** 打开共享内存，如果不存在则打开失败
    @param [in] name 共享内存名称，为空时失败
    @return 映射句柄，失败返回nullptr
    */
    sp_shared_memory open(const ddstr& name);

    // 设置临时目录路径，用于创建共享内存失败时候创建文件地方
    // 如果没有设置，默认为%tmp%路径
    void SetTmpDir(const ddstr& tmpDir);

    /** 查找map是否已经存在了
    @param [in] name 共享内存名称，为空时失败
    @return 映射句柄，不存在返回nullptr
    */
    HANDLE find(const ddstr& name);

private:
    /** 内存映射句柄和他的名称的map
    */
    std::unordered_map<ddstr, HANDLE> m_handleMap;

    // 临时目录
    ddstr m_tmpDir;
};

// 单例对象
#define SHARED_MEMORY_MANAGER (singleton<shared_memory_manager>::get_instance())

END_NSP_DDM
#endif // shared_memory_manager_h_

