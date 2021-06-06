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
    /** ���������ڴ棬����Ѿ��������
    @param [in] name �����ڴ����ƣ�Ϊ��ʱʧ��
    @param [in] size ��С
    @return ӳ������ʧ�ܷ���nullptr
    */
    sp_shared_memory create(const ddstr& name, size_t size);
    void close(const ddstr& name);

    /** �򿪹����ڴ棬������������ʧ��
    @param [in] name �����ڴ����ƣ�Ϊ��ʱʧ��
    @return ӳ������ʧ�ܷ���nullptr
    */
    sp_shared_memory open(const ddstr& name);

    // ������ʱĿ¼·�������ڴ��������ڴ�ʧ��ʱ�򴴽��ļ��ط�
    // ���û�����ã�Ĭ��Ϊ%tmp%·��
    void SetTmpDir(const ddstr& tmpDir);

    /** ����map�Ƿ��Ѿ�������
    @param [in] name �����ڴ����ƣ�Ϊ��ʱʧ��
    @return ӳ�����������ڷ���nullptr
    */
    HANDLE find(const ddstr& name);

private:
    /** �ڴ�ӳ�������������Ƶ�map
    */
    std::unordered_map<ddstr, HANDLE> m_handleMap;

    // ��ʱĿ¼
    ddstr m_tmpDir;
};

// ��������
#define SHARED_MEMORY_MANAGER (singleton<shared_memory_manager>::get_instance())

END_NSP_DDM
#endif // shared_memory_manager_h_

