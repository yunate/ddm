#ifndef shared_memory_h_
#define shared_memory_h_ 1

#include "g_def.h"
#include "nocopyable.hpp"
#include <memory>
#include <Windows.h>

BEG_NSP_DDM

class shared_memory_manager;

class shared_memory : public nocopyable
{
private:
    shared_memory(){}

public:
    ~shared_memory();

    /** 获得内存地址，失败返回nullptr
    @param [in] offset 相对于开始的位置
    @return 返回映射的地址，失败返回nullptr
    */
    void* get_buff(size_t offset = 0);
    void close();
private:
    /** 上一次的映射
    */
    void* m_pMapViewOfFile = nullptr;

    /** m_pMapViewOfFile映射的位置，一般从0开始
    */
    size_t m_offset = 0;

    /** 内存映射句柄，本类不会去close这个句柄，而是将他交给管理类管理
    */
    HANDLE m_hMap = nullptr;

    friend shared_memory_manager;
};

using sp_shared_memory = std::shared_ptr<shared_memory>;
END_NSP_DDM
#endif // shared_memory_h_
