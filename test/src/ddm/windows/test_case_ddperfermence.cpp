
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

    std::wstring processName = L"notepad.exe";
    ddgpu_perfermence gpuperfermence;
    while (true) {
        for (const auto& it : adapterDescs) {
            ddgpu_perfermence::gpu_useage_desc desc;
            bool b1 = gpuperfermence.get_gpu_useagedesc_byname(it.AdapterLuid, processName, desc);
            if (!b1 || desc.mem_usage == 0) {
                continue;
            }

            ::Sleep(1000);

            ddgpu_perfermence::gpu_useage_desc desc1;
            bool b2 = gpuperfermence.get_gpu_useagedesc_byname(it.AdapterLuid, processName, desc1);
            if (!b2 || desc1.mem_usage == 0) {
                continue;
            }

            double useage = ddgpu_perfermence::gpu_useage_desc::get_usage(desc, desc1);
            std::wcout << it.Description << std::endl;
            std::wcout << L" useage : " << useage << std::endl;
            std::wcout << L"desc.mem_usage:" << desc.mem_usage / 1024 / 1024 << std::endl;
            std::wcout << L"desc.mem_share_usage:" << desc.mem_share_usage / 1024 / 1024 << std::endl;
            std::wcout << L"desc.mem_commit_usage:" << desc.mem_commit_usage / 1024 / 1024 << std::endl;
            std::wcout << std::endl;
        }
    }
    
}

END_NSP_DDM
