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

    /** ����ڴ��ַ��ʧ�ܷ���nullptr
    @param [in] offset ����ڿ�ʼ��λ��
    @return ����ӳ��ĵ�ַ��ʧ�ܷ���nullptr
    */
    void* get_buff(size_t offset = 0);
    void close();
private:
    /** ��һ�ε�ӳ��
    */
    void* m_pMapViewOfFile = nullptr;

    /** m_pMapViewOfFileӳ���λ�ã�һ���0��ʼ
    */
    size_t m_offset = 0;

    /** �ڴ�ӳ���������಻��ȥclose�����������ǽ����������������
    */
    HANDLE m_hMap = nullptr;

    friend shared_memory_manager;
};

using sp_shared_memory = std::shared_ptr<shared_memory>;
END_NSP_DDM
#endif // shared_memory_h_
