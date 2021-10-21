#ifndef ddcom_tuils_h_
#define ddcom_tuils_h_

#include "g_def.h"

#ifdef DD_WINDOW
#include "ddwin_utils.h"
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
bool com_has_register(const std::wstring& clsid);

#define CONN_CLSID_CLASS(GUID, CLASS) class __declspec(uuid(GUID)) CLASS
#define CLSID_FROM_CLASS(CLASS) __uuidof(CLASS)

#define  DDREF_COUNT_GEN(CALL_TYPE, REF_COUNT_DEF)                                                                        \
    inline unsigned long CALL_TYPE Release()                                                                              \
    {                                                                                                                     \
        unsigned long cRef = static_cast<unsigned long>(InterlockedDecrement(reinterpret_cast<long*>(&REF_COUNT_DEF)));   \
        if (cRef == 0) {                                                                                                  \
            delete this;                                                                                                  \
        }                                                                                                                 \
        return cRef;                                                                                                      \
    }                                                                                                                     \
    inline unsigned long CALL_TYPE AddRef()                                                                               \
    {                                                                                                                     \
        InterlockedIncrement(reinterpret_cast<long*>(&REF_COUNT_DEF));                                                    \
        return REF_COUNT_DEF;                                                                                             \
    }                                                                                                                     \
    inline unsigned long CALL_TYPE RefCount()                                                                             \
    {                                                                                                                     \
        return REF_COUNT_DEF;                                                                                             \
    }                                                                                                                     \
    unsigned long REF_COUNT_DEF = 0;                                                                                      \

template<typename T>
using DDComPtr = Microsoft::WRL::ComPtr<T>;

class DDRefCountBase
{
public:
    virtual ~DDRefCountBase() { }
    DDREF_COUNT_GEN(__stdcall, m_RefCount);
};

struct DDComDesc
{
    GUID comClsId;
    std::wstring comDesc;
    com_thread_model threadModel;
};

class IDDComFactory : public IClassFactory
{
public:
    IDDComFactory() { AddRef(); };
    virtual ~IDDComFactory() = default;
    IDDComFactory(const IDDComFactory&) = delete;
    IDDComFactory(IDDComFactory&&) = delete;
    IDDComFactory& operator= (const IDDComFactory&) = delete;
    IDDComFactory& operator= (IDDComFactory&&) = delete;

    // Д§жиди
    virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) = 0;
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) = 0;
    virtual HRESULT RegisterCore() { return S_OK; };
    virtual HRESULT UnRegisterCore() { return S_OK; };
    virtual DDComDesc GetComDesc() = 0;

public:
    virtual HRESULT Register(const std::wstring& fullDllPath)
    {
        HRESULT hr = S_FALSE;
        do {
            if ((hr = RegisterCore()) != S_OK) {
                break;
            }

            DDComDesc comDesc = GetComDesc();
            std::wstring comClsId;
            if (!ddguid_str(comDesc.comClsId, comClsId)) {
                break;
            }

            hr = write_com_init_register(comClsId, comDesc.comDesc, comDesc.threadModel, fullDllPath);
        } while (0);

        if (hr != S_OK) {
            (void)UnRegister();
        }
        return hr;
    }
    virtual HRESULT UnRegister()
    {
        HRESULT hr = S_FALSE;
        do {
            if ((hr = UnRegisterCore()) != S_OK) {
                break;
            }

            DDComDesc comDesc = GetComDesc();
            std::wstring comClsId;
            if (ddguid_str(comDesc.comClsId, comClsId)) {
                break;
            }
            hr = write_com_uninit_register(comClsId);
        } while (0);
        return hr;
    }
    virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock)
    {
        if (fLock) {
            (void)InterlockedIncrement(reinterpret_cast<long*>(&m_lockCnt));
        } else {
            (void)InterlockedDecrement(reinterpret_cast<long*>(&m_lockCnt));
        }
        return S_OK;
    }
    virtual HRESULT DllGetClassObject(GUID rclsid, void** ppv)
    {
        DDComDesc comDesc = GetComDesc();
        if (rclsid == comDesc.comClsId) {
            AddRef();
            *ppv = this;
            return S_OK;
        }
        return E_NOINTERFACE;
    }
public:
    DDREF_COUNT_GEN(STDMETHODCALLTYPE, m_refCount);
    inline unsigned long GetLockCnt()
    {
        return m_lockCnt;
    }
protected:
    unsigned long m_lockCnt = 0;
};

END_NSP_DDM
#endif
#endif
