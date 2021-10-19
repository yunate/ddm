#ifndef ddcom_tuils_h_
#define ddcom_tuils_h_

#include "g_def.h"

#ifdef DD_WINDOW
#include <windows.h>
BEG_NSP_DDM

// https://docs.microsoft.com/en-us/windows/win32/com/inprocserver32?redirectedfrom=MSDN
enum class com_thread_model
{
    Apartment,  // single threaded
    Both,       // single threaded or multi threaded ***
    Free,       // multi threaded
    Neutral     // 
};
bool write_com_init_register(const std::wstring& clsid, const std::wstring& desc, com_thread_model threadModel, const std::wstring dllFullPath);
bool write_com_uninit_register(const std::wstring& clsid);

END_NSP_DDM
#endif
#endif
