#ifndef ddcom_tuils_h_
#define ddcom_tuils_h_

#include "g_def.h"

#ifdef DD_WINDOW
#include <windows.h>
#include <wrl/client.h>
BEG_NSP_DDM

// https://docs.microsoft.com/en-us/windows/win32/com/inprocserver32?redirectedfrom=MSDN
enum class com_thread_model
{
    Apartment,  // single threaded
    Both,       // single threaded or multi threaded ***
    Free,       // multi threaded
    Neutral     // 
};
//////////////////////////////////////////////////////////////////////////
// HKEY_CLASSES_ROOT\CLSID\{clsid}
//     default desc
//     InprocServer32
//          default dllFullPath
//          ThreadingModel threadModel
//////////////////////////////////////////////////////////////////////////
HRESULT write_com_init_register(const std::wstring& clsid, const std::wstring& desc, com_thread_model threadModel, const std::wstring dllFullPath);
HRESULT write_com_uninit_register(const std::wstring& clsid);

template<typename T>
using DDComPtr = Microsoft::WRL::ComPtr<T>;

class DDRefCountBase
{
public:
    virtual ~DDRefCountBase() { }
    inline unsigned long AddRef() throw()
    {
        InterlockedIncrement(reinterpret_cast<long*>(&m_RefCount));
        return m_RefCount;
    }

    inline unsigned long Release() throw()
    {
        unsigned long cRef = static_cast<unsigned long>(InterlockedDecrement(reinterpret_cast<long*>(&m_RefCount)));
        if (cRef == 0) {
            delete this;
        }

        return cRef;
    }

    inline unsigned long RefCount() const throw()
    {
        return m_RefCount;
    }
protected:
    unsigned long m_RefCount = 0;
};

END_NSP_DDM
#endif
#endif
