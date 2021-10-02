#ifndef ddsyringe_h_
#define ddsyringe_h_

#include "ddinclude.h"
#include "windows/ddprocess.h"
#include <memory>
#include <windows.h>

using namespace NSP_DDM;

/** 经典注入
1.打开进程，获取进程的句柄，
2.是在内存空间开辟一段内存空间
3.是向刚刚开辟的内存中写入需要注入DLL的路径，
4.是利用GetProcessAddree()获取LoadLibrary的地址。
5.是调用远程线程，利用LoadLibrary（）去加载DLL
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