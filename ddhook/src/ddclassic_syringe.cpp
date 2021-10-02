#include "ddsyringe.h"
#include "ddsyringe.h"
#include "windows/ddwin_utils.h"
#include "file/dir_utils.h"
#include "file/ddpath.h"
#include <tlhelp32.h>

ddclassic_syringe::ddclassic_syringe(const std::shared_ptr<ddprocess>& process, const std::wstring& dllFullPath)
    : m_process(process), m_dllFullPath(dllFullPath)
{ }

bool ddclassic_syringe::inject_dll(u32 waitTime /*= 5000*/)
{
    if (!ddsyringe::check_param(m_process, m_dllFullPath)) {
        return false;
    }

    return ddsyringe::exec_remote(m_process->get_handle(), &LoadLibraryW,
        (void*)m_dllFullPath.data(), m_dllFullPath.size() * 2, waitTime);
}

bool ddclassic_syringe::uninject_dll(u32 waitTime /*= 5000*/)
{
    return ddsyringe::uninject_dll(m_process, m_dllFullPath, waitTime);
}
