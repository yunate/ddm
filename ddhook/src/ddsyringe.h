#ifndef ddsyringe_h_
#define ddsyringe_h_

#include "ddinclude.h"
#include "windows/ddprocess.h"
#include <memory>
#include <windows.h>

using namespace NSP_DDM;

/** ����ע��
1.�򿪽��̣���ȡ���̵ľ����
2.�����ڴ�ռ俪��һ���ڴ�ռ�
3.����ոտ��ٵ��ڴ���д����Ҫע��DLL��·����
4.������GetProcessAddree()��ȡLoadLibrary�ĵ�ַ��
5.�ǵ���Զ���̣߳�����LoadLibrary����ȥ����DLL
*/
class ddclassic_syringe : private nocopyable
{
public:
    ddclassic_syringe(const std::shared_ptr<ddprocess>& process, const std::wstring& dllFullPath);
    ~ddclassic_syringe() = default;

    bool check_param();
    bool inject_dll(u32 waitTime = 5000);
    bool uninject_dll(u32 waitTime = 5000);

private:
    bool exec_remote(HANDLE hProcess, void* enterPoint, void* buff, u32 buffSize, u32 waitTime);
private:
    std::shared_ptr<ddprocess> m_process;
    std::wstring m_dllFullPath;
};

#endif