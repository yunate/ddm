#ifndef ddsyringe_h_
#define ddsyringe_h_

#include "g_def.h"
#include "nocopyable.hpp"

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
	ddclassic_syringe() = default;
	~ddclassic_syringe() = default;

	static bool inject_dll(HANDLE hProcess, const std::wstring& dllFullPath);
	static bool inject_dll(DWORD processID, const std::wstring& dllFullPath);
	static bool inject_dll(const std::wstring& processName, const std::wstring& dllFullPath);
};

#endif