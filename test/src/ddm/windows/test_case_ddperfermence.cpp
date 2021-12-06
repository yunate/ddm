
#include "test_case_factory.h"
#include "windows/ddperfermence.h"
#include "windows/ddprocess.h"


#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "advapi32.lib")

#include <dxgi.h>
#include <vector>

BEG_NSP_DDM

void EnumerateAdapters(std::vector <DXGI_ADAPTER_DESC>& adapterDescs)
{
    IDXGIFactory* pFactory = NULL;
    if (FAILED(::CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)) || pFactory == NULL) {
        return;
    }

    UINT i = 0;
    while (true) {
        IDXGIAdapter* adapter = NULL;
        if (pFactory->EnumAdapters(i++, &adapter) == DXGI_ERROR_NOT_FOUND || adapter == NULL) {
            break;
        }

        DXGI_ADAPTER_DESC desc;
        if (adapter->GetDesc(&desc) == S_OK) {
            adapterDescs.push_back(desc);
        }
        adapter->Release();
    }

    pFactory->Release();
}

TEST(ddperfermence, gpu_useage)
{
    std::vector <DXGI_ADAPTER_DESC> adapterDescs;
    EnumerateAdapters(adapterDescs);
    if (adapterDescs.empty()) {
        return;
    }

    ddgpu_perfermence gpuperfermence;
    for (const auto& it : adapterDescs) {
        ddgpu_perfermence::gpu_useage_desc desc;
        bool b1 = gpuperfermence.get_gpu_useagedesc_byname(it.AdapterLuid, L"notepad.exe", desc);
        ::Sleep(1000);
        ddgpu_perfermence::gpu_useage_desc desc1;
        bool b2 = gpuperfermence.get_gpu_useagedesc_byname(it.AdapterLuid, L"notepad.exe", desc1);
        if (b1 && b2) {
            double useage = ddgpu_perfermence::gpu_useage_desc::get_usage(desc, desc1);
            std::wcout << it.Description << L" useage : " << useage << std::endl;
        }
    }
}

END_NSP_DDM
