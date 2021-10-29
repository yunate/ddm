#ifndef ddsyringe_h_
#define ddsyringe_h_

#include "ddinclude.h"
#include "windows/ddprocess.h"
#include <memory>
#include <windows.h>

using namespace NSP_DDM;
class ddsyringe
{
public:
    // ��� process �� dllFullPath �Ƿ�Ϸ�
    static bool check_param(const std::shared_ptr<ddprocess>& process, const std::wstring& dllFullPath);

    // Զ��ִ��һ�δ���
    static bool exec_remote(HANDLE hProcess, void* enterPoint, void* buff, u32 buffSize, u32 waitTime);

    // ж�ص�ע���dll
    static bool uninject_dll(const std::shared_ptr<ddprocess>& process, const std::wstring& dllFullPath, u32 waitTime = 5000);
};

/** ����ע��
1.�򿪽��̣���ȡ���̵ľ����
2.�����ڴ�ռ俪��һ���ڴ�ռ�
3.����ոտ��ٵ��ڴ���д����Ҫע��DLL��·����
4.������GetProcessAddree()��ȡLoadLibrary�ĵ�ַ��
5.�ǵ���Զ���̣߳�����LoadLibrary����ȥ����DLL
*/
class ddclassic_syringe
{
    DD_NO_COPY_MOVE(ddclassic_syringe);
public:
    ddclassic_syringe(const std::shared_ptr<ddprocess>& process, const std::wstring& dllFullPath);
    ~ddclassic_syringe() = default;

    bool inject_dll(u32 waitTime = 5000);
    bool uninject_dll(u32 waitTime = 5000);

private:
    std::shared_ptr<ddprocess> m_process;
    std::wstring m_dllFullPath;
};

/** ����ע��
ע���ʱ��Ŀ����ں������ǰ5���ֽ��õ�ǰ���滻������ֹĿ����̶�Load Library������HOOK
�����dll�Ƿ�ǩ������ע�벻��
*/
class ddclassic_syringeex
{
    DD_NO_COPY_MOVE(ddclassic_syringeex);
public:
    ddclassic_syringeex(const std::shared_ptr<ddprocess>& process, const std::wstring& dllFullPath);
    ~ddclassic_syringeex() = default;

    bool inject_dll(u32 waitTime = 5000);
    bool uninject_dll(u32 waitTime = 5000);

private:
    std::shared_ptr<ddprocess> m_process;
    std::wstring m_dllFullPath;
};

/** apc ע��
1.�򿪽��̣���ȡ���̵ľ����
2.�����ڴ�ռ俪��һ���ڴ�ռ�
3.����ոտ��ٵ��ڴ���д����Ҫע��DLL��·��
4.�����ý����е������̣߳���ÿһ���̵߳�apc��ѹ�� LoadLibrary
5.�ȴ��̵߳���apc
@note ���ڲ�֪���̺߳�ʱ�����apc��Ϊ�������������ÿ���߳���ѹ��apcִ�к���������һ��������������ڴ��
      �ò����ͷţ�Ҳ��֪����ʱ��ж��ע��
*/
class ddapc_syringe
{
    DD_NO_COPY_MOVE(ddapc_syringe);
public:
    ddapc_syringe(const std::shared_ptr<ddprocess>& process, const std::wstring& dllFullPath);
    ~ddapc_syringe() = default;

    bool inject_dll(bool all);
    bool uninject_dll(u32 waitTime = 5000);

private:
    std::shared_ptr<ddprocess> m_process;
    std::wstring m_dllFullPath;
};
#endif