#ifndef ddsyringe_h_
#define ddsyringe_h_

#include "g_def.h"
#include "nocopyable.hpp"

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
	ddclassic_syringe() = default;
	~ddclassic_syringe() = default;

	static bool inject_dll(HANDLE hProcess, const std::wstring& dllFullPath);
	static bool inject_dll(DWORD processID, const std::wstring& dllFullPath);
	static bool inject_dll(const std::wstring& processName, const std::wstring& dllFullPath);
};

#endif